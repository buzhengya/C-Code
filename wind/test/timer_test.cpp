#include "timer_test.h"
#include "wtime.h"
#include <iostream>

bool CTimerTest::Init()
{
	SetTimer(0, 0, 0, TIMER_ONCE);
	//SetTimer(1, 1 * 1000, 0 ,TIMER_CIRCLE);
	SetTimer(10, 10 * 1000, 0, TIMER_CIRCLE);
	SetTimer(30, 30 * 1000, 0, TIMER_CIRCLE);
	SetTimer(100, 100 * 1000, 0, TIMER_CIRCLE);
	SetTimer(1000, 1000 * 1000, 0, TIMER_CIRCLE);
	cout << CTime::Instance()->GetNowMilTime() ;
	return true;
}

void CTimerTest::OnTimer(uint32 nTimerId)
{
	cout << "interval : " << nTimerId << "  now : " << CTime::Instance()->GetNowMilTime() ;
}
