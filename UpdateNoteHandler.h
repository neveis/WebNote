//
// Created by root on 5/31/17.
//

#ifndef WEBNOTE_UPDATENOTEHANDLER_H
#define WEBNOTE_UPDATENOTEHANDLER_H
#include "Handler.h"

class UpdateNoteHandler :public Handler{
public:
    UpdateNoteHandler(MyWeb::ServerHttp & server);
    ~UpdateNoteHandler();
};


#endif //WEBNOTE_UPDATENOTEHANDLER_H
