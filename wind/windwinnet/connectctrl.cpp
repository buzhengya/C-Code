#include "connectctrl.h"
#include "cpsock.h"
#include <WinSock2.h>
#include <WS2tcpip.h>
#include "log.h"
#pragma comment(lib, "ws2_32.lib")
#include "conndatamgr.h"

void ConnectCtrlThread()
{
	CConnectCtrl::Instance()->OnExecute();
}

bool CConnectCtrl::Init()
{
	m_bTerminate = false;
	m_hThread = new thread(ConnectCtrlThread);

	m_nGlobalReqId = 0;
	return true;
}

void CConnectCtrl::Fini()
{
	m_bTerminate = true;
	m_hThread->join();
}

bool CConnectCtrl::PushConnReq(string strIp, uint16 nPort, IPacketParser *pPacketParser, INetSession* pSession, uint32 nRecvBufSize, uint32 nSendBufSize)
{
	SConnReqEvt * pReqEvt = _GetConnReqEvt();
	if (pReqEvt == nullptr)
	{
		EXLOG_ERROR << "can not get free conn req evt.";
		return false;
	}
	pReqEvt->strIP = strIp;
	pReqEvt->nPort = nPort;
	pReqEvt->pPacketParser = pPacketParser;
	pReqEvt->pSession = pSession;
	pReqEvt->nRecvBufSize = nRecvBufSize;
	pReqEvt->nSendBufSize = nSendBufSize;

	m_queConn.PushBack(pReqEvt);
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

void CConnectCtrl::OnTimer(uint32 nId)
{
	if (m_mapId2Evt.find(nId) == m_mapId2Evt.end())
	{
		return;
	}

	SConnReqEvt * pConnEvt = m_mapId2Evt[nId];
	m_mapId2Evt.erase(nId);
	if (pConnEvt == nullptr)
	{
		EXLOG_ERROR << "OnTimer pConnEvt is nullptr.";
		return;
	}
	EXLOG_DEBUG << "deal conn error. conn ip : " << pConnEvt->strIP << " port : " << pConnEvt->nPort;
	_PushConnReq(pConnEvt);
}

void CConnectCtrl::_ProcRequests()
{
	SConnReqEvt * pReqEvt = nullptr;
	m_queConn.PopFront(pReqEvt);
	if (pReqEvt == nullptr) return;

	SOCKET hClient = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	sockaddr_in svrAddr;

	svrAddr.sin_family = AF_INET;
	svrAddr.sin_port = htons(pReqEvt->nPort);
	inet_pton(AF_INET, pReqEvt->strIP.c_str(), &svrAddr.sin_addr);

	if (connect(hClient, (sockaddr*)&svrAddr, sizeof(svrAddr)) == SOCKET_ERROR)
	{
		EXLOG_ERROR << "connect error. error : " << WSAGetLastError() << " connect should be asynchronous to avoid timeout.";
		EXLOG_ERROR << "remember add error event.";
		_DealConnErr(pReqEvt);
		return;
	}
	_PushBackReqEvt(pReqEvt);
	
	EXLOG_DEBUG << "connect " << pReqEvt->strIP << ":" << pReqEvt->nPort << " successful!!!";
	PrintSocket(hClient);
	
	CConnData * pConnData = CConnDataMgr::Instance()->AllocConnData(pReqEvt->nRecvBufSize, pReqEvt->nSendBufSize);
	if (pConnData == nullptr)
	{
		EXLOG_ERROR << "pConnData is nullptr.";
		return;
	}

	pConnData->oConnection.Init(pConnData, pReqEvt->pSession, 0);
	pConnData->oSock.Init(pConnData, hClient, pReqEvt->pPacketParser);
}

void CConnectCtrl::_PorcEvents()
{
	
}

SConnReqEvt * CConnectCtrl::_GetConnReqEvt()
{
	if (m_listFreeConn.empty())
	{
		return new SConnReqEvt;
	}
	SConnReqEvt * pReqEvt = m_listFreeConn.front();
	m_listFreeConn.pop_front();
	return pReqEvt;
}

void CConnectCtrl::_PushBackReqEvt(SConnReqEvt * pReqEvt)
{
	if (pReqEvt == nullptr)
	{
		return;
	}

	m_listFreeConn.push_back(pReqEvt);
}

void CConnectCtrl::_PushConnReq(SConnReqEvt * pReqEvt)
{
	if (pReqEvt != nullptr)
	{
		m_queConn.PushBack(pReqEvt);
	}
}

void CConnectCtrl::_DealConnErr(SConnReqEvt * pReqEvt)
{
	if (pReqEvt == nullptr)
	{
		return;
	}

	uint32 nId = m_nGlobalReqId++;
	m_mapId2Evt[nId] = pReqEvt;

	SetTimer(nId, 0, 3 * 1000, TIMER_ONCE);
}
