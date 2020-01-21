#pragma once
#include "event.h"

#define EVENT_TIMER_INFINITE int64(-1)

bool AddTimer(Event* ev, int64 nDelay);
bool DelTimer(Event* ev);

bool InitEventTimer();
int64 FindExpireTimer();
bool DealExpireTimer();
bool IsNoLeftTimer();