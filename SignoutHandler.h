//
// Created by root on 5/27/17.
//

#ifndef _SIGNOUTHANDLER_H
#define _SIGNOUTHANDLER_H

#include "Handler.h"

class SignoutHandler : public Handler {
public:
    SignoutHandler(MyWeb::Server<MyWeb::HTTP> & server);
    ~SignoutHandler();
private:
    NO_COPYABLE(SignoutHandler)
};


#endif //_SIGNOUTHANDLER_H
