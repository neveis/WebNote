//
// Created by root on 5/30/17.
//

#ifndef WEBNOTE_CREATENOTEHANDLER_H
#define WEBNOTE_CREATENOTEHANDLER_H

#include "Handler.h"

class CreateNoteHandler:public Handler {
public:
    CreateNoteHandler(MyWeb::Server<MyWeb::HTTP> & server);
    ~CreateNoteHandler();
private:
    NO_COPYABLE(CreateNoteHandler)
};


#endif //WEBNOTE_CREATENOTEHANDLER_H
