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
        if (!request.hasCookie("token") ||
            !request.hasCookie("UID")){
            throw IncorrectDataException();
        }

        SessionInfo *info = session.GetSessionInfo(request.getCookie("token"));
        if (info == nullptr || info->UID != atoi(request.getCookie("UID").c_str())) {
            throw IncorrectSessionException();
        }

        Document data;
        String2Json(request.getRawBody(),data);
        if(!data.HasMember("title") || !data.HasMember("content")){
            throw IncorrectDataException();
        }

        string title(data["title"].GetString(),data["title"].GetStringLength());
        string content(data["content"].GetString(),data["content"].GetStringLength());

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
        Value status;
        status.SetBool(false);
        Value message;
        message.SetString(e.what(),d.GetAllocator());
        d.AddMember("status", status, d.GetAllocator());
        d.AddMember("message", message, d.GetAllocator());
    }catch(...){
        d.AddMember("status", false, d.GetAllocator());
        Value message;
        d.AddMember("message", "unknown error", d.GetAllocator());
    }

    string res;
    Json2String(d, res);
    response.type = "application/json";
    response.body << res;
}

CreateNoteHandler::CreateNoteHandler(MyWeb::Server<MyWeb::HTTP> &server) {
    server.resource["^/create/?$"]["POST"] = createNotePost;
}

CreateNoteHandler::~CreateNoteHandler() {

}
