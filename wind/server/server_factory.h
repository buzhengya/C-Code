#pragma once
#include "wnet.h"
#include "server_session.h"

using namespace wind;

class CServerFactory : public ISessionFactory
{
public:
	INetSession * WAPI CreateSession(IConnection* poConnection);
};