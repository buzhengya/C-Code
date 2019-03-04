#pragma once
#include "mysql_db.h"
#include "wthread.h"
#include "mysql.h"
#include "data_queue.h"

using namespace wind;

const static int MAX_DBCMD_QUEUE_SIZE = 100 * 1024;

class CMyDBWorker : public IDBWorker, public IThread
{
	using TLockFreeCmdQueue = CLockFreeSpscQueue<IDBCommand*, MAX_DBCMD_QUEUE_SIZE>;

	virtual void MainLoop() override;

	bool Connect(SConnectMysqlInfo stConnectInfo);

	bool Connect(string strIp, uint32 nPort, string strUser, string strPasswd, string strDb);

	bool ReconnectMysql();

	void Close();

	void PushCommond(IDBCommand *poCommand, bool bPriority);

	virtual int STDCALL QueryWithResult(const char* szSql, IMysqlRecordset** poRes);

	virtual int STDCALL QueryWithoutResult(const char* szSql);

	virtual int STDCALL EscapeString(const char *szFrom, int nFronLen, char * szTo, int nToLen);

	virtual const char* STDCALL GetLastError();

	virtual int STDCALL GetLastErrorNo();

private:
	SConnectMysqlInfo	m_stConnectInfo;
	MYSQL*			m_oMysql;
	TLockFreeCmdQueue	m_oCmdQueue;
};