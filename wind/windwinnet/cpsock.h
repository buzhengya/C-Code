#pragma once
#include "wnet.h"
#include <WinSock2.h>
#include <WS2tcpip.h>

using namespace wind;

struct SPerHandleData
{

};

class CCPSock
{
public:

private:
	int32 _SyncSend(const char * pData, uint32 nLen);
	int32 _AsyncSend(const char * pData, uint32 nLen);

	SOCKET m_hSock;
	char * m_pSendBuf;
	char * m_pRecvBuf;
	uint32 m_nRecvBufSize;
	uint32 m_nSendBufSize;
	int32  m_nDataRecv;
	int32  m_nDataSend;

	IPacketParser * m_pPacketParser;
};