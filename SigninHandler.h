#pragma once
#include "Handler.h"
class SigninHandler :public Handler
{
public:
	SigninHandler(MyWeb::Server<MyWeb::HTTP> & server);
	~SigninHandler();
};

