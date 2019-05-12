#pragma once
#include "base_define.h"
#include "wnet.h"

using namespace wind;

struct CConnData;

class CConnection : public IConnection
{
public:
	CConnection();

	~CConnection() {}

	void Init(CConnData * pConnData, INetSession * pNetSession, uint32 nParentId);

	virtual void WAPI Send(const char* szSendBuf, uint32 dwLen, bool bAtOnce = false);

	void SedId(uint32 nId) { m_nId = nId; }

	uint32 GetId() { return m_nId; }

	void SetParentId(uint32 nParentId) { m_nParentId = nParentId; }

	uint32 GetParentId() { return m_nParentId; }

	void   SetSession(INetSession * pSession) { m_pNetSession = pSession; }

	INetSession * GetSession() { return m_pNetSession; }

	void OnRecv(const char * pData, uint32 nLen);

private:
	uint32        m_nId;
	uint32        m_nParentId; // connector id
	CConnData *   m_pConnData;
	INetSession * m_pNetSession;
};