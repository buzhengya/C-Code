#pragma once
#include "wheel_timer.h"

class CTimerTest : public ITimerMgr , public TSingleton<CTimerTest>
{
public:
	bool Init();

	void OnTimer(uint32 nTimerId);
};