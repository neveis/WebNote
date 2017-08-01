//
// Created by root on 5/30/17.
//

#ifndef WEBNOTE_FETCHNOTEHANDLER_H
#define WEBNOTE_FETCHNOTEHANDLER_H
#include "Handler.h"

class FetchNoteHandler :public Handler{
public:
    FetchNoteHandler(MyWeb::ServerHttp & server);
    ~FetchNoteHandler();
};


#endif //WEBNOTE_FETCHNOTEHANDLER_H
