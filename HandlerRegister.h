#ifndef _HANDLER_REGISTER_H
#define _HANDLER_REGISTER_H

#include "Handler.h"

//添加一个静态函数用于注册处理函数

class HandlerRegister {
public:
	static void Register(MyWeb::ServerHttp& server);

private:
	HandlerRegister();
	~HandlerRegister();
};

#endif // !_HANDLER_REGISTER_H
