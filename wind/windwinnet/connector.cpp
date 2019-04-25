#include "connector.h"
#include "connectctrl.h"

CConnector::CConnector()
{
	m_pPacketParser = nullptr;
	m_pSession = nullptr;
	m_nRecvBufSize = 0;
	m_nSendBufSize = 0;
	m_strIp = "";
	m_nPort = 0;
}

void WAPI CConnector::SetPacketParser(IPacketParser* poPakcetParser)
{
	m_pPacketParser = poPakcetParser;
}

void WAPI CConnector::SetSession(INetSession * poSession)
{
	m_pSession = poSession;
}

void WAPI CConnector::SetBufferSize(uint32 dwRecvBufSize, uint32 dwSendBufSize)
{
	m_nRecvBufSize = dwRecvBufSize;
	m_nSendBufSize = dwSendBufSize;
}

bool WAPI CConnector::Connect(const char* pszIP, uint16 wPort)
{
	if (pszIP == nullptr) return false;
	//if (m_pPacketParser == nullptr) return false;
	if (m_pSession == nullptr) return false;
	if (m_nRecvBufSize == 0 || m_nSendBufSize == 0) return false;
	m_strIp = pszIP;
	m_nPort = wPort;

	return ReConnect();
}

bool WAPI CConnector::ReConnect(void)
{
	return CConnectCtrl::Instance()->PushConnReq(m_strIp, m_nPort, m_pPacketParser, m_pSession, m_nRecvBufSize, m_nSendBufSize);
}
