#include "Response.h"


using std::string;
MyWeb::Response::Response()
{
	status = 200;
}


MyWeb::Response::~Response()
{
}

MyWeb::Response& MyWeb::Response::setCookie(string key, string value)
{
    cookies[key].value = value;
    return *this;
}

MyWeb::Response& MyWeb::Response::setCookie(string key,string value, time_t expires){
    cookies[key].value = value;
    struct tm *ptr;
    char buf[50];
    ptr = gmtime(&expires);
    strftime(buf,49,"%a, %d-%b-%y %T GMT",ptr);
    cookies[key].expires = buf;

    return *this;
}

MyWeb::Response & MyWeb::Response::operator>>(ostream& out)
{
	out << getStatusInfo();
	out << "Connection:Keep-Alive\r\n";
	if (!contentType.empty()) {
		out << "Content-Type: " << contentType << "\r\n";
	}
    if(!location.empty()){
        out << "Location: " << location << "\r\n";
    }
    for(auto it = cookies.begin();it != cookies.end();++it){
        CookieStruct & cookieStruct = it->second;

        out << "Set-Cookie: " << it->first << "=" << cookieStruct.value;
        if(cookieStruct.expires.size()){
            out << "; expires=" << cookieStruct.expires;
        }

        out << "\r\n";
    }
	string content = body.str();
	out << "Content-Length: " << content.length() << "\r\n\r\n";
	out << content;

	return *this;
}

string MyWeb::Response::getStatusInfo()
{
	switch (status)
	{
	case 200:
		return "HTTP/1.1 200 OK\r\n";
	case 400:
		return "HTTP/1.1 400 Bad Request\r\n";
	case 404:
		return "HTTP/1.1 404 Not Found\r\n";
	case 302:
		return "HTTP/1.1 302 Found\r\n";
	default:
		return "";
	}
}
