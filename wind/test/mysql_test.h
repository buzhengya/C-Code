#pragma once
#include "singleton.h"
#include <string>
#include "base_define.h"
#include "mysql.h"
#include "role.h"

using namespace std;
using namespace wind;

#define MYSQL_IP "192.168.24.74"
#define MYSQL_PORT 3306
#define MYSQL_USER "root"
#define MYSQL_PASSWD "Storm#123"
#define MYSQL_DB_NAME "db_ryz_area_wf"
#define MYSQL_CHARSET ""

#define MYSQL_TABLE_CREATE "create table if not exists `tbl_wf_test` (\
`seq_id` bigint(10) not null auto_increment comment '序列号',\
`id` bigint(10) not null auto_increment comment '角色id',\
`name` char(32) not null comment '角色名称',\
`signature` varchar(255) comment '角色签名',\
`data` blob comment '角色数据',\
`create_time` datetime not null default now() comment '创建时间',\
`update_time` timestamp not null default current_timestamp on update current_timestamp comment '更新时间',\
unique key `seq_index` (`seq_id`),\
primary key(`id`),\
	unique key `name_index` (`name`)\
		)engine = innodb default charset = 'utf8mb4'; "


enum QUERY_TYPE
{
	QUERY_NAME = 0 ,
	QUERY_SIG = 1,
};

class CMysqlTest : public TSingleton<CMysqlTest>
{
	friend class TSingleton<CMysqlTest>;

public:
	bool Init(string strIp, uint32 nPort, string strUser, string strPasswd,string strDbName, string strCharset);

	bool EscapeString(const char* szFrom, uint32 nFromLen, char * szTo, uint32 nToLen);

	const char * GetLastError();

	int GetLastErrorNo();

	int QueryWithResult(const char * szSql,MYSQL_RES ** pRes);

	int QueryWithoutResult(const char * szSql);

	void HandleResult(MYSQL_RES* pRes);

	void Test();

	void Insert(uint32 nNum);

	void Delete(uint32 nNum);

	void Query(uint32 nId);

	void Query(QUERY_TYPE eType,string strCnt);

private:

	bool Connect();

	MYSQL* m_pMysql;

	string m_strIp;
	uint32 m_nPort;
	string m_strUser;
	string m_strPasswd;
	string m_strDbName;
	string m_strCharset;
};