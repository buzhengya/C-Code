#pragma once
#include "wnet.h"
#include "connector.h"
#include "singleton.h"
#include "data_queue.h"
#include <thread>
#include <chrono>

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
class CConnectCtrl : public TSingleton<CConnectCtrl>
{
public:
	bool Init();

	void Fini();

	bool PushConnReq(string strIp,uint16 nPort, IPacketParser *pPacketParser, INetSession* pSession, uint32 nRecvBufSize, uint32 nSendBufSize);

	void OnExecute();
private:

	void _ProcRequests();

	void _PorcEvents();

	thread* m_hThread;
	bool    m_bTerminate;
	CLockFreeQueue<SConnReqEvt> m_queConn;
};