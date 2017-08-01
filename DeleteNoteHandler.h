//
// Created by root on 5/31/17.
//

#ifndef WEBNOTE_DELETENOTEHANDLER_H
#define WEBNOTE_DELETENOTEHANDLER_H
#include "Handler.h"

class DeleteNoteHandler :public Handler{
public:
    DeleteNoteHandler(MyWeb::ServerHttp & server);
    ~DeleteNoteHandler();
};


#endif //WEBNOTE_DELETENOTEHANDLER_H
