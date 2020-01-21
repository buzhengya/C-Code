#include "event_test.h"
#include "../common/time.h"
#include "../common/define.h"
#include "../common/rand_tool.h"
#include <map>
#include <vector>
#include <chrono>
#include <thread>

using namespace std;

#define FACTORY_SIZE 1000
#define OPERATE_TIMES 1
#define MAX_DELAY  1000

map<int64, Event*> mapTimer;
vector<Event*> vecFactory;

void TestEventTimerHandle(Event* ev)
{
	// condition 1 : timer diff less 10ms
	int64 nNow = GetNowMilSec();
	ASSERT((nNow - ev->oNode.nKey) < 20);

	// condition 2 : timer match map timer
	ASSERT(ev == mapTimer.begin()->second);
	mapTimer.erase(ev->oNode.nKey);
	vecFactory.push_back(ev);
}

void TestAddTimer()
{
	int64 nDelay = int64(GetRand(0, MAX_DELAY));
	Event* ev = vecFactory[vecFactory.size() -1];
	if (AddTimer(ev, nDelay))
	{
		vecFactory.pop_back();
		mapTimer[ev->oNode.nKey] = ev;
		mapTimer.emplace(ev->oNode.nKey, ev);
	}
}

void TestDelTimer()
{
	int64 nDelay = int64(GetRand(0, MAX_DELAY));
	Event* ev = nullptr;
	if (mapTimer.find(nDelay) == mapTimer.end())
	{
		return;
	}
	ev = mapTimer[nDelay];
	mapTimer.erase(nDelay);
	vecFactory.push_back(ev);
	ASSERT(DelTimer(ev) == true);
}

void TestEventTimer(int32 nCnt)
{
	ASSERT(InitEventTimer() == true);

	// gen factory
	Event* ev = nullptr;
	for (int32 i = 0; i < FACTORY_SIZE; ++i)
	{
		ev = new(Event);
		ev->fHandle = TestEventTimerHandle;
		vecFactory.push_back(ev);
	}

	for (int32 i = 0; i < nCnt; ++i)
	{
		int64 nBegin = GetNowMilSec();
		for (int32 j = 0; j < OPERATE_TIMES && !vecFactory.empty(); ++j)
		{
			TestAddTimer();
		}

		for (int32 j = 0; j < OPERATE_TIMES && vecFactory.size() < FACTORY_SIZE; ++j)
		{
			TestDelTimer();
		}

		if (FindExpireTimer() == 0)
		{
			ASSERT(DealExpireTimer() == true);
		}
		int64 nEnd = GetNowMilSec();
		if (nEnd == nBegin)
		{
			this_thread::sleep_for(chrono::milliseconds(1));
		}
	}
}