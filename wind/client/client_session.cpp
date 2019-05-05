#include "client_session.h"
#include "log.h"
#include <thread>
#include <chrono>

using namespace std;

void WAPI CClientSession::SetConnection(IConnection* poConnection)
{
	m_pConnection = poConnection;
}

void WAPI CClientSession::OnEstablish()
{
}

void WAPI CClientSession::OnTerminate()
{

}

void WAPI CClientSession::OnRecv(const char* pBuf, uint32 dwLen)
{
	EXLOG_DEBUG << "client session recv successful!!! msg : " << pBuf;
	this_thread::sleep_for(chrono::seconds(3));
	if (m_pConnection != nullptr)
	{
		m_pConnection->Send(pBuf, dwLen);
	}
}