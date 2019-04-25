#pragma once

#include "base_define.h"
#include "connection.h"
#include "cpsock.h"
#include "singleton.h"

using namespace std;
using namespace wind;

#define DEFAULT_RECVBUF_SIZE 65536
#define DEFAULT_SENDBUF_SIZE 65536

struct CConnData
{
	CConnData(uint32 nRecvBufSize = DEFAULT_RECVBUF_SIZE, uint32 nSendBufSize = DEFAULT_SENDBUF_SIZE);

	~CConnData();
	void Release();

	CConnection  oConnection;
	CCPSock      oSock;
	char * szSendBuf;
	char * szRecvBuf;
	uint32 nConnId;
	uint32 nSendSize;
	uint32 nRecvSize;
};

class CConnDataMgr :public TSingleton<CConnDataMgr>
{
public:
	void Init();
	CConnData * AllocConnData(uint32 nRecvSize, uint32 nSendSize);
private:
	CConnDataMgr() {}
	~CConnDataMgr() {}

	uint32 m_nCurConnId;
};