#pragma once
#include "singleton.h"
#include <string>
#include "base_define.h"
#include <sstream>
#include <unordered_map>
#include <iostream>
#include <fstream>

using namespace wind;
using namespace std;

class CLogMessage;
class CLogDispatchCallback;
class CDefaultLogDispatchCallback;

enum class ELevel
{
	Global = 1,
	Trace = 2,
	Debug = 4,
	Fatal = 8,
	Error = 16,
	Warning = 32,
	Verbose = 64,
	Info = 128
};

class CStorage : public TSingleton<CStorage>
{
public:
	inline CLogDispatchCallback * GetCallBack() { return m_pLogDispatchCallBack; }

private:
	CLogDispatchCallback * m_pLogDispatchCallBack;
	CLogger *              m_pLogger;
};

class CTypedConfigurations
{


private:
	string m_strLogFormat;
	string m_strFileFormat;
	uint32 m_nMaxLogSize;
};

class CLogBuilder
{
public:
	virtual string Build(CLogMessage * pLogMsg) = 0;
};

class CDefaultLogBuilder : public CLogBuilder
{
public:
	string Build(CLogMessage * pLogMsg);
};

//每个CLogger对应一个配置文件 
//不同的配置文件有不同的 LogBuilder
class CLogger
{
	//typedef unordered_map<string, fstream*> LogStreamMap;
	CLogger(CLogBuilder * pLogBuilder) : m_pLogBuilder(pLogBuilder) {}

	const CLogBuilder * GetLogBuilder() { return m_pLogBuilder; }

	void SetConfigurations(CTypedConfigurations & oConfiguration) { m_pTypedConf = m_pTypedConf; }

	
private:
	CTypedConfigurations   m_pTypedConf;
	CLogBuilder*           m_pLogBuilder;
	fstream                m_fStream;
};

class CLogMessage
{
public:
	CLogMessage(ELevel eLevel, const char *szFile, const uint32 nLine, const char* szFunc, CLogger * pLogger) :
		m_nLevel(eLevel), m_szFile(szFile), m_nLine(nLine), m_szFunc(szFunc), m_pLogger(pLogger)
	{}

	template<class T>
	CLogMessage& operator<<(T& tLog)
	{
		m_oSS << T;
	}

private:
	ELevel m_nLevel;
	const char* m_szFile;
	const uint32 m_nLine;
	const char* m_szFunc;
	const CLogger * m_pLogger;
	stringstream m_oSS;
};

class CLogDispatchData 
{
public:
	CLogDispatchData(CLogMessage * pMsg) : m_pMsg(pMsg) {}

	inline const CLogMessage * GetMsg() { return m_pMsg; }
private:
	CLogMessage * m_pMsg;
};

class CLogDispatcher : public TSingleton<CLogDispatcher>
{
public:
	void Dispatch(CLogMessage * pMsg);
};

class CLogDispatchCallback
{
public:
	virtual void Handle(const CLogDispatchData* pData) = 0;
};

class CDefaultLogDispatchCallback : public CLogDispatchCallback
{
public:
	void Handle(const CLogDispatchData * pData);
	
private:
	void Dispatch(string && strLogLine); //右值引用如 "test"这样的右值建立引用

	const CLogDispatchData * m_pData;
};

class CWriter
{
public:
	CWriter(ELevel eLevel, const char *szFile, const uint32 nLine, const char* szFunc,CLogger * pLogger)
	{
		m_pLogger = pLogger;
		m_pMsg = new CLogMessage(eLevel, szFile, nLine, szFunc, pLogger);
	}

	~CWriter() { ProcessMsg(); }

	template<class T>
	inline Writer& operator<<(T & tLog)
	{
		if (m_pMsg != nullptr)
		{
			m_pMsg << T;
		}
	}

private:
	void ProcessMsg();

	const CLogger * m_pLogger;
	CLogMessage * m_pMsg;
};