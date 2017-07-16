#pragma once
#ifndef _SIGNUP_HANDLER_H
#define _SIGNUP_HANDLER_H


#include "Handler.h"

class SignupHandler:public Handler {
public:
	SignupHandler(MyWeb::Server<MyWeb::HTTP> &server);
	~SignupHandler() {};
};

#endif // !_SIGNUP_HANDLER_H