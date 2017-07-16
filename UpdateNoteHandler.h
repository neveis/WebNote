//
// Created by root on 5/31/17.
//

#ifndef WEBNOTE_UPDATENOTEHANDLER_H
#define WEBNOTE_UPDATENOTEHANDLER_H
#include "Handler.h"

class UpdateNoteHandler :public Handler{
public:
    UpdateNoteHandler(MyWeb::Server<MyWeb::HTTP> & server);
    ~UpdateNoteHandler();
};


#endif //WEBNOTE_UPDATENOTEHANDLER_H
