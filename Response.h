#pragma once
#include <string>
#include <sstream>
#include <unordered_map>
#include <ostream>
#include <ctime>

namespace MyWeb {
	using namespace std;
	class Response
	{

	public:
		Response();
		~Response();
		Response& setStatus(int);
		Response& setStatus(char*);
		Response& setStatus(string);
		Response& setContentType(string);
		Response& setCookie(string key, string value);
		Response& setCookie(string key,string value, time_t expires);
		Response& redirect(string url);
		Response& operator<<(string);
		Response& operator>>(ostream&);
	private:
		int status;
		string contentType;
		string location;
		stringstream body;
		struct CookieStruct{
			string value;
			string expires;
		};
		typedef unordered_map<string, CookieStruct> Cookies;
		Cookies cookies;
		string getStatusInfo();
	};

}

inline MyWeb::Response & MyWeb::Response::setStatus(int s)
{
	status = s;
	return *this;
}

inline MyWeb::Response & MyWeb::Response::setStatus(char *s)
{
	status = atoi(s);
	return *this;
}

inline MyWeb::Response & MyWeb::Response::setStatus(string s)
{
	status = atoi(s.c_str());
	return *this;
}

inline MyWeb::Response & MyWeb::Response::setContentType(string type)
{
	contentType = type;
	return *this;
}

inline MyWeb::Response & MyWeb::Response::operator<<(string str)
{
	body << str;
	return *this;
}

inline MyWeb::Response& MyWeb::Response::redirect(string url){
	status = 302;
	location = url;
    return *this;
}