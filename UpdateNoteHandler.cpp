//
// Created by root on 5/31/17.
//

#include "UpdateNoteHandler.h"
#include <rapidjson/document.h>

using namespace std;
using namespace MyWeb;
using namespace rapidjson;

void updateNotePost(Response& response, Request& request){
    Session session;
    Database *db = Database::getInstance();
    Document d;
    d.SetObject();
    try{
        if (request.cookies.find("token") == request.cookies.end() ||
            request.cookies.find("UID") == request.cookies.end()){
            throw IncorrectDataException();
        }

        SessionInfo *info = session.GetSessionInfo(request.cookies["token"]);
        if (info == nullptr || info->UID != atoi(request.cookies["UID"].c_str())) {
            throw IncorrectSessionException();
        }

        if(request.body.find("nid") == request.body.end() ||
                request.body.find("title") == request.body.end() ||
                request.body.find("content") == request.body.end()){
            throw IncorrectDataException();
        }

        int nid = atoi(request.body["nid"].c_str());

        if(!db->checkNoteOwner(nid,info->UID)){
            throw IncorrectDataException();
        }

        db->updateNote(nid,request.body["title"],request.body["content"],0);

        d.AddMember("status",true,d.GetAllocator());
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

    }catch(exception const& e){
        d.AddMember("status", false, d.GetAllocator());
        Value message;
        message.SetString(e.what(),d.GetAllocator());
        d.AddMember("message", message, d.GetAllocator());
    }
    string res;
    Json2String(d, res);
    response.type = "application/json";
    response.body << res;
}

UpdateNoteHandler::UpdateNoteHandler(MyWeb::Server<MyWeb::HTTP> &server) {
    server.resource["^/update/?$"]["POST"] = updateNotePost;
}

UpdateNoteHandler::~UpdateNoteHandler() {

}
