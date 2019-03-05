#pragma once
#include "mysql_db.h"
#include "wthread.h"
#include "mysql.h"
#include "data_queue.h"

using namespace wind;

const static int MAX_DBCMD_QUEUE_SIZE = 100 * 1024;

class CMyDBWorker : public IDBWorker
{
	using TLockFreeCmdQueue = CLockFreeSpscQueue<IDBCommand*, MAX_DBCMD_QUEUE_SIZE>;

	void Start();

	void MainLoop();

	bool Connect(SConnectMysqlInfo stConnectInfo);

	bool Connect(string strIp, uint32 nPort, string strUser, string strPasswd, string strDb, string strCharset);

	bool ReconnectMysql();

	void Close();

	IDBCommand* PopCommond();

	void PushCommond(IDBCommand *poCommand, bool bPriority);

	virtual int STDCALL QueryWithResult(const char* szSql, IMysqlRecordset** poRes);

	virtual int STDCALL QueryWithoutResult(const char* szSql);

	//将from中的字符串编码转义 并且将结果置于to中 并添加null字节 并不是检查SQL语句 而是检查SQL语句中的字段
	virtual int STDCALL EscapeString(const char *szFrom, int nFronLen, char * szTo, int nToLen);

	virtual const char* STDCALL GetLastError();

	virtual int STDCALL GetLastErrorNo();

private:
	bool _Connect();

	SConnectMysqlInfo	m_stConnectInfo;
	MYSQL*			m_pMysql;
	TLockFreeCmdQueue	m_oCmdQueue;
};