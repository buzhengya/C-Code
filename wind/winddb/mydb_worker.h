#pragma once
#include "mysql_db.h"
#include "wthread.h"
#include "mysql.h"

using namespace wind;

const static int MAX_DBCMD_QUEUE_SIZE = 100 * 1024;

class CMyDBWorker : public IDBWorker, public IThread
{
	using TLockFreeCmdQueue = CSTLockFreeSpscQueue<IDBCommand*, MAX_DBCMD_QUEUE_SIZE>;

private:
	SConnectMysqlInfo	m_stConnectInfo;

	MYSQL*			m_oMysql;

	TLockFreeCmdQueue	m_oCmdQueue;

	//for debug
	int					m_nDebugTime = 0;
};