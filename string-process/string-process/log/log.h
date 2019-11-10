#pragma once
#include <sstream>
#include "../common/define.h"

enum class LOG_LEVEL
{
	ERROR = 0,
	WARN = 1,
	INFO = 2,
	DEBUG = 3,
};

class CWriter
{
public:
	//CWriter& Construct() { return *this; }
	CWriter(LOG_LEVEL eLevel, const char * pszFile, const char *pszFunc, const int32 nLine);
	~CWriter();
	
	template<class T>
	inline CWriter& operator<<(const T& tLog)
	{
		m_oStream << tLog;
		return *this;
	}

private:
	std::string _GetLevelStr(LOG_LEVEL eLv);
	const char* _GetFileName(const char* pszFile);

	std::stringstream	m_oStream;
};

#define LOGS(LEVEL) CWriter(LEVEL, __FILE__, __FUNCTION__, __LINE__)

#define LOG_ERROR LOGS(LOG_LEVEL::ERROR)
#define LOG_WARN LOGS(LOG_LEVEL::WARN)
#define LOG_INFO LOGS(LOG_LEVEL::INFO)
#define LOG_DEBUG LOGS(LOG_LEVEL::DEBUG)
