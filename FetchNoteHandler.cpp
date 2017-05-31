//
// Created by root on 5/30/17.
//

#include "FetchNoteHandler.h"
#include <rapidjson/document.h>

using namespace std;
using namespace MyWeb;
using namespace rapidjson;

void fetchNotePost(Response& response, Request& request){
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

        if(request.body.find("nid") == request.body.end()){
            throw IncorrectDataException();
        }

        int nid = atoi(request.body["nid"].c_str());
        Value noteList(kArrayType);
        if(nid){
            note_ptr note = db->getNote(nid);
            Value noteValue(kObjectType);
            Value title;
            title.SetString(note->title.c_str(),d.GetAllocator());
            Value content;
            content.SetString(note->content.c_str(),d.GetAllocator());
            noteValue.AddMember("nid",note->nid,d.GetAllocator());
            noteValue.AddMember("title",title,d.GetAllocator());
            noteValue.AddMember("content",content,d.GetAllocator());
            noteList.PushBack(noteValue,d.GetAllocator());
        }else{
            note_ptr* notes = nullptr;
            int count = db->getAllNote(info->UID,&notes);

            for(int i = 0;i<count;i++){
                Value noteValue(kObjectType);
                Value title;
                title.SetString(notes[i]->title.c_str(),d.GetAllocator());
                Value content;
                content.SetString(notes[i]->content.c_str(),d.GetAllocator());
                noteValue.AddMember("nid",notes[i]->nid,d.GetAllocator());
                noteValue.AddMember("title",title,d.GetAllocator());
                noteValue.AddMember("content",content,d.GetAllocator());
                noteList.PushBack(noteValue,d.GetAllocator());
            }
            if(notes){
                delete[] notes;
            }
        }
        d.AddMember("status", true, d.GetAllocator());
        d.AddMember("notes",noteList,d.GetAllocator());
        string res;
        Json2String(d,res);
        response.type = "application/json";
        response.body << res;
    }catch(IncorrectSessionException const& e){
        d.AddMember("status", false, d.GetAllocator());
        Value message;
        message.SetString(e.what(),d.GetAllocator());
        d.AddMember("message", message, d.GetAllocator());
        string res;
        Json2String(d, res);

        time_t t;
        time(&t);
        t -= 60*60*24;
        response.setCookies("UID","",t);
        response.setCookies("token","",t);
        response.type = "application/json";
        response.body << res;
    }catch(exception const& e){
        d.AddMember("status", false, d.GetAllocator());
        Value message;
        message.SetString(e.what(),d.GetAllocator());
        d.AddMember("message", message, d.GetAllocator());
        string res;
        Json2String(d, res);
        response.type = "application/json";
        response.body << res;
    }
}

FetchNoteHandler::FetchNoteHandler(MyWeb::Server<MyWeb::HTTP> &server) {
    server.resource["^/fetch/?$"]["POST"] = fetchNotePost;
}

FetchNoteHandler::~FetchNoteHandler() {

}
