#ifndef _CONNECTOR_H
#define _CONNECTOR_H

#include "wnet.h"
#include <map>

using namespace wind;


//本地连接远程服务器
//class CConnector : public IConnector
//{
//	enum EConnStat
//	{
//		CONN_NONE = 0,
//		CONN_PROC,
//		CONN_OK
//	};
//
//public:
//	CConnector();
//	virtual ~CConnector();
//
//	virtual void WAPI SetPacketParser(IPacketParser* poPakcetParser);
//
//
//	virtual void WAPI SetSession(ISession* poSession);
//
//	virtual void WAPI SetBufferSize(uint32 dwRecvBufSize, uint32 dwSendBufSize);
//
//	virtual void WAPI SetOpt(uint32 dwType, void* pOpt);
//
//	virtual bool WAPI Connect(const char* pszIP, uint16 wPort);
//
//	virtual bool WAPI ReConnect(void);
//
//	virtual void WAPI Release(void);
//
//	void SetID(uint32 dwID) { m_dwID = dwID; }
//
//	uint32 GetID() { return m_dwID; }
//
//	void OnBindErr(int32 nSDError, int32 nSysError);
//
//	void OnConnect();
//
//	void OnConnectErr(int32 nSysError);
//
//	void OnClose();
//
//protected:
//	uint32				m_dwID;
//	IPacketParser*		m_poPacketParser;
//	ISession*			m_poSession;
//	uint32				m_dwRecvBufSize;
//	uint32				m_dwSendBufSize;
//	uint32				m_dwIP;
//	uint16				m_wPort;
//	EConnStat			m_nConnStat;
//	bool				m_bNeedBind;
//	const char*			m_pszBindIP;
//	uint16				m_wBindPort;
//	bool                m_bNoDelay;
//};
//
//typedef std::map<uint32, CConnector*> CMapConnector;

#endif

