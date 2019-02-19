#include "wtime.h"
#include <iostream>
#include "timer_test.h"
#include <windows.h>
using namespace wind;

int main()
{
	CTimerTest::Instance()->Init();
	while (true)
	{
		CWheelTimerMgr::Instance()->Run();
		Sleep(100);
	}
	return 0;
}