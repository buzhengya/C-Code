#include "log.h"
#include "time.h"
#include <stdio.h>

CWriter::CWriter(LOG_LEVEL eLevel, const char* pszFile, const char* pszFunc, const int32 nLine)
{
	// log time
	m_oStream << GetTimeStr(nullptr) << ",";
	m_oStream << GetNowMilSec() % 1000 << " ";

	// log level
	m_oStream << _GetLevelStr(eLevel) << " ";

	// log file function line
	m_oStream << _GetFileName(pszFile) << " " << pszFunc << ":" << nLine << " ";
}

CWriter::~CWriter()
{
	std::cout << m_oStream.str() << std::endl;
}

std::string CWriter::_GetLevelStr(LOG_LEVEL eLv)
{
	switch (eLv)
	{
	case LOG_LEVEL::ERROR:
		return "[ERROR]";
		break;

	case LOG_LEVEL::WARN:
		return "[WARN]";
		break;

	case LOG_LEVEL::INFO:
		return "[INFO]";
		break;

	case LOG_LEVEL::DEBUG:
		return "[DEBUG]";
		break;

	default:
		ASSERT(false);
		break;
	}
	return "[UNKNOW]";
}

const char* CWriter::_GetFileName(const char* pszFile)
{
	int32 nPos = -1;
	for (int32 i = 0; pszFile[i] != '\0'; ++i)
	{
		if (pszFile[i] == '/' || pszFile[i] == '\\')
		{
			nPos = i;
		}
	}

	return pszFile + nPos + 1;
}
