#include "connectctrl.h"
#include <WinSock2.h>
#include <WS2tcpip.h>
#include "log.h"
#pragma comment(lib, "ws2_32.lib")

void ConnectCtrlThread()
{
	CConnectCtrl::Instance()->OnExecute();
}

bool CConnectCtrl::Init()
{
	m_bTerminate = false;
	m_hThread = new thread(ConnectCtrlThread);
}

void CConnectCtrl::Fini()
{
	m_bTerminate = true;
	m_hThread->join();
}

bool CConnectCtrl::PushConnReq(string strIp, uint16 nPort, IPacketParser *pPacketParser, INetSession* pSession, uint32 nRecvBufSize, uint32 nSendBufSize)
{
	SConnReqEvt connReqEvt;
	connReqEvt.strIP = strIp;
	connReqEvt.nPort = nPort;
	connReqEvt.pPacketParser = pPacketParser;
	connReqEvt.pSession = pSession;
	connReqEvt.nRecvBufSize = nRecvBufSize;
	connReqEvt.nSendBufSize = nSendBufSize;

	m_queConn.PushBack(connReqEvt);
	return true;
}

void CConnectCtrl::OnExecute()
{
	while (!m_bTerminate)
	{
		_ProcRequests();
		//_PorcEvents();
		this_thread::sleep_for(chrono::milliseconds(1));
	}
}

void CConnectCtrl::_ProcRequests()
{
	SConnReqEvt connReqEvt;
	m_queConn.PopFront(connReqEvt);

	SOCKET hClient = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	sockaddr_in svrAddr;

	svrAddr.sin_family = AF_INET;
	svrAddr.sin_port = htons(connReqEvt.nPort);
	inet_pton(AF_INET, connReqEvt.strIP.c_str(), &svrAddr.sin_addr);

	if (connect(hClient, (sockaddr*)&svrAddr, sizeof(svrAddr)) == SOCKET_ERROR)
	{
		EXLOG_ERROR << "connect error. error : " << WSAGetLastError() << " connect should be asynchronous to avoid timeout.";
		EXLOG_ERROR << "remember add error event.";
		return;
	}


}

void CConnectCtrl::_PorcEvents()
{
	
}