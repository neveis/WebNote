#include "Handler.h"

void String2Json(std::string const & str, rapidjson::Document & d)
{
	//TODO EXCEPTION
	d.Parse(str.c_str());
}

void Json2String(rapidjson::Document & d, std::string &str)
{
	//TODO EXCEPTION
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	d.Accept(writer);
	str = buffer.GetString();
}
