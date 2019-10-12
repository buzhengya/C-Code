#pragma once
#include "event.h"

bool AddTimer(Event* ev, int64 nExpire);
bool DelTimer(Event* ev);

bool InitEventTimer();
int64 FindExpireTimer();
bool DealExpireTimer();
bool IsLeftExpireTimer();