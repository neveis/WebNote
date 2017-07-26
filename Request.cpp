#include "Request.h"

using namespace std;

MyWeb::Request::Request()
{
}


MyWeb::Request::~Request()
{
}

MyWeb::Request::Cookies MyWeb::parseCookies(string cookiesStr) {
    Request::Cookies cookies;
	regex e("( |^)(.*?)=([^;]*)(;|$)");
	smatch match;
	while (regex_search(cookiesStr, match, e)) {
		cookies[match[2]] = match[3];
		cookiesStr = match.suffix();
	}

	return cookies;
}

void MyWeb::Request::parseBody(string rawstr)
{
	regex r("^(.*?)=(.*?)(&|$)");
	smatch match;
	while (regex_search(rawstr, match, r)) {
		body[match[1]] = match[2];
		rawstr = match.suffix();
	}

}

shared_ptr<MyWeb::Request> MyWeb::parseRequest(istream & stream)
{
	auto spRequest = make_shared<Request>();
    Request &request = *spRequest;

	regex e("^([^ ]*) ([^ ]*) HTTP/([^ ]*)$");

	smatch sub_match;

	string line;
	getline(stream, line);
	// 头信息以/r/n换行，getline不会读取/n，所以还需要把/r去掉。
	line.pop_back();

	if (regex_match(line, sub_match, e)) {
		request.method = sub_match[1];
		request.path = sub_match[2];
		request.httpVersion = sub_match[3];

		//解析path，分离query
		regex r("^(/.*?)(\\?(.*))?$");
		smatch match;
		string strTemp = sub_match[2];
		regex_match(strTemp, match, r);
		//出错怎么办？
		request.path = match[1];
		request.queryStr = match[3];

		//解析query
		regex r1("^(.*?)=(.*?)(&|$)");
		smatch match2;
		strTemp = request.queryStr;
		while (regex_search(strTemp, match2, r1)) {
			request.query[match2[1]] = match2[2];
			strTemp = match2.suffix();
		}

		//解析头信息
		bool matched;
		e = "^([^:]*): ?(.*)$";
		do {
			getline(stream, line);
			line.pop_back();
			matched = regex_match(line, sub_match, e);
			if (matched) {
				request.header[sub_match[1]] = sub_match[2];
				//如果是cookie，进一步解析cookie
				if (sub_match[1] == "Cookie") {
					request.cookies = parseCookies(sub_match[2]);
				}
			}
		} while (matched);
	}
	return spRequest;
}