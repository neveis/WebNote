//
// Created by root on 5/30/17.
//

#include "CreateNoteHandler.h"
#include <rapidjson/document.h>

using namespace std;
using namespace MyWeb;
using namespace rapidjson;

void createNotePost(Response& response, Request& request){
    Session session;
    Document d;
    d.SetObject();
    try {
        if (request.cookies.find("token") == request.cookies.end() ||
            request.cookies.find("UID") == request.cookies.end()){
            throw IncorrectDataException();
        }

        SessionInfo *info = session.GetSessionInfo(request.cookies["token"]);
        if (info == nullptr || info->UID != atoi(request.cookies["UID"].c_str())) {
            throw IncorrectSessionException();
        }

        if (request.body.find("title") == request.body.end() ||
            request.body.find("content") == request.body.end()){
            throw IncorrectDataException();
        }

        string &title = request.body["title"];
        string &content = request.body["content"];
        Database *db = Database::getInstance();
        int nid = db->createNote(info->UID, title, content, 0);

        if(!nid){
            throw HandlingFailureException();
        }
        Value status;
        status.SetBool(true);
        Value nidValue;
        nidValue.SetInt(nid);
        d.AddMember("status", status, d.GetAllocator());
        d.AddMember("nid", nidValue, d.GetAllocator());
        string res;
        Json2String(d, res);
        response.type = "application/json";
        response.body << res;
    }catch(exception const& e){
        Value status;
        status.SetBool(false);
        Value message;
        message.SetString(e.what(),d.GetAllocator());
        d.AddMember("status", status, d.GetAllocator());
        d.AddMember("message", message, d.GetAllocator());
        string res;
        Json2String(d, res);
        response.type = "application/json";
        response.body << res;
    }
}

CreateNoteHandler::CreateNoteHandler(MyWeb::Server<MyWeb::HTTP> &server) {
    server.resource["^/create/?$"]["POST"] = createNotePost;
}

CreateNoteHandler::~CreateNoteHandler() {

}
