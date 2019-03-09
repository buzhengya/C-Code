#include "auto_meter.h"
#include "wtime.h"
#include "logger.h"

using namespace wind;

CAutoTimeMeter::CAutoTimeMeter(const string & strEvent, const uint32 nLimitTime)
{
	m_strEvent = strEvent;
	m_nLimitTime = nLimitTime;
	Start();
}

CAutoTimeMeter::~CAutoTimeMeter()
{
	Stop();
}

void CAutoTimeMeter::Start()
{
	m_nStartTime = CTime::Instance()->GetNowMilTime();
}

void CAutoTimeMeter::Stop()
{
	uint64 nSpend = CTime::Instance()->GetNowMilTime();

	if (nSpend>= m_nLimitTime)
	{
		LOG << "[WARN] " << m_strEvent << " spend " << nSpend << endl;
	}
	else
	{
		LOG << "[INFO]" << m_strEvent << " spend " << nSpend << endl;
	}
}
