#pragma once
#include "wnet.h"
#include <string>

using namespace wind;
using namespace std;

class CConnector : public IConnector
{
public:
	CConnector();

	void WAPI SetPacketParser(IPacketParser* poPakcetParser);

	void WAPI SetSession(INetSession* poSession);

	void WAPI SetBufferSize(uint32 dwRecvBufSize, uint32 dwSendBufSize);

	bool WAPI Connect(const char* pszIP, uint16 wPort);

	bool WAPI ReConnect(void);

private:
	IPacketParser * m_pPacketParser;
	INetSession *   m_pSession;
	uint32 m_nSendBufSize;
	uint32 m_nRecvBufSize;
	string m_strIp;
	uint16 m_nPort;
};