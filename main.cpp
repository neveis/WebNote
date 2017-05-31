#include "server_http.hpp"
#include <iostream>
#include "HandlerRegister.h"
using namespace MyWeb;

int main(){

	Server<HTTP> server(12345,4);
    HandlerRegister *hr = new HandlerRegister(server);
	delete hr;

    std::cout << "server start" << std::endl;
	server.start();
	
	return 0;
}
