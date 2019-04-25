#include "client_session.h"

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

}