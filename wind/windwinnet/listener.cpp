#include "listener.h"
#include "log.h"
#include <WS2tcpip.h>
#include <WinSock2.h>

#pragma comment(lib,"ws2_32.lib")

const uint32 POST_ACCEPTEX_COUNT = 128;

CCPListener::CCPListener(string strIp, uint32 nPort) : m_strIp(strIp), m_nPort(nPort)
{
	m_stKeyData.bListen = true;
	m_stKeyData.ptr = this;
	m_pstIoData = new SPerIoData[POST_ACCEPTEX_COUNT];
}

bool CCPListener::Start()
{
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

	if (listen(m_hListen, 100) == SOCKET_ERROR)
	{
		EXLOG_ERROR << "listen failed. error : " << WSAGetLastError();
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
	closesocket(pstPerIoData->hSock);

	_PostAcceptex(pstPerIoData);
}

bool CCPListener::_InitAcceptex()
{
	DWORD dwBtyes = 0;
	GUID  guidAcceptEx = WSAID_ACCEPTEX;
	if(WSAIoctl(m_hListen, SIO_GET_EXTENSION_FUNCTION_POINTER, &guidAcceptEx, sizeof(guidAcceptEx),
		&m_lpfnAcceptex, sizeof(m_lpfnAcceptex), &dwBtyes, nullptr, nullptr) == false)
	{
		EXLOG_ERROR << "WSAIoctl failed. error : " << WSAGetLastError();
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
	if (m_lpfnAcceptex(m_hListen, hCliSocket, &pstPerIoData->stWsaBuf, 1, sizeof(sockaddr_in) + 16,
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