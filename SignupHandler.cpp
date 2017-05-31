/*
* 注册相关操作
* 接受一个POST请求，请求包含账号和密码
* 检测账号是否已存在，存在则返回注册失败
* 不存在则添加到数据库中（需对密码进行加密）
*/


#include <fstream>
#include <cstdio>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include "SignupHandler.h"

using namespace std;
using namespace MyWeb;
using namespace rapidjson;

void signupGet(Response& response, Request& request) {
	if(request.cookies.find("token")!= request.cookies.end()){
		Session session;

		SessionInfo *info = session.GetSessionInfo(request.cookies["token"]);
		if(info == nullptr || info->UID != atoi(request.cookies["UID"].c_str())){
			//remove cookies
			time_t t;
			time(&t);
			t-=60*60*24;
			response.setCookies("UID","",t);
			response.setCookies("token","",t);
			session.DeleteSession(request.cookies["tooken"]);
		}else{
			response.redirect("/");
			return;
		}
	}

	string filename = "view/signup.html";
	ifstream ifs;

	ifs.open(filename, ifstream::in);

	if (ifs) {
		ifs.seekg(0, ios::end);
		size_t length = ifs.tellg();

		ifs.seekg(0, ios::beg);

		// 文件内容拷贝到 response-stream 中，不应该用于大型文件
		response.body << ifs.rdbuf();

		ifs.close();
	}
	else {
		// 文件不存在时，返回无法打开文件
		string content = "Could not open file " + filename;
		response.status = 400;
		response.body << content;
	}
}

void signupPost(Response& response, Request& request) {


	Database *db = Database::getInstance();

	string username = request.body["username"];
	string pass = request.body["password"];

	bool status;
	//添加到数据库
	int uid = db->addUser(username, pass);
	if (!uid) {
		cout << "注册失败" << endl;
		status = false;
	}else{
		status = true;
		Session session;
		SessionInfo info;
		info.UID = uid;
		SessionKeyType sessionkey = session.CreateSession(info);

		time_t t;
		time(&t);
		t = t + 30*24*60*60;
		response.setCookies("UID",to_string(uid));
		response.setCookies("token",sessionkey,t);

	}

	string resContent;
	/*
	stringstream s;
	s << "user: " << username << endl;
	s << "password: " << pass << endl;
	resContent = s.str();
	*/
	Document d;
	d.SetObject();
	Value statusValue;
	statusValue.SetBool(status);
	d.AddMember("status", statusValue, d.GetAllocator());
	Value href;
	href = "/";
	d.AddMember("href",href,d.GetAllocator());

	Json2String(d, resContent);

	response.body << resContent;

}


SignupHandler::SignupHandler(Server<HTTP> & server)
{
	server.resource["^/signup/?$"]["GET"] = signupGet;
	server.resource["^/signup/?$"]["POST"] = signupPost;
}