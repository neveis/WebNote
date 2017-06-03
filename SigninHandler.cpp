#include "SigninHandler.h"
#include "Database.h"
#include <fstream>
#include <rapidjson/document.h>

using namespace std;
using namespace MyWeb;
using namespace rapidjson;

void signinGet(Response& response, Request& request) {
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

	string filename = "view/signin.html";
	ifstream ifs;

	ifs.open(filename, ifstream::in);

	if (ifs) {
		ifs.seekg(0, ios::end);
		size_t length = ifs.tellg();

		ifs.seekg(0, ios::beg);

		//
		response.body << ifs.rdbuf();

		ifs.close();
	}
	else {
		//
		string content = "Could not open file " + filename;
		response.status = 400;
		response.body << content;
	}
}

void verityTokenPost(Response& response, Request& request){
    Database *db = Database::getInstance();
    Session session;
    Document d;
    d.SetObject();
    try{
        if (request.cookies.find("token") == request.cookies.end() ||
            request.cookies.find("UID") == request.cookies.end()){
            d.AddMember("status",false,d.GetAllocator());
        }else{
            SessionInfo *info = session.GetSessionInfo(request.cookies["token"]);
            if (info == nullptr || info->UID != atoi(request.cookies["UID"].c_str())) {
                throw IncorrectSessionException();
            }

            string username = db->getUserName(info->UID);
            if(username.size()){
                d.AddMember("status",true,d.GetAllocator());
                Value name;
                name.SetString(username.c_str(),d.GetAllocator());
                d.AddMember("username",name,d.GetAllocator());
            }else{
                d.AddMember("status",false,d.GetAllocator());
            }
        }
    }catch(IncorrectSessionException const& e){
        d.AddMember("status", false, d.GetAllocator());
        Value message;
        message.SetString(e.what(),d.GetAllocator());
        d.AddMember("message", message, d.GetAllocator());

        time_t t;
        time(&t);
        t -= 60*60*24;
        response.setCookies("UID","",t);
        response.setCookies("token","",t);

    }catch(...){
        d.AddMember("status",false,d.GetAllocator());
    }

    string content;
    Json2String(d, content);
    response.type = "application/json";
    response.body << content;
}

void signinPost(Response& response, Request& request) {
	Database *db = Database::getInstance();
	string username = request.body["username"];
	string password = request.body["password"];
    Document d;
    d.SetObject();
	int uid = db->signin(username, password);

	if (!uid) {
        d.AddMember("status", false, d.GetAllocator());
	}else{
        Session session;
        SessionInfo info;
        info.UID = uid;
        SessionKeyType sessionkey = session.CreateSession(info);

        time_t t;
        time(&t);
        //one month
        t = t + 30*24*60*60;
        response.setCookies("UID",to_string(uid));
        response.setCookies("token",sessionkey,t);
        d.AddMember("status", true, d.GetAllocator());
    }

	string content;
	Json2String(d, content);
    response.type = "application/json";
	response.body << content;
}

SigninHandler::SigninHandler(MyWeb::Server<MyWeb::HTTP> & server)
{
	server.resource["^/signin/?$"]["POST"] = signinPost;
    server.resource["^/verity/?$"]["POST"] = verityTokenPost;

}


SigninHandler::~SigninHandler()
{
}
