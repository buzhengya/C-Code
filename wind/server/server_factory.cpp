#include "server_factory.h"
#include "log.h"

INetSession *WAPI CServerFactory::CreateSession(IConnection * poConnection)
{
	if (poConnection == nullptr)
	{
		EXLOG_ERROR << "create session failed. connection is nullptr";
		return false;
	}

	CServerSession * pSession = new CServerSession();
	return pSession;
}
