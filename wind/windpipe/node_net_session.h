#pragma once
#include "wnet.h"
#include "wnet_opt.h"
#include "wnet_proto.h"
#include "noncopy.h"

class CNodePipe;
class CNodeNetSession : public wind::INetSession, public wind::CNonCopy
{
	CNodeNetSession();
	virtual ~CNodeNetSession() {};

	void		SetNodePipe(CNodePipe * pNodePipe) { m_pNodePipe = pNodePipe; }
	CNodePipe * GetNodePipe() { return m_pNodePipe; }
	
	void		SetConnection(wind::IConnection * pConnection) { m_pConnection = pConnection; }
	wind::IConnection *	GetConnection() { return m_pConnection; }

	void		OnRecv(const char * pBuf, const uint32 nLen) override;
	void		OnEstablish() override;
	void		OnTerminate() override;
	void		OnError(int32 nModuleErr, int32 nSysErr) override;

	bool		Send(const char * pBuf, const uint32 nLen) override;
	void		Ping();
	bool		CheckZombie();
	void		Close();

private:
	void		_OnRecvConnReq(wind::SPkgConnReq * pReq);
	void		_OnRecvConnAck(wind::SPkgConnAck * pReq);
	void		_OnRecvPing();
	void		_OnRecvData(const char *pBuf, const uint32 nLen);

private:
	CNodePipe *				m_pNodePipe;
	wind::IConnection *		m_pConnection;
	uint32					m_nLastRecvPingTm;
	wind::NET_SESSION_TYPE	m_nNetSessionType;
};
