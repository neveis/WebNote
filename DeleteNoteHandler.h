//
// Created by root on 5/31/17.
//

#ifndef WEBNOTE_DELETENOTEHANDLER_H
#define WEBNOTE_DELETENOTEHANDLER_H
#include "Handler.h"

class DeleteNoteHandler :public Handler{
public:
    DeleteNoteHandler(MyWeb::Server<MyWeb::HTTP> & server);
    ~DeleteNoteHandler();
private:
    NO_COPYABLE(DeleteNoteHandler)
};


#endif //WEBNOTE_DELETENOTEHANDLER_H
