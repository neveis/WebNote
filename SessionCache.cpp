#include "SessionCache.h"

SessionCache* SessionCache::_instance = new SessionCache();

SessionCache * SessionCache::getInstance()
{
	return _instance;
}

bool SessionCache::IsExist(SessionKeyType const & sessionKey)
{
	return _lrucache.isExist(sessionKey);
}

void SessionCache::Set(SessionKeyType const & sessionKey, SessionInfo const & sessionInfo)
{
	_lrucache.set(sessionKey, sessionInfo);

}

SessionInfo * SessionCache::Get(SessionKeyType const & sessionKey)
{
	return _lrucache.get(sessionKey);
}

void SessionCache::Erase(SessionKeyType const & sessionKey)
{
	_lrucache.erase(sessionKey);
}

SessionCache::~SessionCache()
{
}
