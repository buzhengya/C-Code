#ifndef _CONNECTION_H_
#define _CONNECTION_H_

struct CConnData;

#define	 NET_SEND_BUFFER_SIZE		10

struct SNetSendBuffer
{
	SNetSendBuffer() :dwLen(0) {}
	char    cBuffer[NET_SEND_BUFFER_SIZE];
	uint32  dwLen;
};

class CConnection : public IConnection
{
	enum EConnStat
	{
		CONN_NONE = 0,
		CONN_OK,
		CONN_ASSOCIATE,
		CONN_CLOSING,
	};
public:
	CConnection();
	virtual ~CConnection();

	virtual bool WAPI IsConnected(void);

	virtual void WAPI Send(const char* pBuf, uint32 dwLen, bool bAtOnce = false);

	virtual void WAPI SetOpt(uint32 dwType, void* pOpt);

	virtual void WAPI Disconnect(void);

	virtual const uint32 WAPI GetRemoteIP(void) { return m_dwRemoteIP; }

	virtual const char* WAPI GetRemoteIPStr(void) { return inet_ntoa((in_addr&)m_dwRemoteIP); }

	virtual uint16  WAPI GetRemotePort(void) { return m_wRemotePort; }

	virtual const uint32 WAPI GetLocalIP(void) { return m_dwLocalIP; }

	virtual const char* WAPI GetLocalIPStr(void) { return inet_ntoa((in_addr&)m_dwLocalIP); }

	virtual uint16 WAPI GetLocalPort(void) { return m_wLocalPort; }

	virtual uint32 WAPI GetSendBufFree(void);

	void Reset();

	inline void SetID(uint32 dwID) { m_dwID = dwID; }

	uint32 GetID() { return m_dwID; }

	inline void SetAccept(bool bAccept) { m_bAccept = bAccept; }

	bool IsAccept() { return m_bAccept; }

	inline void SetParentID(uint32 dwParentID) { m_dwParentID = dwParentID; }
	uint32 GetParentID() { return m_dwParentID; }

	void SetRemoteIP(uint32 dwIP);

	inline void SetRemotePort(uint16 wPort) { m_wRemotePort = wPort; }

	void SetLocalIP(uint32 dwIP);

	inline void SetLocalPort(uint16 wPort) { m_wLocalPort = wPort; }

	inline void SetConnData(CConnData* pConnData) { m_pConnData = pConnData; }

	inline void SetSession(ISession* poSession) { m_poSession = poSession; }

	void OnConnect();

	void OnSend(const char * pBuf, uint32 dwLen);


	void OnAssociate();

	void OnClose();

	inline void OnRecv(const char* pData, INT32 nLen)
	{
		
	}

	inline void OnError(INT32 nSDError, INT32 nSysError)
	{
		
	}

	void UpdateSend();

	inline	uint8	GetRunIndex() { return m_byRunIndex; }
protected:
	uint32		m_dwID;
	uint32		m_dwParentID;
	bool		m_bAccept;
	uint8		m_byRunIndex;
	EConnStat	m_nConnStat;
	uint32		m_dwLocalIP;
	uint32		m_dwRemoteIP;
	uint16		m_wLocalPort;
	uint16		m_wRemotePort;
	CConnData *	m_pConnData;
	ISession* m_poSession;
	SNetSendBuffer	m_stSendBuffer;
};


#endif
