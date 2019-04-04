#include "wheel_timer.h"
#include "log.h"

const uint32 TVR_BIT = 8;
const uint32 TVN_BIT = 6;
const uint32 TVR_SIZE = 1 << TVR_BIT;
const uint32 TVN_SIZE = 1 << TVN_BIT;
const uint32 TVR_MASK = TVR_SIZE - 1;
const uint32 TVN_MASK = TVN_SIZE - 1;
const uint64 TIMER_PRECISION = 100;
const uint32 INVALID_INDEX = 0xffffffff;
const uint32 MAX_U32_BIT = 0xffffffff;


//计算该层级时间轮需要偏移的距离
uint32 Offset(uint32 nLevel)
{
	return TVR_SIZE + nLevel * TVN_SIZE;
}

//计算该时间在该层级时间轮中的位置
uint32 Index(uint64 nTime, uint32 nLevel)
{
	return nTime >> (TVR_BIT + nLevel * TVN_BIT) & TVN_MASK;
}

CWheelTimer::CWheelTimer()
{
	m_nIndex = INVALID_INDEX;
	m_pTimerMgr = nullptr;
}

CWheelTimer::~CWheelTimer()
{
	Stop();
}

void CWheelTimer::Start(ITimerMgr * pTimerMgr, const uint32 nTimerId, const uint32 nInterval, const uint32 nDelay, const TIMER_TYPE eType)
{
	Stop();

	m_nTimerId = nTimerId;
	m_pTimerMgr = pTimerMgr;
	m_nInterval = CWheelTimerMgr::Instance()->HandlePrecision(nInterval);
	m_eType = eType;
	m_nExpiredTime = CWheelTimerMgr::Instance()->HandlePrecision(nDelay + CWheelTimerMgr::Instance()->GetNowMilSec());
	m_nIndex = INVALID_INDEX;
	CWheelTimerMgr::Instance()->AddTimer(this);
}

void CWheelTimer::Stop()
{
	CWheelTimerMgr::Instance()->RemoveTimer(this);
}

void CWheelTimer::OnTrigger(const uint64 nNow)
{
	if (m_pTimerMgr != nullptr)
	{
		m_pTimerMgr->OnTimer(m_nTimerId);
	}

	if (m_eType == TIMER_ONCE)
	{
		m_nIndex = INVALID_INDEX;
	}
	else if (m_eType == TIMER_CIRCLE)
	{
		m_nExpiredTime = nNow + m_nInterval;
		CWheelTimerMgr::Instance()->AddTimer(this);
	}
}

ITimerMgr::~ITimerMgr()
{
	for (const auto & it : m_mapTimer)
	{
		if (it.second != nullptr)
		{
			it.second->Stop();
			CTimerFactory::Instance()->ReleaseWheelTimer(it.second);
		}
	}
}

bool ITimerMgr::SetTimer(const uint32 nId, const uint32 nInterval, const uint32 nDelay, const TIMER_TYPE eType)
{
	if (IsExistTimer(nId))
	{
		return false;
	}

	CWheelTimer * pWheelTimer = CTimerFactory::Instance()->CreateWheelTimer();
	if (pWheelTimer == nullptr)
	{
		return false;
	}

	pWheelTimer->Start(this, nId, nInterval, nDelay, eType);
	m_mapTimer[nId] = pWheelTimer;

	return true;
}

void ITimerMgr::KillTimer(const uint32 nId)
{
	if (!IsExistTimer(nId))
	{
		return;
	}
	CWheelTimer * pWheelTimer = m_mapTimer[nId];
	if (pWheelTimer != nullptr)
	{
		pWheelTimer->Stop();
		CTimerFactory::Instance()->ReleaseWheelTimer(pWheelTimer);
	}
	m_mapTimer.erase(nId);
}

bool ITimerMgr::IsExistTimer(const uint32 nId) const
{
	if (m_mapTimer.find(nId) != m_mapTimer.end())
	{
		return true;
	}
	return false;
}

CWheelTimerMgr::CWheelTimerMgr()
{
	m_nCheckTime = HandlePrecision(GetNowMilSec());
	m_vecListTimer.resize(TVR_SIZE + TVN_SIZE * 4);
}

uint32 CWheelTimerMgr::Cascad(uint32 nOffset, uint32 nIndex)
{
	TListTimer& listTimer = m_vecListTimer[nOffset + nIndex];
	TListTimer listTmp;
	listTmp.splice(listTmp.end(), listTimer);

	for (auto & it : listTmp)
	{
		AddTimer(it);
	}
	return nIndex;
}

void CWheelTimerMgr::AddTimer(CWheelTimer * pTimer)
{
	uint64 nDiffTime = pTimer->m_nExpiredTime - m_nCheckTime;

	if (nDiffTime < 0)
	{
		pTimer->m_nIndex = m_nCheckTime & TVR_MASK;
	}
	else if (nDiffTime < TVR_SIZE)
	{
		pTimer->m_nIndex = (m_nCheckTime + nDiffTime) & TVR_MASK;
	}
	else if (nDiffTime < TVR_SIZE * TVN_SIZE)
	{
		pTimer->m_nIndex = Offset(0) + Index(m_nCheckTime + nDiffTime, 0);
	}
	else if (nDiffTime < TVR_SIZE * TVN_SIZE * TVN_SIZE)
	{
		pTimer->m_nIndex = Offset(1) + Index(m_nCheckTime + nDiffTime, 1);
	}
	else if (nDiffTime < TVR_SIZE * TVN_SIZE * TVN_SIZE * TVN_SIZE)
	{
		pTimer->m_nIndex = Offset(2) + Index(m_nCheckTime + nDiffTime, 2);
	}
	else
	{
		if (nDiffTime > MAX_U32_BIT)
		{
			nDiffTime = MAX_U32_BIT;
		}
		pTimer->m_nIndex = Offset(3) + Index(m_nCheckTime + nDiffTime, 3);
	}

	TListTimer& listTimer = m_vecListTimer[pTimer->m_nIndex];
	listTimer.push_back(pTimer);
	pTimer->m_itTimer = listTimer.end();
	--pTimer->m_itTimer;
	EXLOG_DEBUG << "add timer in index : " << pTimer->m_nIndex;
}

void CWheelTimerMgr::RemoveTimer(CWheelTimer * pTimer)
{
	if (pTimer->m_nIndex == INVALID_INDEX)
	{
		return;
	}

	TListTimer& listTimer = m_vecListTimer[pTimer->m_nIndex];
	listTimer.erase(pTimer->m_itTimer);
	pTimer->m_nIndex = INVALID_INDEX;
}

void CWheelTimerMgr::Run()
{
	uint64 nNow = HandlePrecision(GetNowMilSec());
	while (m_nCheckTime <= nNow)
	{
		uint32 nIndex = m_nCheckTime & TVR_MASK;
		if (nIndex == 0 && 
			Cascad(Offset(0),Index(m_nCheckTime,0)) &&
			Cascad(Offset(1), Index(m_nCheckTime, 1)) &&
			Cascad(Offset(2), Index(m_nCheckTime, 2)))
		{
			Cascad(Offset(3), Index(m_nCheckTime, 3));
		}

		m_nCheckTime++;

		TListTimer& listTimer = m_vecListTimer[nIndex];
		//EXLOG_DEBUG << "run index : " << nIndex << endl;
		TListTimer listTmp;
		listTmp.splice(listTmp.end(), listTimer);
		for (const auto & it :listTmp)
		{
			it->OnTrigger(m_nCheckTime);
		}
	}
}

uint64 CWheelTimerMgr::HandlePrecision(uint64 nTime)
{
	return nTime / TIMER_PRECISION;
}

uint64 CWheelTimerMgr::GetNowMilSec()
{
	return CTime::Instance()->GetNowMilTime();
}

CTimerFactory::CTimerFactory()
{
	m_oWheelTimer.Init(32, 8);
}

CWheelTimer * CTimerFactory::CreateWheelTimer()
{
	CWheelTimer * pTimer = m_oWheelTimer.FetchObj();
	if (pTimer == nullptr)
	{
		EXLOG_DEBUG << "m_oWheelTimer.FetchObj() is nullptr";
	}
	return pTimer;
}

void CTimerFactory::ReleaseWheelTimer(CWheelTimer * pWheelTimer)
{
	if (pWheelTimer != nullptr)
	{
		m_oWheelTimer.ReleaseObj(pWheelTimer);
	}
}
