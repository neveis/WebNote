//
// Created by root on 5/24/17.
//
#include "Database.h"

#include <iostream>
#include <cstdio>
#include <ctime>
#include <sstream>


Database* Database::instance = new Database();

Database * Database::getInstance()
{
    return instance;
}

Database::Database()
{
    const char *hostname = "127.0.0.1";
    int port = 6379;
    struct timeval timeout = { 1,500000 };

    db = redisConnectWithTimeout(hostname, port, timeout);
    if (db == NULL || db->err) {
        std::cout << "Redis connect error" << std::endl;
        exit(1);
    }
    //databaseInit();
}

void Database::databaseInit() {
    //运行此段代码，会出现段错误，未找到原因
    redisReply *reply;

    reply = (redisReply*)redisCommand(db,"GET user:uid");
    if(reply->type == REDIS_REPLY_NIL){
        freeReplyObject(reply);
        reply = (redisReply*)redisCommand(db,"SET user:uid 1000");
        freeReplyObject(reply);
    }else{
        freeReplyObject(reply);
    }


    reply = (redisReply*)redisCommand(db,"GET note:nid");
    if(reply->type == REDIS_REPLY_NIL){
        freeReplyObject(reply);
        reply = (redisReply*)redisCommand(db,"SET note:nid 1000");
        freeReplyObject(reply);
    }else {
        freeReplyObject(reply);
    }
}

Database::~Database() {
    redisFree(db);
}

int Database::addUser(string& userName, string& password) {
    if(checkUserExist(userName)){
        return 0;
    }

    RedisReplyCls reply;
    reply = (redisReply*) redisCommand(db,"GET user:uid");
    //if(reply->type == REDIS_REPLY_ERROR){};
    int uid = atoi(reply->str) + 1;


    cout << "Add user: " << userName << " uid: " << uid  << endl;

    //add salt
    string salt = generateSalt();
    string temp = password + salt;
    string passSha1 = sha1(temp);

    //redisReply *reply;
    redisAppendCommand(db,"HSET nametoid %s %d",userName.c_str(),uid);
    redisAppendCommand(db,"SET user:%d:username %s",uid,userName.c_str());
    redisAppendCommand(db,"SET user:%d:password %s",uid,passSha1.c_str());
    redisAppendCommand(db,"SET user:%d:salt %s",uid,salt.c_str());
    redisAppendCommand(db,"INCR user:uid");
    reply.redisGetReplyCls(db); //==REDIS_OK
    //freeReplyObject(reply);
    reply.redisGetReplyCls(db);
    //freeReplyObject(reply);
    reply.redisGetReplyCls(db);
    //freeReplyObject(reply);
    reply.redisGetReplyCls(db);
    //freeReplyObject(reply);
    reply.redisGetReplyCls(db);
    //freeReplyObject(reply);

    /*
    reply = (redisReply*) redisCommand(db,"HSET nametoid %s %d",userName.c_str(),uid);
    //if(reply->type == REDIS_REPLY_ERROR){}
    freeReplyObject(reply);

    reply = (redisReply*) redisCommand(db,"HSET user %d:username %s",uid,userName.c_str());
    freeReplyObject(reply);

    reply = (redisReply*) redisCommand(db,"HSET user %d:password %s",uid,password.c_str());
    freeReplyObject(reply);
    */
    return uid;
}

bool Database::checkUserExist(string& userName) {
    RedisReplyCls reply;
    reply = (redisReply *)redisCommand(db,"HGET nametoid %s",userName.c_str());
    bool result = false;

    if(reply->type!= REDIS_REPLY_NIL) {
        result = true;
    }

    return result;
}

int Database::signin(const string &userName, const string &password) {
    int uid = getUserId(userName);
    if(!uid){
        return 0;
    }


    string passSha1;
    //redisReply *reply;
    RedisReplyCls reply;
    reply = (redisReply*) redisCommand(db,"GET user:%d:password",uid);
    passSha1 = reply->str;

    string salt;
    reply = (redisReply*) redisCommand(db,"GET user:%d:salt",uid);
    salt = reply->str;

    string temp = password + salt;
    string pass = sha1(temp);

    if(pass == passSha1){
        return uid;
    }else{
        return 0;
    }

}

/*
 * get user id.
 * if user doesn't exist,return 0;
 */
int Database::getUserId(const string &userName) {
    RedisReplyCls reply;
    //redisReply *reply;
    int uid;
    reply = (redisReply*)redisCommand(db,"HGET nametoid %s",userName.c_str());
    if(reply->type == REDIS_REPLY_NIL || reply->type == REDIS_REPLY_ERROR){
        uid =  0;
    }else{
        uid = atoi(reply->str);
    }
    //freeReplyObject(reply);

    return uid;
}

string Database::getUserName(int uid) {
    RedisReplyCls reply;
    //redisReply *reply;
    reply = (redisReply*)redisCommand(db,"GET user:%d:username",uid);
    string username;
    if(reply->type == REDIS_REPLY_NIL || reply->type == REDIS_REPLY_ERROR){
        username = "";
    }else{
        username = reply->str;
    }
    //freeReplyObject(reply);

    return username;
}

long Database::createNote(int uid, string &title, string &content,int did) {
    RedisReplyCls reply;
    //redisReply * reply;
    int nid;
    reply = (redisReply*) redisCommand(db,"GET note:nid");
    if(reply->type == REDIS_REPLY_NIL || reply->type == REDIS_REPLY_ERROR){
        nid = 0;
    }else{
        nid = atoi(reply->str) + 1;
    }
    //freeReplyObject(reply);

    if(!nid){
        return nid;
    }

    time_t createTime = time(nullptr);

    redisAppendCommand(db,"SET note:%d:title %s",nid,title.c_str());
    redisAppendCommand(db,"SET note:%d:content %s",nid,content.c_str());
    redisAppendCommand(db,"SET note:%d:createTime %d",nid,createTime);
    redisAppendCommand(db,"SET note:%d:modifiedTime %d",nid,createTime);
    redisAppendCommand(db,"SET note:%d:did %d",nid,did);
    redisAppendCommand(db,"SET note:%d:uid %d",nid,uid);
    redisAppendCommand(db,"INCR note:nid");
    redisAppendCommand(db,"LPUSH user:%d:noteList %d",uid,nid);

    reply.redisGetReplyCls(db);
    //freeReplyObject(reply);
    reply.redisGetReplyCls(db);
    //freeReplyObject(reply);
    reply.redisGetReplyCls(db);
    //freeReplyObject(reply);
    reply.redisGetReplyCls(db);
    //freeReplyObject(reply);
    reply.redisGetReplyCls(db);
    //freeReplyObject(reply);
    reply.redisGetReplyCls(db);
    //freeReplyObject(reply);
    reply.redisGetReplyCls(db);
    //freeReplyObject(reply);
    reply.redisGetReplyCls(db);
    //freeReplyObject(reply);

    return nid;

}

note_ptr Database::getNote(int nid){
    RedisReplyCls reply;
    //redisReply* reply;

    redisAppendCommand(db,"GET note:%d:title",nid);
    redisAppendCommand(db,"GET note:%d:content",nid);

    note_ptr note = std::make_shared<NOTE_STRUCT>();
    note->nid = nid;

    reply.redisGetReplyCls(db);
    note->title = reply->str;
    //freeReplyObject(reply);

    reply.redisGetReplyCls(db);
    note->content = reply->str;
    //freeReplyObject(reply);

    return note;
}

int Database::getAllNote(int uid, note_ptr **notes) {
    int count;

    RedisReplyCls reply;
    reply = (redisReply*) redisCommand(db,"LRANGE user:%d:noteList 0 -1",uid);

    count = reply->elements;
    *notes = new note_ptr[count];
    for(int i=0;i<count;i++){
        (*notes)[i] = getNote(atoi(reply->element[i]->str));
    }

    //freeReplyObject(reply);

    return count;
}



bool Database::updateNote(int nid, string &title, string &content, int did) {
    RedisReplyCls reply;

    time_t modifiedTime = time(nullptr);
    redisAppendCommand(db,"set note:%d:title %s",nid,title.c_str());
    redisAppendCommand(db,"set note:%d:content %s",nid,content.c_str());
    redisAppendCommand(db,"set note:%d:modifiedTime %d",nid,modifiedTime);

    if(reply.redisGetReplyCls(db) == REDIS_ERR){
        return false;
    }
    if(reply.redisGetReplyCls(db) == REDIS_ERR){
        return false;
    }
    if(reply.redisGetReplyCls(db) == REDIS_ERR){
        return false;
    }

    return true;
}

bool Database::deleteNote(int nid) {
    RedisReplyCls reply;
    int uid;
    //check note exist
    reply = (redisReply*) redisCommand(db,"GET note:%d:uid",nid);
    if(reply->type == REDIS_REPLY_ERROR || reply->type == REDIS_REPLY_NIL){
        return false;
    }
    uid = atoi(reply->str);

    //delete note
    redisAppendCommand(db,"DEL note:%d:title",nid);
    redisAppendCommand(db,"DEL note:%d:content",nid);
    redisAppendCommand(db,"DEL note:%d:uid",nid);
    redisAppendCommand(db,"DEL note:%d:did",nid);
    redisAppendCommand(db,"DEL note:%d:createTime",nid);
    redisAppendCommand(db,"DEL note:%d:modifiedTime",nid);
    redisAppendCommand(db,"LREM user:%d:noteList 0 %d",uid,nid);

    for(int i=0;i<7;i++){
        reply.redisGetReplyCls(db);
    }

    return true;
}

string Database::generateSalt() {

    srand(time(nullptr));
    stringstream ss;
    ss << rand();
    string strTemp;
    ss >> strTemp;
    string salt = sha1(strTemp);

    return salt.substr(0,15);
}

string Database::sha1(string &str) {
    boost::uuids::detail::sha1 sha;

    sha.process_bytes(str.c_str(),str.size());
    unsigned int digest[5];
    sha.get_digest(digest);
    stringstream ss;
    for(int i =0 ;i<5;i++){
        ss << hex << digest[i];
    }

    return ss.str();
}

bool Database::checkNoteOwner(int nid, int uid) {
    RedisReplyCls reply;

    reply = (redisReply*) redisCommand(db,"GET note:%d:uid",nid);

    if(reply->type == REDIS_REPLY_NIL || reply->type == REDIS_REPLY_ERROR){
        return false;
    }

    if(atoi(reply->str) == uid){
        return true;
    }else{
        return false;
    }
}

