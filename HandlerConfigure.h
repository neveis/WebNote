#pragma once
#ifndef _HANDLER_CONFIGURE_H
#define _HANDLER_CONFIGURE_H

#include "Handler.h"
#include "SignupHandler.h"
#include "SigninHandler.h"
#include "SignoutHandler.h"
#include "CreateNoteHandler.h"
#include "FetchNoteHandler.h"
#include "UpdateNoteHandler.h"
#include "DeleteNoteHandler.h"

#define REGISTER_HANDLERS(server)	\
REGISTER_HANDLER(server,SignupHandler)\
REGISTER_HANDLER(server, SigninHandler)\
REGISTER_HANDLER(server, SignoutHandler)\
REGISTER_HANDLER(server, CreateNoteHandler)\
REGISTER_HANDLER(server, FetchNoteHandler)\
REGISTER_HANDLER(server, UpdateNoteHandler)\
REGISTER_HANDLER(server, DeleteNoteHandler)
#endif // !_HANDLER_CONFIGURE_H
