#include <iostream>
#include "ServerHttp.h"
#include "HandlerRegister.h"
using namespace MyWeb;

int main(){

	ServerHttp server(10080,4);
	HandlerRegister::Register(server);

    std::cout << "server start on 10080" << std::endl;
	server.start();
	
	return 0;
}
