#pragma once
#include "wnet.h"

using namespace wind;

class CClientSession : INetSession
{
public:
	void WAPI SetConnection(IConnection* poConnection);

	void WAPI OnEstablish();

	void WAPI OnTerminate();

	void WAPI OnRecv(const char* pBuf, uint32 dwLen);

private:
	IConnection * m_pConnection;
};