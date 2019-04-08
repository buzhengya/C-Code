#include "connection.h"

CConnection::CConnection()
{
	m_nId = 0;
	m_nParentId = 0;
	m_pConnData = nullptr;
	m_pNetSession = nullptr;
}

void CConnection::Send(const char * szSendBuf, uint32 nLen)
{
	m_pConnData->oSock.Send(szSendBuf, nLen);
}

void CConnection::OnRecv(const char * pData, uint32 nLen)
{
	m_pNetSession->OnRecv(pData, nLen);
}
