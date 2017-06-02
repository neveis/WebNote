/*
 处理句柄基类
*/

#pragma once
#ifndef _HANDLER_H
#define _HANDLER_H

#include "server_http.hpp"
#include <string>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include "Database.h"
#include "Session.h"
#include "Exception.hpp"
#include "struct.h"

#define NO_COPYABLE(Class)	\
Class(const Class&){};		\
Class &operator=(const Class &) {};

class Handler {
public:
	Handler() {};
	virtual ~Handler() {};

private:
	NO_COPYABLE(Handler)
};

void String2Json(std::string const& str, rapidjson::Document &d);
void Json2String(rapidjson::Document &d, std::string &str);

#define REGISTER_HANDLER(server,handlerName)		\
{													\
	Handler* handler = new handlerName(server);		\
	delete handler;									\
}

#endif // !_HANDLER_H
