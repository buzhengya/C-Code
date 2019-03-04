#include "mydb_worker.h"

void CMyDBWorker::MainLoop()
{
}

bool CMyDBWorker::Connect(SConnectMysqlInfo stConnectInfo)
{
	return false;
}

bool CMyDBWorker::Connect(string strIp, uint32 nPort, string strUser, string strPasswd, string strDb)
{
	return false;
}

bool CMyDBWorker::ReconnectMysql()
{
	return false;
}

void CMyDBWorker::Close()
{
}

void CMyDBWorker::PushCommond(IDBCommand * poCommand, bool bPriority)
{
}

int STDCALL CMyDBWorker::QueryWithResult(const char * szSql, IMysqlRecordset ** poRes)
{
	return 0;
}

int STDCALL CMyDBWorker::QueryWithoutResult(const char * szSql)
{
	return 0;
}

int STDCALL CMyDBWorker::EscapeString(const char * szFrom, int nFronLen, char * szTo, int nToLen)
{
	return 0;
}

const char *STDCALL CMyDBWorker::GetLastError()
{
	return 0;
}

int STDCALL CMyDBWorker::GetLastErrorNo()
{
	return 0;
}
