#ifndef _WTIME_H_
#define _WTIME_H_
#include <time.h>
#include "base_define.h"
#include "singleton.h"
#include<string>
namespace wind
{
	class CTime : public TSingleton<CTime>  //���м̳� �̳�ģ�嵥���ķ��� û��˽�г�Ա
	{
		//friend class TSingleton<CTime>;//��Ԫ��ɷ������˽�г�Ա
	public:

		time_t GetTimeValue();

		tm* GetTime();

		std::string ToString(const char * pszFmt="%Y-%m-%d %H:%M:%S");


		uint64 GetNowMilTime(); //��ȡ���뼶ʱ��
	};
}
#endif
