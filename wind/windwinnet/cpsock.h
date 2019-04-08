#pragma once
#include "wnet.h"
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>
#include "conndatamgr.h"

using namespace wind;
using namespace std;

enum EIOCP_OPERATION
{

};

struct SPerHandleData
{
	bool bListen;
	void *ptr;
};

struct SPerIoData
{
	OVERLAPPED stOverlapped;
	//SOCKET     hSock;
	EIOCP_OPERATION eOp;
	WSABUF     stWsaBuf;
};

class CCPSock
{
public:
	void Send(const char * pData, uint32 nLen);

	void OnRecv(DWORD dwBytes);

	void SetConnData(CConnData * pConnData) { m_pConnData = pConnData; }

	CConnData * GetConnData() { return m_pConnData; }

	void SetPacketParser(IPacketParser * pPacketParser) { m_pPacketParser = pPacketParser; }

	void AssociateWithIocp();

	void AttachRecvBuf(char * pRecvBuf, uint32 dwRecvBufSize);

private:
	int32 _SyncSend(const char * pData, uint32 nLen);

	int32 _AsyncSend(const char * pData, uint32 nLen);

	void  _OnError();

	bool  _PostRecv();

	SOCKET m_hSock;
	//char * m_pSendBuf;
	char * m_pRecvBuf;
	uint32 m_nRecvBufSize;
	//uint32 m_nSendBufSize;
	int32  m_nDataRecv;
	//int32  m_nDataSend;
	SPerHandleData m_stRecvIoData;

	CConnData * m_pConnData;
	IPacketParser * m_pPacketParser;
};