#pragma once
#include "base_define.h"
#include <list>
#include <vector>
#include "singleton.h"
#include <unordered_map>
#include "object_pool.h"
#include "wtime.h"

class ITimerMgr;
class CWheelTimerMgr;
class CWheelTimer;

using namespace std;
using namespace wind;

enum TIMER_TYPE
{
	TIMER_ONCE = 0,
	TIMER_CIRCLE = 1
};

typedef list<CWheelTimer*> TListTimer;

class CWheelTimer
{
public:
	CWheelTimer();

	~CWheelTimer();

	void Start(ITimerMgr * pTimerMgr, const uint32 nTimerId, const uint32 nInterval, const uint32 nDelay = 0, const TIMER_TYPE eType = TIMER_CIRCLE);

	void Stop();

	void OnTrigger(const uint64 nNow);

	friend class CWheelTimerMgr;

private:
	uint32			m_nTimerId;
	ITimerMgr *		m_pTimerMgr;
	uint32			m_nInterval;
	TIMER_TYPE		m_eType;
	uint64			m_nExpiredTime;
	uint32			m_nIndex;
	TListTimer::iterator m_itTimer;
};

class ITimerMgr
{
public:
	ITimerMgr() {}

	~ITimerMgr();

	virtual void OnTimer(uint32 nTimerId) = 0;

	bool SetTimer(const uint32 nId, const uint32 nInterval, const uint32 nDelay = 0, const TIMER_TYPE eType = TIMER_CIRCLE);

	void KillTimer(const uint32 nId);

	bool IsExistTimer(const uint32 nId) const;

private:
	unordered_map<uint32, CWheelTimer*> m_mapTimer;
};

class CWheelTimerMgr : public TSingleton<CWheelTimerMgr>
{
	friend class TSingleton<CWheelTimerMgr>;
private:
	CWheelTimerMgr();

	uint32 Cascad(uint32 nOffset, uint32 nIndex);

public:
	void AddTimer(CWheelTimer * pTimer);

	void RemoveTimer(CWheelTimer * pTimer);

	void Run();

	uint64 HandlePrecision(uint64 nTime);

	uint64 GetNowMilSec();

private:
	vector<TListTimer> m_vecListTimer;
	uint64 m_nCheckTime;
};

class CTimerFactory : public TSingleton<CTimerFactory>
{
	friend class TSingleton<CTimerFactory>;
private:
	CTimerFactory();

public:
	CWheelTimer * CreateWheelTimer();
	
	void ReleaseWheelTimer(CWheelTimer * pWheelTimer);

private:
	CObjPool<CWheelTimer> m_oWheelTimer;
};