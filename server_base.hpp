﻿#ifndef SERVER_BASE_HPP
#define SERVER_BASE_HPP

#include <boost/asio.hpp>
#include <regex>
#include <unordered_map>
#include <thread>
#include <iostream>
#include <ctime>
#include "Request.h"
#include "Response.h"
namespace MyWeb {

	typedef std::map<std::string, std::unordered_map<std::string,
		    std::function<void(Response&, Request&)>>> resource_type;

	template <typename socket_type>
    class ServerBase{
    public:
		resource_type resource;
		resource_type default_resource;

		ServerBase(unsigned short port,size_t num_threads=1):
			endpoint(boost::asio::ip::tcp::v4(),port),
			acceptor(m_io_service,endpoint),
			num_threads(num_threads){}

		void start(){
			for(auto it=resource.begin();it!=resource.end();++it){
				all_resources.push_back(it);
			}
			for(auto it=default_resource.begin();it!=default_resource.end();++it){
				all_resources.push_back(it);
			}
			
			accept();

			for(size_t i=1;i<num_threads;i++){
				threads.emplace_back([this](){
					m_io_service.run();
				});
			}

			m_io_service.run();

			for(auto& t:threads){
				t.join();
			}
		};
	protected:
		boost::asio::io_service m_io_service;
		boost::asio::ip::tcp::endpoint endpoint;
		boost::asio::ip::tcp::acceptor acceptor;
		std::vector<resource_type::iterator> all_resources;

		size_t num_threads;
		std::vector<std::thread> threads;

		virtual void accept(){}
		
		void process_request_and_respond(std::shared_ptr<socket_type> socket) const{
			auto read_buffer = std::make_shared<boost::asio::streambuf>();
			
			boost::asio::async_read_until(*socket,*read_buffer,"\r\n\r\n",
			[this,socket,read_buffer](const boost::system::error_code& ec,size_t bytes_transferred){
				if(!ec){
					size_t total = read_buffer->size();
					std::istream stream(read_buffer.get());

					std::shared_ptr<Request> request = parseRequest(stream);

					size_t num_additional_bytes = total - bytes_transferred;

					if(request->getHeader("Content-Length").size()>0){
						boost::asio::async_read(*socket,*read_buffer,
						boost::asio::transfer_exactly(std::stoull(request->getHeader("Content-Length"))- num_additional_bytes),
						[this,socket,read_buffer,request](const boost::system::error_code& ec,size_t bytes_transferred){
							if(!ec){
								auto bodyStream = std::shared_ptr<std::istream>(new std::istream(read_buffer.get()));
								//解析body
                                std::string rawstr;
                                std::string strTemp;
                                while(std::getline(*bodyStream,strTemp)) {
                                    rawstr.append(strTemp);
                                }
                                request->setRawBody(rawstr);

								respond(socket,request);
							}
						});
					}else{
						respond(socket,request);
					}
				}	
			});
		};

	    void respond(std::shared_ptr<socket_type> socket, std::shared_ptr<Request> request) const {
            // 对请求路径和方法进行匹配查找，并生成响应
            std::string path = request->getPath();
            for(auto res_it: all_resources) {
                std::regex e(res_it->first);
                std::smatch sm_res;
                if(std::regex_match(path, sm_res, e)) {
                    if(res_it->second.count(request->getMethod())>0) {
                        //request->path_match = move(sm_res);

                        // 会被推导为 std::shared_ptr<boost::asio::streambuf>
                        auto write_buffer = std::make_shared<boost::asio::streambuf>();
                        std::ostream wb(write_buffer.get());
						auto response = std::make_shared<Response>();

                        res_it->second[request->getMethod()](*response, *request);
						*response >> wb;

                        // 在 lambda 中捕获 write_buffer 使其不会再 async_write 完成前被销毁
                        boost::asio::async_write(*socket, *write_buffer,
                        [this, socket, request, write_buffer](const boost::system::error_code& ec, size_t bytes_transferred) {
                            //HTTP 持久连接(HTTP 1.1):
                            if(!ec && stof(request->getHttpVersion())>1.05)
                                process_request_and_respond(socket);
                        });
                        return;
                    }
                }
            }
        }

    };

	template<typename socket_type>
	class Server : public ServerBase<socket_type>{};	

}
#endif /* SERVER_BASE_HPP */
