//
// Created by root on 5/27/17.
//

#include "SignoutHandler.h"

using namespace std;
using namespace MyWeb;
using namespace rapidjson;

void signoutGet(Response& response, Request& request){
    if(request.hasCookie("token")) {
        Session session;
        session.DeleteSession(request.getCookie("token"));
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
    Document d;
    d.SetObject();
    //remove session
    if(request.hasCookie("token")) {
        Session session;
        session.DeleteSession(request.getCookie("token"));
        //remove cookies
        time_t t;
        time(&t);
        t -= 60 * 60 * 24;
        response.setCookies("UID", "", t);
        response.setCookies("token", "", t);
        d.AddMember("status",true,d.GetAllocator());
    }else{
        d.AddMember("status",false,d.GetAllocator());
    }

    string content;
    Json2String(d,content);

    response.type = "application/json";
    response.body << content;

}

SignoutHandler::SignoutHandler(MyWeb::Server<MyWeb::HTTP> &server) {
    server.resource["^/signout/?$"]["GET"] = signoutGet;
    server.resource["^/signout/?$"]["POST"] = signoutPost;
}

SignoutHandler::~SignoutHandler() {

}
