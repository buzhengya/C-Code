#include "wtime.h"
#include <sstream>
#include <iomanip>
#include <sys/timeb.h>
#include <chrono>
#include <iostream>
using namespace std;

namespace wind
{
	time_t CTime::GetTimeValue()
	{
		return time(nullptr);
	}

	tm* CTime::GetTime()
	{
		time_t oTime = GetTimeValue();
		return localtime(&oTime);
	}

	std::string CTime::ToString(const char * pszFmt)
	{
		if (pszFmt == nullptr)
		{
			pszFmt = "%Y-%m-%d %H:%M:%S";
		}

		std::stringstream ss;
		auto oCurDate = GetTime();
		ss << std::put_time(oCurDate, pszFmt);
		return ss.str();
	}

	uint64 CTime::GetNowMilTime()
	{
		using namespace std::chrono;
		time_point<system_clock> oNow = system_clock::now();
		return duration_cast<milliseconds>(oNow.time_since_epoch()).count();
	}

}