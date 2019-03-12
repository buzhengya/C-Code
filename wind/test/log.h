#pragma once
#include "singleton.h"
#include <string>
#include "base_define.h"
#include <sstream>

using namespace wind;
using namespace std;

class CLogDispatchCallback;
class CDefaultLogDispatchCallback;

class CStorage : public TSingleton<CStorage>
{
public:


private:
	CLogDispatchCallback * m_pLogDispatchCallBack;
};

enum class CLevel
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


class CLogger
{
	
private:
	string m_strId;
	stringstream m_stream;

};

class CLogMessage
{

private:
	CLevel m_nLevel;
	string m_strFile;
	uint32 m_nLine;
	string m_strFunc;
	CLogger * m_pLogger;
	string m_strMsg;
};

class CLogDispatchData 
{
public:

private:
	
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