#include "connection.h"
#include "log.h"
#include "conndatamgr.h"

CConnection::CConnection()
{
	m_nId = 0;
	m_nParentId = 0;
	m_pConnData = nullptr;
	m_pNetSession = nullptr;
}

void CConnection::Init(CConnData * pConnData, INetSession * pNetSession, uint32 nParentId)
{
	if (pConnData == nullptr || pNetSession == nullptr)
	{
		EXLOG_ERROR << "pConnData or pNetSession is nullptr.";
		return;
	}

	m_pConnData = pConnData;
	m_pNetSession = pNetSession;
	m_nParentId = nParentId;
	m_nId = m_pConnData->nConnId;
}

void CConnection::Send(const char * szSendBuf, uint32 dwLen, bool bAtOnce)
{
	if (m_pConnData == nullptr)
	{
		EXLOG_ERROR << "m_pConnData is nullptr.";
		return;
	}
	m_pConnData->oSock.Send(szSendBuf, dwLen);
}

void CConnection::OnRecv(const char * pData, uint32 nLen)
{
	m_pNetSession->OnRecv(pData, nLen);
}
