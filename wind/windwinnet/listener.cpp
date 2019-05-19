#include "listener.h"
#include "log.h"
#include <WS2tcpip.h>
#include <WinSock2.h>
#include "conndatamgr.h"

#pragma comment(lib,"ws2_32.lib")

const uint32 POST_ACCEPTEX_COUNT = 128;

CCPListener::CCPListener(string strIp, uint32 nPort) : m_strIp(strIp), m_nPort(nPort)
{
	m_stKeyData.bListen = true;
	m_stKeyData.ptr = this;
	m_pstIoData = new SPerIoData[POST_ACCEPTEX_COUNT];
}

bool CCPListener::Start(uint32 nSendSize, uint32 nRecvSize)
{
	if (m_pSessionFactory == nullptr)
	{
		EXLOG_ERROR << "listener session factory is nullptr.";
		return false;
	}
	m_nSendSize = nSendSize;
	m_nRecvSize = nRecvSize;

	m_hListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_hListen == INVALID_SOCKET)
	{
		EXLOG_ERROR << "init listen socket failed.";
		return false;
	}

	m_stAddr.sin_family = AF_INET;
	m_stAddr.sin_port = htons(m_nPort);
	if (inet_pton(AF_INET, m_strIp.c_str(), &m_stAddr.sin_addr) != 1)
	{
		EXLOG_ERROR << "inet_pton failed. ip : " << m_strIp;
		return false;
	}

	if (::bind(m_hListen, (sockaddr*)&m_stAddr, sizeof(m_stAddr)) == SOCKET_ERROR)
	{
		EXLOG_ERROR << "bind failed. error : " << WSAGetLastError();
		return false;
	}
	
	if (listen(m_hListen, POST_ACCEPTEX_COUNT) == SOCKET_ERROR)
	{
		EXLOG_ERROR << "listen failed. error : " << WSAGetLastError();
		return false;
	}

	if (_InitAcceptex() == false)
	{
		EXLOG_ERROR << "Init Acceptex failed.";
		return false;
	}

	return CIocpCtrl::Instance()->AssociateWithIocp(m_hListen, &m_stKeyData);
}

void CCPListener::OnAccept(SPerIoData * pstPerIoData)
{
	if (pstPerIoData == nullptr)
	{
		EXLOG_ERROR << "pstPerIoData is nullptr";
		return;
	}

	//deal
	EXLOG_INFO << "accept successful!!!";
	_PrintSockAddr(pstPerIoData);

	CConnData * pConnData = CConnDataMgr::Instance()->AllocConnData(m_nRecvSize, m_nSendSize);
	if (pConnData != nullptr)
	{
		INetSession * pSession = m_pSessionFactory->CreateSession(&pConnData->oConnection);
		pConnData->oConnection.Init(pConnData, pSession, 0);
		pConnData->oSock.Init(pConnData, pstPerIoData->hSock, nullptr);
	}

	_PostAcceptex(pstPerIoData);
}

void CCPListener::SetSessionFactory(ISessionFactory * pSessionFactory)
{
	m_pSessionFactory = pSessionFactory;
}

void CCPListener::_PrintSockAddr(SPerIoData * pstPerIoData)
{
	if (pstPerIoData == nullptr)
	{
		return;
	}

	sockaddr_in * addrLocal;
	int32 nLocalLen = 0;
	sockaddr_in * addrRemote;
	int32 nRemoteLen = 0;
	m_lpfnSockAddr(&pstPerIoData->stWsaBuf, 0, sizeof(sockaddr) + 16, sizeof(sockaddr) + 16,
		(sockaddr**)&addrLocal, &nLocalLen, (sockaddr**)& addrRemote, &nRemoteLen);

	char szIp[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, addrLocal, szIp, INET_ADDRSTRLEN);
	EXLOG_INFO << "local ip : " << szIp << " port : " << ntohs(addrLocal->sin_port);

	inet_ntop(AF_INET, addrRemote, szIp, INET_ADDRSTRLEN);
	EXLOG_INFO << "remote ip : " << szIp << " port : " << ntohs(addrRemote->sin_port);
}

bool CCPListener::_InitAcceptex()
{
	DWORD dwBtyes = 0;
	GUID  guidAcceptEx = WSAID_ACCEPTEX;
	if(WSAIoctl(m_hListen, SIO_GET_EXTENSION_FUNCTION_POINTER, &guidAcceptEx, sizeof(guidAcceptEx),
		&m_lpfnAcceptex, sizeof(m_lpfnAcceptex), &dwBtyes, nullptr, nullptr) != 0)
	{
		EXLOG_ERROR << "WSAIoctl get acceptex func failed. error : " << WSAGetLastError();
		return false;
	}

	GUID guidSocketAddr = WSAID_GETACCEPTEXSOCKADDRS;
	if (WSAIoctl(m_hListen,SIO_GET_EXTENSION_FUNCTION_POINTER, &guidSocketAddr, sizeof(guidSocketAddr),
		&m_lpfnSockAddr, sizeof(m_lpfnSockAddr), &dwBtyes, nullptr, nullptr) != 0)
	{
		EXLOG_ERROR << "WSAIoctl get socket addr failed. error : " << WSAGetLastError();
		return false;
	}

	for (size_t i = 0; i < POST_ACCEPTEX_COUNT; i++)
	{
		if (_PostAcceptex(m_pstIoData + i) == false)
		{
			return false;
		}
	}
	return true;
}

bool CCPListener::_PostAcceptex(SPerIoData * pstPerIoData)
{
	if (pstPerIoData == nullptr)
	{
		EXLOG_ERROR << "pstPerIoData is nullptr.";
		return false;
	}

	SOCKET hCliSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (hCliSocket == INVALID_SOCKET)
	{
		EXLOG_ERROR << "init client socket error : " << WSAGetLastError();
		return false;
	}
	pstPerIoData->hSock = hCliSocket;
	memset(&pstPerIoData->stOverlapped, 0, sizeof(pstPerIoData->stOverlapped));

	DWORD dwBytes;
	if (m_lpfnAcceptex(m_hListen, pstPerIoData->hSock, &pstPerIoData->stWsaBuf, 0, sizeof(sockaddr_in) + 16,
		sizeof(sockaddr_in) + 16, &dwBytes, &pstPerIoData->stOverlapped) != S_OK)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			EXLOG_ERROR << "accept ex function return error." << WSAGetLastError();
			return false;
		}
	}

	return true;
}