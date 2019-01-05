#include "wtime.h"
#include <sstream>
#include <iomanip>
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
}