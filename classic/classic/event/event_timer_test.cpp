#include "event_test.h"
#include "../common/time.h"
#include "../common/define.h"
#include <map>
#include <vector>

using namespace std;

map<int64, Event*> mapTimer;
vector<Event*> vecFactory;

void TestEventTimerHandle(Event* ev)
{
	// condition 1 : timer diff less 10ms
	int64 nNow = GetNowMilSec();
	ASSERT((nNow - ev->oNode.nKey) < 10);

	// condition 2 : timer match map timer
	ASSERT(ev == mapTimer.begin()->second);
	mapTimer.erase(ev->oNode.nKey);
	vecFactory.push_back(ev);
}

void TestEventTimer(int32 nCnt)
{
	
}