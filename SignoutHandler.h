//
// Created by root on 5/27/17.
//

#ifndef _SIGNOUTHANDLER_H
#define _SIGNOUTHANDLER_H

#include "Handler.h"

class SignoutHandler : public Handler {
public:
    SignoutHandler(MyWeb::ServerHttp & server);
    ~SignoutHandler();
};


#endif //_SIGNOUTHANDLER_H
