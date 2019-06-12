#pragma once
#include "node_net_session.h"

#include "base_define.h"
#include "wpipe.h"
#include "noncopy.h"

//class CNodeNetSession;
//class IBizSession;

class CNodePipe : public wind::IPipe, public wind::CNonCopy
{
public:
	CNodePipe();
	CNodePipe(const CNodePipe & oPipe) = delete;
	CNodePipe & operator=(CNodePipe & oPipe) = delete;
	~CNodePipe();
	void		SetNetSession(CNodeNetSession * pNetSession) { m_pNetSession = pNetSession; }
	CNodeNetSession * GetNetSession() { return m_pNetSession; }

	void		SetRmtId(uint64 nRmtId) { m_nRmtId = nRmtId; }
	uint64		GetRmtId() { return m_nRmtId; }

	bool		SetBizSession(uint16 nBizId, wind::IBizSession * pBizSession);
	uint16		GetBizId() { return m_nBizId; }
	wind::IBizSession* GetBizSession() { return m_pBizSession; }

	bool		Send(const char * pBuf, const uint32 nLen);

	void		OnRecv(const char * pBuf, const uint32 nLen);

	void		Ping();

private:
	CNodeNetSession*	m_pNetSession;
	wind::IBizSession*	m_pBizSession;
	uint16				m_nBizId;
	uint64				m_nRmtId;
};