#pragma once
#include "Handler.h"
class SigninHandler :public Handler
{
public:
	SigninHandler(MyWeb::ServerHttp & server);
	~SigninHandler();
};

