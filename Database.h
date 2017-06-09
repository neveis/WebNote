//
// Created by root on 5/24/17.
//

#ifndef TEST_DATABASE_H
#define TEST_DATABASE_H
#include <hiredis/hiredis.h>
#include <string>
#include <boost/uuid/sha1.hpp>

#include "struct.h"

using namespace std;

class Database {
public:
	static Database* getInstance();
    redisContext *db;
    ~Database();

    bool checkUserExist(string& userName);
    int getUserId(const string& userName);
	string getUserName(int uid);
    int addUser(string& userName,string& password);
    int signin(const string& userName,const string& password);

    long createNote(int uid, string &title, string &content, int did);
	bool checkNoteOwner(int nid,int uid);
    bool updateNote(int nid,string &title,string &content,int did);
	note_ptr getNote(int nid);
	int getAllNote(int uid,note_ptr** notes);
    bool deleteNote(int nid);
private:
	Database();
	Database(const Database&) {};
	Database& operator=(const Database&) {};
	string generateSalt();
	string sha1(string &str);
    void databaseInit();

	static Database* instance;


};


#endif //TEST_DATABASE_H
