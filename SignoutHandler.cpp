//
// Created by root on 5/27/17.
//

#include "SignoutHandler.h"

using namespace std;
using namespace MyWeb;
using namespace rapidjson;

void signoutGet(Response& response, Request& request){
    if(request.cookies.find("token")!= request.cookies.end()) {
        Session session;
        session.DeleteSession(request.cookies["token"]);
        //remove cookies
        time_t t;
        time(&t);
        t -= 60 * 60 * 24;
        response.setCookies("UID", "", t);
        response.setCookies("token", "", t);
    }
    response.redirect("/");
}

void signoutPost(Response& response, Request& request){
    //remove session
    int uid = atoi(request.body["UID"].c_str());

    Document d;
    d.SetObject();
    Value status;
    status = "success";

    d.AddMember("status",status,d.GetAllocator());

    string content;
    Json2String(d,content);

    //remove cookies

    response.body << content;

}

SignoutHandler::SignoutHandler(MyWeb::Server<MyWeb::HTTP> &server) {
    server.resource["^/signout/?$"]["GET"] = signoutGet;
}

SignoutHandler::~SignoutHandler() {

}
