#include "node_net_session.h"

#include "assert_wrap.h"
#include "wnet_pkg.h"

#include "node_pipe.h"

void CNodeNetSession::OnRecv(const char * pBuf, const uint32 nLen)
{
	ASSERT_RETVOID_MSG(m_pNodePipe != nullptr, "node pipe is nullptr!");
	ASSERT_RETVOID_MSG(pBuf != nullptr, "buf is nullptr.");
	ASSERT_RETVOID_MSG(nLen >= sPkgNetHeadLen, "msg len is too short.");

	m_pNodePipe->OnRecv(pBuf, nLen);
}

void CNodeNetSession::OnEstablish()
{
	
}

void CNodeNetSession::OnTerminate()
{
}

void CNodeNetSession::OnError(int32 nModuleErr, int32 nSysErr)
{
}

bool CNodeNetSession::Send(const char * pBuf, const uint32 nLen)
{
	ASSERT_RETVAL_MSG(m_pConnection != nullptr, false, "connection is nullptr.");
	m_pConnection->Send(pBuf, nLen);
	return true;
}

void CNodeNetSession::Ping()
{
}

bool CNodeNetSession::CheckZombie()
{
	return false;
}

void CNodeNetSession::Close()
{
}

void CNodeNetSession::_OnRecvConnReq(wind::SPkgConnReq * pReq)
{
}

void CNodeNetSession::_OnRecvConnAck(wind::SPkgConnAck * pReq)
{
}

void CNodeNetSession::_OnRecvPing()
{
}

void CNodeNetSession::_OnRecvData(const char * pBuf, const uint32 nLen)
{
}
