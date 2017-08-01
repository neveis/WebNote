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

void signupPost(Response& response, Request& request) {


	Database *db = Database::getInstance();


	Document d;
	d.SetObject();

	try{
		if(!request.hasBody("username") ||
				!request.hasBody("password")){
			throw IncorrectDataException();
		}

		string username = request.getBody("username");
		string pass = request.getBody("password");

		if(pass.size()<8){
			throw IncorrectDataException();
		}

		regex e("[a-zA-Z0-9]+");
		smatch match;
		if(regex_match(username,match,e)){
			if(match[0] != username){
				throw IncorrectDataException();
			}
		}else{
			throw IncorrectDataException();
		}

		//添加到数据库
		int uid = db->addUser(username, pass);
		if (!uid) {
			d.AddMember("status", false, d.GetAllocator());
			d.AddMember("message", "database error", d.GetAllocator());
		}else{
			Session session;
			SessionInfo info;
			info.UID = uid;
			SessionKeyType sessionkey = session.CreateSession(info);

			time_t t;
			time(&t);
			t = t + 30*24*60*60;
			response.setCookie("UID",to_string(uid));
			response.setCookie("token",sessionkey,t);
			d.AddMember("status", true, d.GetAllocator());
		}
	}catch(exception const& e){
		d.AddMember("status", false, d.GetAllocator());
		Value message;
		message.SetString(e.what(),d.GetAllocator());
		d.AddMember("message", message, d.GetAllocator());
	}catch(...){
		d.AddMember("status", false, d.GetAllocator());
		Value message;
		d.AddMember("message", "unknown error", d.GetAllocator());
	}

	string resContent;
	Json2String(d, resContent);
	response.setContentType("application/json");
	response << resContent;

}

void checkUserNamePost(Response& response, Request& request){
	Database *db = Database::getInstance();
	Session session;
	Document d;
	d.SetObject();
	try{
		if(!request.hasBody("username")){
			throw IncorrectDataException();
		}
		string uname = request.getBody("username");
		if(db->checkUserExist(uname)){
			d.AddMember("status",false,d.GetAllocator());
		}else{
			d.AddMember("status",true,d.GetAllocator());
		}
	}catch(exception const& e){
		d.AddMember("status", false, d.GetAllocator());
		Value message;
		message.SetString(e.what(),d.GetAllocator());
		d.AddMember("message", message, d.GetAllocator());
	}catch(...){
		d.AddMember("status",false,d.GetAllocator());
		d.AddMember("message", "unknown error", d.GetAllocator());
	}

	string content;
	Json2String(d, content);
	response.setContentType("application/json");
	response << content;
}

SignupHandler::SignupHandler(ServerHttp & server)
{
	server.addResource("^/signup/?$","POST", signupPost);
	server.addResource("^/checkuname/?$","POST",checkUserNamePost);
}