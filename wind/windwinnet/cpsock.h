#pragma once
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>
#include "wnet.h"

using namespace wind;
using namespace std;

struct CConnData;
class CConnDataMgr;

void PrintSocket(SOCKET hSock);

enum EIOCP_OPERATION
{
	IOCP_SEND = 0,
	IOCP_RECV = 1,
};

struct SPerKeyData
{
	bool bListen;
	void *ptr;
};

struct SPerIoData
{
	OVERLAPPED			stOverlapped;
	SOCKET				hSock;
	WSABUF				stWsaBuf;
	EIOCP_OPERATION		eOp;
};

struct CConnData;

class CCPSock
{
public:
	CCPSock() {}

	~CCPSock() {}

	void Init(CConnData * pConnData, SOCKET hSock, IPacketParser * pPacketParser = nullptr);

	void Send(const char * pData, uint32 nLen);

	void OnRecv(DWORD dwBytes);

	//void SetConnData(CConnData * pConnData) { m_pConnData = pConnData; }

	CConnData * GetConnData() { return m_pConnData; }

	void SetPacketParser(IPacketParser * pPacketParser) { m_pPacketParser = pPacketParser; }

	void AttachRecvBuf(char * pRecvBuf, uint32 dwRecvBufSize);

private:
	int32 _SyncSend(const char * pData, uint32 nLen);

	int32 _AsyncSend(const char * pData, uint32 nLen);

	void  _OnError();

	bool  _PostRecv();

	SOCKET m_hSock;
	char * m_pSendBuf;
	char * m_pRecvBuf;
	uint32 m_nRecvBufSize;
	uint32 m_nSendBufSize;
	int32  m_nDataRecv;
	int32  m_nDataSend;
	SPerKeyData * m_pstPerKeyData;
	SPerIoData  * m_pstRecvIoData;
	SPerIoData  * m_pstSendIoData;

	CConnData * m_pConnData;
	IPacketParser * m_pPacketParser;
};