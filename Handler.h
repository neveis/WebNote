/*
 处理句柄基类
*/

#pragma once
#ifndef _HANDLER_H
#define _HANDLER_H

#include "ServerHttp.h"
#include <string>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include "Database.h"
#include "Session.h"
#include "Exception.hpp"
#include "struct.h"

class Handler {
public:
	Handler() {};
	virtual ~Handler() {};

private:
	Handler(const Handler&) = delete;
	Handler& operator=(const Handler&) = delete;
};

void String2Json(std::string const& str, rapidjson::Document &d);
void Json2String(rapidjson::Document &d, std::string &str);

#define REGISTER_HANDLER(server,handlerName) {handlerName h(server);}

#endif // !_HANDLER_H

