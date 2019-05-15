#pragma once
#include "winnet.h"

using namespace wind;

class CServerSession : public INetSession
{
public:
	void WAPI SetConnection(IConnection* poConnection);

	void WAPI OnEstablish();

	void WAPI OnTerminate();

	void WAPI OnRecv(const char* pBuf, uint32 dwLen);

private:
	IConnection * m_pConnection;
};