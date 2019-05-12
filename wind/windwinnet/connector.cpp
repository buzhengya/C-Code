#include "connector.h"
#include "connectctrl.h"
#include "log.h"

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
	if (pszIP == nullptr)
	{
		EXLOG_ERROR << "connector ip is nullptr.";
		return false;
	}
	//if (m_pPacketParser == nullptr) return false;
	if (m_pSession == nullptr)
	{
		EXLOG_ERROR << "connector session is nullptr.";
		return false;
	}
	if (m_nRecvBufSize == 0 || m_nSendBufSize == 0)
	{
		EXLOG_ERROR << "recv buf size or send buf size is 0. recv : " << m_nRecvBufSize << " send : " << m_nSendBufSize;
	}
	m_strIp = pszIP;
	m_nPort = wPort;

	return ReConnect();
}

bool WAPI CConnector::ReConnect(void)
{
	return CConnectCtrl::Instance()->PushConnReq(m_strIp, m_nPort, m_pPacketParser, m_pSession, m_nRecvBufSize, m_nSendBufSize);
}
