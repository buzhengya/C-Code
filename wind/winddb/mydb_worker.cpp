#include "mydb_worker.h"
#include "log.h"
#include "mysql_db.h"

void CMyDBWorker::Start()
{

}

void CMyDBWorker::MainLoop()
{
	for(;;)
	{
		IDBCommand * pCommond = PopCommond();
		if (pCommond != nullptr)
		{
			pCommond->DoExecute(this);

			//执行完的命令 由调度器处理结果

		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}

bool CMyDBWorker::Connect(SConnectMysqlInfo stConnectInfo)
{
	m_stConnectInfo = stConnectInfo;
	return _Connect();
}

bool CMyDBWorker::Connect(string strIp, uint32 nPort, string strUser, string strPasswd, string strDb, string strCharset)
{
	m_stConnectInfo.strIp = strIp;
	m_stConnectInfo.wPort = nPort;
	m_stConnectInfo.strUser = strUser;
	m_stConnectInfo.strPassword = strPasswd;
	m_stConnectInfo.strDBName = strDb;
	m_stConnectInfo.strCharset = strCharset;

	return _Connect();
}

bool CMyDBWorker::ReconnectMysql()
{
	Close();
	_Connect();
}

void CMyDBWorker::Close()
{
	if (m_pMysql != nullptr)
	{
		mysql_close(m_pMysql);
		m_pMysql = nullptr;
	}
}

IDBCommand * CMyDBWorker::PopCommond()
{
	IDBCommand * pCommond = nullptr;
	m_oCmdQueue.PopFront(pCommond);
	return pCommond;
}

void CMyDBWorker::PushCommond(IDBCommand * poCommand, bool bPriority)
{
	m_oCmdQueue.PushBack(poCommand);
}

int STDCALL CMyDBWorker::QueryWithResult(const char * szSql, IMysqlRecordset ** poRes)
{
	if (mysql_real_query(m_pMysql, szSql, strlen(szSql)) == 0)
	{
		MYSQL_RES * pMysqlRes = mysql_store_result(m_pMysql);

	}
	return 0;
}

int STDCALL CMyDBWorker::QueryWithoutResult(const char * szSql)
{
	return 0;
}

int STDCALL CMyDBWorker::EscapeString(const char * szFrom, int nFromLen, char * szTo, int nToLen)
{
	return mysql_real_escape_string(m_pMysql, szTo, szFrom, nToLen);
}

const char *STDCALL CMyDBWorker::GetLastError()
{
	return mysql_error(m_pMysql);
}

int STDCALL CMyDBWorker::GetLastErrorNo()
{
	return mysql_errno(m_pMysql);
}

bool CMyDBWorker::_Connect()
{
	if (m_pMysql == nullptr)
	{
		m_pMysql = mysql_init(nullptr);
	}

	if (mysql_options(m_pMysql, MYSQL_SET_CHARSET_NAME, m_stConnectInfo.strCharset.c_str()))
	{
		EXLOG_DEBUG << "mysql_options set charset name failed.";
		return false;
	}

	if (!mysql_real_connect(m_pMysql,
		m_stConnectInfo.strIp.c_str(),
		m_stConnectInfo.strUser.c_str(),
		m_stConnectInfo.strPassword.c_str(),
		m_stConnectInfo.strDBName.c_str(),
		m_stConnectInfo.wPort, nullptr, 0))
	{
		EXLOG_DEBUG << "mysql connect failed." << " IP : " << m_stConnectInfo.strIp << " database : "
			<< m_stConnectInfo.strIp << " password : " << m_stConnectInfo.strPassword;
		return false;
	}

	char nReconnect = 1;
	mysql_options(m_pMysql, MYSQL_OPT_RECONNECT, &nReconnect);
	return true;
}
