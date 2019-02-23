#pragma once
#include "base_define.h"
#include <string>

#if defined(WIN32) || defined(WIN64)
#ifndef STDCALL
#define STDCALL __stdcall
#endif
#else
#define STDCALL
#endif

namespace wind
{

#define MAX_IP_LEN       16
#define MAX_NAME_LEN     32
#define MAX_PASSWORD_LEN 32
#define MAX_DBNAME_LEN   32
#define MAX_CHARSET_LEN  16
#define MAX_CREATEFILE_LEN  256

#define MAX_CONNECT_POOL_SIZE 100

	//db缺省hash值
	static const uint64	EDB_HASH_DEFAULT = 0xFFFFFFFFFFFFFFFF;

	// mysql connection information
	struct SConnectMysqlInfo
	{
		int				nGroup = 0;						//connection group
		int				nEnable = 0;
		std::string		strIp;
		unsigned short  wPort = 0;
		std::string		strUser;
		std::string		strPassword;
		std::string		strDBName;
		std::string		strCharset;
		std::string		strCreateFile;
	};

	// redis fonnection information
	struct SRedisConnectInfo
	{
		int			 nGroup = 0;		//	connection group
		int			 nEnable = 0;		//  是否启用
		unsigned int nDBIndex = 0;      //  节点编号索引，从0开始
		std::string  strIp;				//  REDIS节点主机IP地址
		unsigned int nPort = 0;         //  redis服务端口
		std::string  strPasswd;			//  redis认证密码
		unsigned int nPoolSize = 0;     //  此节点上的连接池大小
		unsigned int nTimeout = 0;      //  连接超时时间 秒
		unsigned int nRole = 0;         //  节点角色
	};

	// error code
	enum EExecuteResult
	{
		MYSQL_EXECUTE_UNKNOWN = -5,

		// connect error
		MYSQL_CONNECT_ERROR = -4,

		// wrong index
		MYSQL_EXECUTE_NO_INDEX = -3,

		// parameter error
		MYSQL_EXECUTE_PARA_ERROR = -2,

		// mysql execute fail
		MYSQL_EXECUTE_RESULT_FAIL = -1,

		// success
		MYSQL_EXECUTE_RESULT_SUCC = 0,

		// without result
		MYSQL_EXECUTE_RESULT_WITHOUT_RES = 1,

		// with result
		MYSQL_EXECUTE_RESULT_WITH_RES = 2,
	};


	// record
	class IMysqlRecordset
	{
	public:
		virtual STDCALL					~IMysqlRecordset() {}

		// row number and field number
		virtual uint32 STDCALL			GetRowsNum()const = 0;
		virtual uint32 STDCALL			GetFieldsNum()const = 0;
		// next array
		virtual bool STDCALL			Next() = 0;
		// index of get const char*
		virtual const char* STDCALL		GetFieldOfIndex(unsigned int dwIndex)const = 0;
		// length of get const char*
		virtual unsigned long STDCALL	GetFieldLengthOfIndex(unsigned int dwIndex)const = 0;
		// field name of const char*
		virtual const char* STDCALL		GetFieldOfName(const char *szField)const = 0;
		// release
		virtual void STDCALL			Release() = 0;
	};

	// a db worker which connect to mysql & redis, to execute mysql&redis command
	class IDBWorker
	{
	public:
		virtual STDCALL					~IDBWorker() {}

		// query return result
		virtual int STDCALL				QueryWithResult(const char* szSql, IMysqlRecordset** poRes) = 0;

		// query return no result;
		virtual int STDCALL				QueryWithoutResult(const char* szSql) = 0;

		// escape string, should (nToLen >= (2 * nFromLen + 1)), otherwise, return -1
		virtual int STDCALL				EscapeString(const char *szFrom, int nFromLen, char *szTo, int nToLen) = 0;

		// query last error
		virtual const char* STDCALL		GetLastError() = 0;
		virtual int STDCALL				GetLastErrorNo() = 0;

		//to fetch redis client 
		//virtual xRedisClient* STDCALL	GetRedisClient() { return nullptr; }
	};

	// db command which to execute mysql & redis curd 
	class IDBCommand
	{
	public:
		virtual STDCALL		~IDBCommand() {}

		// thread execute function
		virtual void STDCALL DoExecute(IDBWorker *pWorker) = 0;

		// main execute function
		virtual void STDCALL OnExecuted() = 0;

		// release object
		virtual void STDCALL Release() = 0;
	};

	// a db dispatcher who dispatch db command(mysql & redis) to different worker
	class IDBDispatcher
	{
	public:
		virtual STDCALL				~IDBDispatcher() {}
		// run the query result
		virtual bool STDCALL		Run(int nCount = -1) = 0;
		// escape data
		virtual int STDCALL			EscapeString(int nGroup, const char *szFrom, int nLen, char *szTo, int nToLen) = 0;
		// init thread pool and connect mysql, nConnectNum denote connect number
		virtual bool STDCALL		Start(int nGroup, const SConnectMysqlInfo &stConnectInfo, int nConnectNum = 1) { return false; }

		// start thread pool and connect mysql & redis
		virtual bool STDCALL		Start(const int nWorkerNum, const SConnectMysqlInfo & oMysqlConn, const SRedisConnectInfo & oRedisConn) = 0;

		//process quit 
		virtual void				ProcessQuit(unsigned int nWaitSeconds) = 0;
		// find a worker to handle command
		virtual IDBWorker* STDCALL  FindWorker(int nGroup, int nIndex) = 0;
		// add command
		virtual bool STDCALL		PostCommand(const int nGroup, const uint64 nHash, IDBCommand *poCommand, bool bHighPriority = false) = 0;

		// query directly with or without result
		virtual int STDCALL			QueryWithoutResult(const int nGroup, const uint64 nIndex, const char *szSql) = 0;
		virtual int STDCALL			QueryWithResult(const int nGroup, const uint64 nIndex, const char *szSql, IMysqlRecordset **pRes) = 0;
		// query last error
		virtual const char* STDCALL GetLastError(int nGroup, int nIndex) = 0;
		virtual int STDCALL			GetLastErrorNo(int nGroup, int nIndex) = 0;
		// release
		virtual void STDCALL		Release(unsigned int nWaitSeconds) = 0;

		virtual std::string STDCALL DebugInfo() = 0;
	};

	// get mysql module GetMysqlModule
	IDBDispatcher* STDCALL	GetDBDispatcher();
	// set logger
	//void STDCALL			SetMysqlLogger(Storm::ISTLogger *poLogger, unsigned int dwLevel);
}
