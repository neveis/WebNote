//
// Created by root on 5/31/17.
//

#include "DeleteNoteHandler.h"

using namespace std;
using namespace MyWeb;
using namespace rapidjson;

void deleteNotePost(Response& response, Request& request){
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

        Document data;
        String2Json(request.body["raw"],data);
        if(!data.HasMember("nid")){
            throw IncorrectDataException();
        }

        int nid = data["nid"].GetInt();

        if(!db->checkNoteOwner(nid,info->UID)){
            throw IncorrectDataException();
        }

        db->deleteNote(nid);
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

DeleteNoteHandler::DeleteNoteHandler(MyWeb::Server<MyWeb::HTTP> &server) {
    server.resource["^/delete/?$"]["POST"] = deleteNotePost;
}

DeleteNoteHandler::~DeleteNoteHandler() {

}
