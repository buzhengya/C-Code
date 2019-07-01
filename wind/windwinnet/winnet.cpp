#include "winnet.h"
#include "event_mgr.h"
#include "connectctrl.h"
#include "iocpctrl.h"

IConnector * WAPI CNetWin::CreateConnector(uint32 dwNetIOType)
{
	return nullptr;
}

wind::IListener * WAPI CNetWin::CreateListener(uint32 dwNetIOType)
{
	return nullptr;
}

bool WAPI CNetWin::Run(int32 nCount)
{
	for (auto i = 0; i < nCount; i++)
	{
		SNetEvent * pEvent = CEventMgr::Instance()->PopNetEvt();
		if (pEvent == nullptr)
		{
			return true;
		}

		switch (pEvent->eType)
		{
		case NETEVT_RCV:
			pEvent->stRecvEvt.pLoopBuf->Read(m_pBuf, pEvent->stRecvEvt.nLen);
			m_nBufSize = pEvent->stRecvEvt.nLen;
			pEvent->stRecvEvt.pConnData->oConnection.GetSession()->OnRecv(m_pBuf, m_nBufSize);
			break;
			
		default:
			break;
		}
	}
	return true;
}

bool CNetWin::Init(uint32 nSize)
{
	WSAData wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);
	CConnectCtrl::Instance()->Init();
	CIocpCtrl::Instance()->Init();
	CEventMgr::Instance()->Init();

	m_nBufSize = nSize;
	m_pBuf = new char(nSize);
	return m_pBuf != nullptr;
}

