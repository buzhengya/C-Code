#include "event_mgr.h"
#include "iocpctrl.h"

void CEventMgr::Init()
{
	m_pThreadBuf = &CIocpCtrl::Instance()->GetThreadBuf();
}

bool CEventMgr::PushRecvEvt(CConnData *pConnData, char * szBuf, uint32 nSize)
{
	if (pConnData == nullptr || szBuf == nullptr || nSize == 0)
	{
		return false;
	}

	SNetEvent * pNetEvt = _GetNetEvt();
	if (pNetEvt == nullptr)
	{
		return false;
	}

	pNetEvt->eType = NETEVT_RCV;
	pNetEvt->stRecvEvt.nConnectionId = pConnData->nConnId;
	pNetEvt->stRecvEvt.pConnData = pConnData;
	pNetEvt->stRecvEvt.nLen = nSize;
	pNetEvt->stRecvEvt.pLoopBuf = m_pThreadBuf->PushData(szBuf, nSize);

	return true;
}

SNetEvent * CEventMgr::PopNetEvt()
{
	if (m_queNetEvt.Empty())
	{
		return nullptr;
	}
	SNetEvent * pNetEvt = nullptr;
	m_queNetEvt.PopFront(pNetEvt);
	_ReleaseEvt(pNetEvt);
	return pNetEvt;
}

void CEventMgr::_PushNetEvt(SNetEvent * pstNetEvt)
{
	if (pstNetEvt != nullptr)
	{
		m_queNetEvt.PushBack(pstNetEvt);
	}
}

void CEventMgr::_ReleaseEvt(SNetEvent * pstNetEvt)
{
	if (pstNetEvt != nullptr)
	{
		m_queFreeEvt.PushBack(pstNetEvt);
	}
}

SNetEvent * CEventMgr::_GetNetEvt()
{
	SNetEvent *pEvt = nullptr;
	if (m_queFreeEvt.Empty())
	{
		pEvt = new SNetEvent;
	}
	else
	{
		m_queFreeEvt.PopFront(pEvt);
	}
	return pEvt;
}
