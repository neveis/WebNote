#ifndef WEBNOTE_CREATENOTEHANDLER_H
#define WEBNOTE_CREATENOTEHANDLER_H

#include "Handler.h"

class CreateNoteHandler:public Handler {
public:
    CreateNoteHandler(MyWeb::ServerHttp & server);
    ~CreateNoteHandler();
};


#endif //WEBNOTE_CREATENOTEHANDLER_H
