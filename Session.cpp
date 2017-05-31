#include "Session.h"
#include <cstdlib>
#include <ctime>
int Session::_seed = 0;

Session::Session()
{
	_cache = SessionCache::getInstance();
}


Session::~Session()
{
}

SessionKeyType Session::CreateSession(SessionInfo const & sessionInfo)
{
	SessionInfo info = sessionInfo;
	info.Signin = _getTime();
	info.LastVist = _getTime();

	SessionKeyType key;
	do {
		key = _GenerateSessionKey();
	} while (_cache->IsExist(key));
	
	_cache->Set(key, info);

	return key;
}

bool Session::VeritySession(SessionKeyType const & sessionKey)
{
	SessionInfo* info = _cache->Get(sessionKey);
	if (info != nullptr) {
		info->LastVist = _getTime();
		return true;
	}else{
		return false;
	}
}

SessionInfo * Session::GetSessionInfo(SessionKeyType const & sessionKey)
{
	SessionInfo *info = _cache->Get(sessionKey);
	if(info != nullptr) {
		info->LastVist = _getTime();
	}
	return info;
}

void Session::DeleteSession(SessionKeyType const & sessionKey)
{
	_cache->Erase(sessionKey);
}

//简单生成一串32个字符的随机字符串
SessionKeyType Session::_GenerateSessionKey()
{
	char key[_sessionKeyLen + 1];

	time_t t;
	time(&t);
	srand(t);
	_seed = rand();
	for (int i = 0; i < _sessionKeyLen; i++) {
		srand(_seed);
		_seed = rand();
		int u = rand();
		key[i] = _seed % 26 + (u % 2 ? 'a' : 'A');
	}
	key[_sessionKeyLen] = '\0';

	return SessionKeyType(key);
}

time_t Session::_getTime()
{
	time_t t;
	time(&t);
	return t;
}
