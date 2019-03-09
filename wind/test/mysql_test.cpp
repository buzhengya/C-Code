#include "mysql_test.h"
#include "logger.h"
#include <sstream>
#include <vector>

bool CMysqlTest::Init(string strIp, uint32 nPort, string strUser, string strPasswd,string strDbName, string strCharset)
{
	m_strIp = strIp;
	m_nPort = nPort;
	m_strUser = strUser;
	m_strPasswd = strPasswd;
	m_strDbName = strDbName;
	m_strCharset = strCharset;
	return Connect();
}

bool CMysqlTest::EscapeString(const char * szFrom, uint32 nFromLen, char * szTo, uint32 nToLen)
{
	if (szFrom == nullptr || szTo == nullptr)
	{
		return false;
	}
	if (mysql_real_escape_string(m_pMysql, szTo, szFrom, nFromLen) > nToLen)
	{
		return false;
	}
	return true;
}

const char * CMysqlTest::GetLastError()
{
	return mysql_error(m_pMysql);
}

int CMysqlTest::GetLastErrorNo()
{
	return mysql_errno(m_pMysql);
}

int CMysqlTest::QueryWithResult(const char * szSql, MYSQL_RES ** pRes)
{
	if (szSql == nullptr)
	{
		return -1;
	}

	if (mysql_real_query(m_pMysql, szSql, strlen(szSql)) != 0)
	{
		LOG << GetLastError() << endl;
		return GetLastErrorNo();
	}

	*pRes = mysql_store_result(m_pMysql);
	if (*pRes == nullptr)
	{
		return -1;
	}
}

int CMysqlTest::QueryWithoutResult(const char * szSql)
{
	if (szSql == nullptr)
	{
		return -1;
	}

	if (mysql_real_query(m_pMysql, szSql, strlen(szSql)) != 0)
	{
		return GetLastErrorNo();
	}
	return 0;
}

void CMysqlTest::HandleResult(MYSQL_RES * pRes)
{
	uint32 nFieldNum = mysql_num_fields(pRes);
	uint32 nRowNum = mysql_num_rows(pRes);

	MYSQL_ROW pRow;
	for (auto i = 0; i < nRowNum; i++)
	{
		pRow = mysql_fetch_row(pRes);
		if (pRow == nullptr || pRow[0] == nullptr || pRow[1] == nullptr || pRow[2] == nullptr || pRow[3] == nullptr || pRow[4] == nullptr)
		{
			LOG << "query is null.";
			continue;
		}
		LOG << "-------------------------- row " << i + 1 << " -------------------------" << endl;
		LOG << "seq id : " << pRow[0] << " id : " << pRow[1] << " name : " << pRow[2] << " sig : " << pRow[3] << endl;
		SRole oRole;
		oRole.Data2Role(pRow[4]);
	}

	mysql_free_result(pRes);
}

void CMysqlTest::Test()
{
	if (CMysqlTest::Instance()->Init(MYSQL_IP, MYSQL_PORT, MYSQL_USER, MYSQL_PASSWD, MYSQL_DB_NAME, MYSQL_CHARSET) == false)
	{
		LOG << CMysqlTest::Instance()->GetLastError() << endl;
	}
	Insert(1);
}

void CMysqlTest::Insert(uint32 nNum)
{
	for (auto i = 0; i < nNum; i++)
	{
		SRole oRole;
		stringstream oSS;
		uint32 nDataLen;
		char *szData = oRole.Role2Data(nDataLen);
		vector<char> vecEscapeStr(nDataLen * 2 + 1, '\0');
		EscapeString(szData, nDataLen, vecEscapeStr.data(), vecEscapeStr.size());
		oSS << "insert into tbl_wf_test (id,name,signature,date) values (" << oRole.GetId() << "," << oRole.GetName() << "," 
			<< oRole.GetSig() << vecEscapeStr.data() << ")";
		LOG << oSS.str();
		int32 nRes = QueryWithoutResult(oSS.str().c_str());
		if (nRes != 0)
		{
			LOG << "insert failed. errno : " << nRes << endl;
		}
	}
}

void CMysqlTest::Delete(uint32 nNum)
{

}

void CMysqlTest::Query(uint32 nId)
{
	stringstream oSS;
	oSS << "select * from tbl_wf_test where id=" << nId << ";";
	MYSQL_RES * pRes;
	QueryWithResult(oSS.str().c_str(), &pRes);

	HandleResult(pRes);
}

void CMysqlTest::Query(QUERY_TYPE eType, string strCnt)
{
	string strType = "";
	if (eType == QUERY_SIG)
	{
		strType = "signature";
	}
	else 
	{
		strType = "name";
	}

	vector<char> vecData(strCnt.size() * 2 + 1, '\0');
	EscapeString(strCnt.c_str(), strCnt.size(), vecData.data(), vecData.size());

	stringstream oSS;
	oSS << "select * from tbl_wf_test where " << strType << " = '" << vecData.data() << "';";

	MYSQL_RES * pRes;
	QueryWithResult(oSS.str().c_str(),&pRes);

	HandleResult(pRes);
}

bool CMysqlTest::Connect()
{
	if (m_pMysql == nullptr)
	{
		m_pMysql = mysql_init(nullptr);
	}

	if (m_pMysql == nullptr)
	{
		return false;
	}

	if (!mysql_real_connect(m_pMysql, m_strIp.c_str(), m_strUser.c_str(), 
		m_strPasswd.c_str(), m_strDbName.c_str(), m_nPort, nullptr, 0))
	{
		return false;
	}
	return true;
}
