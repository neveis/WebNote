#include "server_http.hpp"
#include "ServerHttp.h"

MyWeb::ServerHttp::ServerHttp(unsigned short port,size_t num_threads) {
    impl = new Server<HTTP>(port,num_threads);
}

MyWeb::ServerHttp::~ServerHttp() {
    delete impl;
}

void MyWeb::ServerHttp::addResource(std::string path, std::string method,
                                    std::function<void(Response &, Request &)> callback) {
    (*impl).resource[path][method] = callback;
}

void MyWeb::ServerHttp::addDefaultResource(std::string path, std::string method,
                                           std::function<void(Response &, Request &)> callback) {
    (*impl).default_resource[path][method] = callback;
}

void MyWeb::ServerHttp::start() {
    impl->start();
}
