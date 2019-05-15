#pragma once
#include "wnet.h"
#include "connector.h"
#include "singleton.h"
#include "data_queue.h"
#include <thread>
#include <chrono>
#include <wheel_timer.h>
#include <map>

struct SConnReqEvt
{
	string				strIP;
	uint16				nPort;
	IPacketParser*		pPacketParser;
	INetSession*		pSession;
	uint32				nRecvBufSize;
	uint32				nSendBufSize;
};

void ConnectCtrlThread();
using namespace wind;
class CConnectCtrl : public ITimerMgr, public TSingleton<CConnectCtrl>
{
	friend class TSingleton<CConnectCtrl>;
public:
	bool Init();

	void Fini();

	bool PushConnReq(string strIp,uint16 nPort, IPacketParser *pPacketParser, INetSession* pSession, uint32 nRecvBufSize, uint32 nSendBufSize);

	void OnExecute();

	void OnTimer(uint32 nId);
private:
	CConnectCtrl() {}

	~CConnectCtrl() {}

	CConnectCtrl & operator=(CConnectCtrl &) {}

	void _ProcRequests();

	void _PorcEvents();

	SConnReqEvt * _GetConnReqEvt();

	void _PushBackReqEvt(SConnReqEvt * pReqEvt);

	void _PushConnReq(SConnReqEvt * pReqEvt);

	void _DealConnErr(SConnReqEvt *pReqEvt);

	thread* m_hThread;
	bool    m_bTerminate;
	CLockFreeQueue<SConnReqEvt*> m_queConn;
	list<SConnReqEvt*> m_listFreeConn;

	uint32			m_nGlobalReqId;
	map<uint32, SConnReqEvt*>	m_mapId2Evt;
};