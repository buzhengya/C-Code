#include "server_session.h"
#include "log.h"
#include <thread>
#include <chrono>

using namespace std;

void WAPI CServerSession::SetConnection(IConnection * poConnection)
{
	m_pConnection = poConnection;
}

void WAPI CServerSession::OnEstablish()
{
}

void WAPI CServerSession::OnTerminate()
{
}

void WAPI CServerSession::OnRecv(const char * pBuf, uint32 dwLen)
{
	EXLOG_DEBUG << "server session recv successful!!! msg : " << pBuf << " len :" << dwLen;
	memcpy(m_pBuf, pBuf, dwLen);
	m_nLen = dwLen;
	if (m_pConnection != nullptr)
	{
		m_pConnection->Send(m_pBuf, m_nLen);
	}
}
