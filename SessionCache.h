#pragma once
#ifndef _SESSION_CACHE_H
#define _SESSION_CACHE_H
#include <ctime>
#include <string>
#include <unordered_map>
#include "LRUCache.hpp"

#define CAPCITY 1000

typedef std::string SessionKeyType;

struct SessionInfo
{
	int UID;
	time_t Signin;
	time_t LastVist;
};

class SessionCache
{
public:
	static SessionCache* getInstance();
	bool IsExist(SessionKeyType const& sessionKey);
	void Set(SessionKeyType const& sessionKey, SessionInfo const& sessionInfo);
	SessionInfo* Get(SessionKeyType const& sessionKey);
	void Erase(SessionKeyType const& sessionKey);
	~SessionCache();

private:
	LRUCache<SessionKeyType, SessionInfo> _lrucache;
	static SessionCache* _instance;
	SessionCache() :_lrucache(CAPCITY) {};
	SessionCache& operator=(SessionCache const&) {};
	SessionCache(SessionCache const&) {};

	
};

#endif // !_SESSION_CACHE_H