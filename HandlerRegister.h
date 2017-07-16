#ifndef _HANDLER_REGISTER_H
#define _HANDLER_REGISTER_H

#include "Handler.h"

class HandlerRegister {
public:
	HandlerRegister(MyWeb::Server<MyWeb::HTTP> & server);
	~HandlerRegister();
};

#endif // !_HANDLER_REGISTER_H
