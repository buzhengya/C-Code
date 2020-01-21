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
	char		  m_pBuf[1024];
	uint32		  m_nLen;
};