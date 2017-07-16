#ifndef _SESSION_H
#define _SESSION_H

#include "SessionCache.h"


class Session
{
public:
	Session();
	~Session();

	SessionKeyType CreateSession(SessionInfo const& sessionInfo);
	bool VeritySession(SessionKeyType const& sessionKey);
	SessionInfo* GetSessionInfo(SessionKeyType const& sessionKey);
	void DeleteSession(SessionKeyType const& sessionKey);

private:
	static const int _sessionKeyLen = 32;
	static int _seed;
	SessionCache *_cache;

	SessionKeyType _GenerateSessionKey();
	time_t _getTime();

	Session(const Session&) = delete;
	Session& operator=(const Session&) = delete;
};

#endif // !_SESSION_H