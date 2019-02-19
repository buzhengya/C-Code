#ifndef _WTIME_H_
#define _WTIME_H_
#include <time.h>
#include "base_define.h"
#include "singleton.h"
#include<string>
namespace wind
{
	class CTime : public TSingleton<CTime>  //公有继承 继承模板单例的方法 没有私有成员
	{
		//friend class TSingleton<CTime>;//友元类可访问类的私有成员
	public:

		time_t GetTimeValue();

		tm* GetTime();

		std::string ToString(const char * pszFmt="%Y-%m-%d %H:%M:%S");


		uint64 GetNowMilTime(); //获取毫秒级时间
	};
}
#endif
