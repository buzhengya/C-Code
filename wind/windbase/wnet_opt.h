#ifndef _WNET_OPT_H_
#define _WNET_OPT_H_
#include "base_define.h"
namespace wind
{
	const uint32 CONNECTION_OPT_SOCKOPT = 1;

	struct SConnectionOptSockopt
	{
		int32       nLevel;
		int32       nOptName;
		const char* pOptVal;
		int32       nOptLen;
	};

	const uint32 LISTENER_OPT_SESSION_FACTORY = 101;

	struct SListenerOptSessionFactory
	{
		void* pSessionFactory;
	};

	const uint32 NETWIN_OPT_MAX_CONNECTION = 301;

	struct SNetWinOptMaxConnection
	{
		int32 nMaxConnection;
	};

	const uint32 NETWIN_OPT_QUEUE_SIZE = 302;

	struct SNetWinOptQueueSize
	{
		int32 nRecvBufSize;         
		int32 nEventQueueSize;      
	};

	const uint32 NETWIN_OPT_ADVANCE_PARAM = 303;

	struct SNetWinOptAdvanceParam
	{
		int32 nParam1; 
	};

	const uint32 CONNECTOR_OPT_SOCKBIND = 401;

	struct SConnectorOptSockBind
	{
		const char*	pszIP;	
		uint16			wPort;	
	};

	const uint32 TCP_NO_DELAY = 501;

	struct STcpOptSockNoDelay
	{
		bool    bNodelay;
	};

	const uint32 IOCP_THREAD_NUM = 601;

	struct SIocpOptThreadNum
	{
		uint8    byThreadNum;
	};

	const uint32 THREAD_LOOP_SIZE = 701;

	struct SThreadLoopSize
	{
		uint32 dwLoopSize;
	};

	const uint32 SEND_STRATEGY = 801;

	struct SSendStrategy
	{
		uint32 dwSendBuffFullDelayInMilliSec;
		uint32 dwSendBuffFullRetryCountBeforeDisconnect;
	};
}
#endif
