#pragma once
#include "base_define.h"
#include <string>

using namespace std;

#define MIN_PROC_TIME 30

class CAutoTimeMeter
{
public:
	CAutoTimeMeter(const string& strEvent, const uint32 nLimitTime = MIN_PROC_TIME);

	~CAutoTimeMeter();

private:
	void Start();
	void Stop();

	uint64 m_nStartTime;
	uint64 m_nLimitTime;
	string m_strEvent;
};