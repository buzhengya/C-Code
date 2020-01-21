#include "time.h"
#include <chrono>
#include <sstream>
#include <iomanip>

using namespace std::chrono;

int64 GetNowMilSec()
{
	auto tp = system_clock::now();
	auto du = tp.time_since_epoch();
	return duration_cast<milliseconds>(du).count();
}

std::string GetTimeStr(const char* pszFmt)
{
	if (pszFmt == nullptr)
	{
		pszFmt = "%Y-%m-%d %H:%M:%S";
	}

	std::stringstream ss;
	time_t oTime = time(nullptr);
	tm oCurDate;
	localtime_s(&oCurDate, &oTime);
	ss << std::put_time(&oCurDate, pszFmt);
	return ss.str();
}
