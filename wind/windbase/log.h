#pragma once
#include "singleton.h"
#include <string>
#include "base_define.h"
#include <sstream>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <mutex>

using namespace wind;
using namespace std;

class CLogMessage;
class CLogDispatchCallback;
class CDefaultLogDispatchCallback;
class CLogger;

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

typedef string PreRollOutCallback(const char* szOld);

class CStorage : public TSingleton<CStorage>
{
public:
	void Init(uint32 nMaxSize, string strPath, string strPrefix);
	CLogDispatchCallback * GetCallBack() { return m_pLogDispatchCallBack; }
	CLogger * GetLogger() { return m_pLogger; }
private:
	CLogDispatchCallback * m_pLogDispatchCallBack;
	CLogger *              m_pLogger;
};

class CTypedConfigurations
{
public:
	CTypedConfigurations();
	CTypedConfigurations(bool bToFile,bool bToStand, uint32 nMaxSize):
		m_bToFile(bToFile),m_bToStand(bToStand),m_nMaxLogSize(nMaxSize)
	{}
	bool GetToFile() const { return m_bToFile; }

	bool GetToStand() const { return m_bToStand; }

	string GetFileName() const { return m_strFileName; }

	uint32 GetMaxSize() const { return m_nMaxLogSize; }

	void SetMaxSize(uint32 nMaxSize) { m_nMaxLogSize = nMaxSize; }

	void   SetFileName(string strFileName) { m_strFileName = strFileName; }

	bool   GetRollback() { return m_bRollback; }

	void SetPath(string strPath) { m_strPath = strPath; }

	string GetPath() { return m_strPath; }

	void SetPrefix(string strPrefix) { m_strPrefix = strPrefix; }

	string GetPrefix() { return m_strPrefix; }
private:
	string m_strPath;
	string m_strPrefix;
	uint32 m_nMaxLogSize;
	bool   m_bToFile;
	bool   m_bToStand;
	bool   m_bRollback;
	string m_strFileName;
};

class CLogBuilder
{
public:
	virtual string Build(const CLogMessage * pLogMsg) = 0;
};

class CDefaultLogBuilder : public CLogBuilder
{
public:
	string Build(const CLogMessage * pLogMsg);
private:
	string GetTime(const char* szFmt);
	string Level2Str(ELevel eLv);
};

//每个CLogger对应一个配置文件 
//不同的配置文件有不同的 LogBuilder
class CLogger
{
public:
	//typedef unordered_map<string, fstream*> LogStreamMap;
	CLogger(CLogBuilder * pLogBuilder, PreRollOutCallback* pRollOut);

	CLogBuilder * GetLogBuilder() const { return m_pLogBuilder; }

	void SetConfigurations(CTypedConfigurations & oConfiguration) { m_oTypedConf = oConfiguration; }

	void SetRollOutFunc(PreRollOutCallback* pRollOut) { m_pRollOut = pRollOut; }

	void RollbackFileName();

	CTypedConfigurations&  GetConf() { return m_oTypedConf; }

	fstream * GetFstream() const { return m_pStream; }
private:
	fstream * GenFstream(string strPath);
	bool CreateFile(string strPath);
	string GetNewFile(string strOld);
	void RollFile(string strNewFile);

	CTypedConfigurations   m_oTypedConf;
	CLogBuilder*           m_pLogBuilder;
	fstream  *              m_pStream;
	PreRollOutCallback*     m_pRollOut;
};

class CLogMessage
{
public:
	CLogMessage(ELevel eLevel, const char *szFile, const uint32 nLine, const char* szFunc, CLogger * pLogger) :
		m_nLevel(eLevel), m_szFile(szFile), m_nLine(nLine), m_szFunc(szFunc), m_pLogger(pLogger)
	{}

	CLogger * GetLogger() const { return m_pLogger; }

	ELevel GetLevel() const { return m_nLevel; }

	const char* GetFileName() const 
	{
		int nPos = -1;
		for (int32 i = 0; m_szFile[i] != '\0'; i++)
		{
			if (m_szFile[i] == '\\' || m_szFile[i] == '/')
			{
				nPos = i;
			}
		}
		return m_szFile + nPos + 1; 
	}

	const uint32 GetLine() const { return m_nLine; }

	const char* GetFunc() const { return m_szFunc; }

	string GetMsg() const { return m_oSS.str(); }

	stringstream & GetSStream() { return m_oSS; }
private:
	ELevel m_nLevel;
	const char* m_szFile;
	const uint32 m_nLine;
	const char* m_szFunc;
	CLogger * m_pLogger;
	stringstream m_oSS;
};

class CLogDispatchData 
{
public:
	CLogDispatchData(CLogMessage * pMsg) : m_pMsg(pMsg) {}

	inline const CLogMessage * GetMsg() const { return m_pMsg; }
private:
	CLogMessage * m_pMsg;
};

class CLogDispatcher : public TSingleton<CLogDispatcher>
{
public:
	void Dispatch(CLogMessage * pMsg);
};

typedef std::recursive_mutex Mutex;
class CLogDispatchCallback
{
public:
	virtual void Handle(const CLogDispatchData* pData) = 0;
	Mutex & GetMutex() { return m_Lock; }
private:
	Mutex		m_Lock;
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
	CWriter(ELevel eLevel, const char *szFile, const uint32 nLine, const char* szFunc)
	{
		m_pLogger = CStorage::Instance()->GetLogger();
		m_pMsg = new CLogMessage(eLevel, szFile, nLine, szFunc, CStorage::Instance()->GetLogger());
	}

	~CWriter() { ProcessMsg(); }

	CWriter & Construct() { return *this; }

	template<class T>
	inline CWriter& operator<<(const T & tLog)
	{
		if (m_pMsg != nullptr)
		{
			m_pMsg->GetSStream() << tLog;
		}
		return *this;
	}

private:
	void ProcessMsg();

	const CLogger * m_pLogger;
	CLogMessage * m_pMsg;
};
#define WRITE_LOG(writer, level)\
writer(level,__FILE__,__LINE__,__FUNCSIG__).Construct()

#define CTRACE(writer) WRITE_LOG(writer, ELevel::Info)
#define CDEBUG(writer) WRITE_LOG(writer, ELevel::Debug)
#define CWARNING(writer) WRITE_LOG(writer, ELevel::Warning)
#define CERROR(writer) WRITE_LOG(writer, ELevel::Error)
#define CFATAL(writer) WRITE_LOG(writer, ELevel::Fatal)
#define CINFO(writer) WRITE_LOG(writer, ELevel::Trace)

#define LOG(LEVEL) C##LEVEL(CWriter)

//#define EXLOG_DEBUG(LEVLE) CLOG(LEVEL)

#define EXLOG_TRACE	LOG(TRACE)
#define EXLOG_DEBUG	LOG(DEBUG)
#define EXLOG_WARN	LOG(WARNING)
#define EXLOG_ERROR	LOG(ERROR)
#define EXLOG_FATAL	LOG(FATAL)
#define EXLOG_INFO	LOG(INFO)