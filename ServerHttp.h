#ifndef WEBNOTE_SERVERHTTP_H
#define WEBNOTE_SERVERHTTP_H
#include <functional>
#include <string>
#include "Response.h"
#include "Request.h"
#include <boost/asio.hpp>

//前置声明，减少编译依赖
template <typename T>
class Server;

namespace MyWeb {
    class ServerHttp {
    public:
        ServerHttp(unsigned short port,size_t num_threads=1);
        ~ServerHttp();
        void addResource(std::string path,std::string method,std::function<void(MyWeb::Response&, MyWeb::Request&)> callback);
        void addDefaultResource(std::string path,std::string method,std::function<void(MyWeb::Response&, MyWeb::Request&)> callback);
        void start();
    private:
        Server<boost::asio::ip::tcp::socket> *impl;
    };
}

#endif //WEBNOTE_SERVERHTTP_H
