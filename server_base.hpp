#ifndef SERVER_BASE_HPP
#define SERVER_BASE_HPP

#include <boost/asio.hpp>

#include <regex>
#include <unordered_map>
#include <thread>
#include <iostream>
#include <ctime>

namespace MyWeb {
   
	typedef std::unordered_map<std::string, std::string> Cookies;

	struct Request {
         std::string method, path, http_version,queryStr;
         std::shared_ptr<std::istream> content;
         std::unordered_map<std::string,std::string> header;
		 std::unordered_map<std::string, std::string> query;
		 std::unordered_map<std::string, std::string> body;
		 Cookies cookies;
         std::smatch path_match;    
	};

	struct SetCookieStruct{
		std::string value;
		std::string expires;
	};
	struct Response {
		int status = 200;
		std::stringstream body;
		std::unordered_map<std::string,SetCookieStruct> cookies;
		std::string type;
		std::string location;
		std::ostream *writeBuf;
		void write() {
			switch (status)
			{
				case 200:
					*writeBuf << "HTTP/1.1 200 OK\r\n";
					break;
				case 400:
					*writeBuf << "HTTP/1.1 400 Bad Request\r\n";
					break;
				case 404:
					*writeBuf << "HTTP/1.1 404 Not Found\r\n";
					break;
				case 302:
					*writeBuf << "HTTP/1.1 302 Found\r\n";
					break;
				default:
					break;
			}
			*writeBuf << "Connection:Keep-Alive\r\n";
			if (!type.empty()) {
				*writeBuf << "Content-Type: " << type << "\r\n";
			}
			if(!location.empty()){
				*writeBuf << "Location: " << location << "\r\n";
			}
			for(auto it = cookies.begin();it != cookies.end();++it){
				SetCookieStruct & cookieStruct = it->second;

				*writeBuf << "Set-Cookie: " << it->first << "=" << cookieStruct.value;
				if(cookieStruct.expires.size()){
					*writeBuf << "; " << "expires=" << cookieStruct.expires;
				}

				*writeBuf << "\r\n";
			}
			std::string content = body.str();
			*writeBuf << "Content-Length: " << content.length() << "\r\n\r\n";
			*writeBuf << content;
		}
		void setCookies(std::string const& key,std::string const& value){
			cookies[key].value = value;
		}
		void setCookies(std::string const& key,std::string const& value, time_t const& expires){
			cookies[key].value = value;
			struct tm *ptr;
			char buf[50];
			ptr = gmtime(&expires);
			strftime(buf,49,"%a, %d-%b-%y %T GMT",ptr);
			cookies[key].expires = buf;
		}
		void redirect(std::string const& url){
			status = 302;
			location = url;
		}
	};

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

		Cookies parse_cookies(std::string cookiesStr) const {
			Cookies cookies;
			std::regex e("( |^)(.*?)=([^;]*)(;|$)");
			std::smatch match;
			while (std::regex_search(cookiesStr, match, e)) {
				cookies[match[2]] = match[3];
				cookiesStr = match.suffix();
			}

			return cookies;
		}

		Request parse_request(std::istream& stream) const{
			Request request;

			std::regex e("^([^ ]*) ([^ ]*) HTTP/([^ ]*)$");

			std::smatch sub_match;

			std::string line;
			getline(stream,line);
			line.pop_back();

			if(std::regex_match(line,sub_match,e)){
				request.method = sub_match[1];
				request.path = sub_match[2];
				request.http_version = sub_match[3];
				
				//匹配path
				std::regex r("^(/.*?)(\\?(.*))?$");
				std::smatch match;
				std::string strTemp = sub_match[2];
				std::regex_match(strTemp, match, r);
				request.path = match[1];
				request.queryStr = match[3];

				//解析query
				std::regex r1("^(.*?)=(.*?)(&|$)");
				std::smatch match2;
				strTemp = request.queryStr;
				while (regex_search(strTemp, match2, r1)) {
					request.query[match2[1]] = match2[2];
					strTemp = match2.suffix();
				}

				bool matched;
				e = "^([^:]*): ?(.*)$";
				do {
					getline(stream,line);
					line.pop_back();
					matched=std::regex_match(line,sub_match,e);
					if(matched){
						request.header[sub_match[1]] = sub_match[2];
						if (sub_match[1] == "Cookie") {
							request.cookies = parse_cookies(sub_match[2]);
						}
					}
				}while(matched==true);
			}
			return request;
		}

		void process_request_and_respond(std::shared_ptr<socket_type> socket) const{
			auto read_buffer = std::make_shared<boost::asio::streambuf>();
			
			boost::asio::async_read_until(*socket,*read_buffer,"\r\n\r\n",
			[this,socket,read_buffer](const boost::system::error_code& ec,size_t bytes_transferred){
				if(!ec){
					size_t total = read_buffer->size();
					std::istream stream(read_buffer.get());

					auto request = std::make_shared<Request>();
					*request = parse_request(stream);

					size_t num_additional_bytes = total - bytes_transferred;

					if(request->header.count("Content-Length")>0){
						boost::asio::async_read(*socket,*read_buffer,
						boost::asio::transfer_exactly(std::stoull(request->header["Content-Length"])- num_additional_bytes),
						[this,socket,read_buffer,request](const boost::system::error_code& ec,size_t bytes_transferred){
							if(!ec){
								request->content = std::shared_ptr<std::istream>(new std::istream(read_buffer.get()));
								//解析body
								std::regex r("^(.*?)=(.*?)(&|$)");
								std::smatch match;
                                std::string &rawstr = request->body["raw"];
                                std::string strTemp;
                                while(std::getline(*request->content,strTemp)) {
                                    rawstr.append(strTemp);
                                }
                                strTemp = rawstr;
								while (regex_search(strTemp, match, r)) {
									request->body[match[1]] = match[2];
                                    strTemp = match.suffix();
								}
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
            for(auto res_it: all_resources) {
                std::regex e(res_it->first);
                std::smatch sm_res;
                if(std::regex_match(request->path, sm_res, e)) {
                    if(res_it->second.count(request->method)>0) {
                        request->path_match = move(sm_res);
						
                        // 会被推导为 std::shared_ptr<boost::asio::streambuf>
                        auto write_buffer = std::make_shared<boost::asio::streambuf>();
                        std::ostream wb(write_buffer.get());
						auto response = std::make_shared<Response>();
						response->writeBuf = &wb;

                        res_it->second[request->method](*response, *request);
						response->write();

                        // 在 lambda 中捕获 write_buffer 使其不会再 async_write 完成前被销毁
                        boost::asio::async_write(*socket, *write_buffer,
                        [this, socket, request, write_buffer](const boost::system::error_code& ec, size_t bytes_transferred) {
                            //HTTP 持久连接(HTTP 1.1):
                            if(!ec && stof(request->http_version)>1.05)
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
