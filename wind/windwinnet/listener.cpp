#include "listener.h"
#include "logger.h"
#include <WS2tcpip.h>
#include "iocpctrl.h"
#define POST_ACCEPTEX_COUNT     128

CListener::CListener()
{
	m_hListenSock = INVALID_SOCKET;
	m_lpfnAcceptEx = nullptr;
	m_lpfnGetAcceptExSockaddrs = nullptr;
	m_stPerHandleData.bListen = true;
	m_stPerHandleData.ptr = this;
	m_bStart = false;
}

CListener::~CListener()
{
	if (m_hListenSock != INVALID_SOCKET)
	{
		closesocket(m_hListenSock);
	}
	if (m_pPerIoDataArray != nullptr)
	{
		delete m_pPerIoDataArray;
	}
}

bool CListener::Start(const char* pszIP, uint16 wPort)
{
	if (pszIP == nullptr)
	{
		return false;
	}
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		return false;
	}
	m_hListenSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	sockaddr_in addrListen;
	addrListen.sin_family = AF_INET;
	addrListen.sin_port = htons(wPort);
	inet_pton(AF_INET, pszIP, &addrListen.sin_addr);
	if (bind(m_hListenSock, (sockaddr*)&addrListen, sizeof(sockaddr_in)) == SOCKET_ERROR)
	{
		LOG << "CListener::Start, bind error : " << WSAGetLastError() << std::endl;
		return false;
	}

	if (listen(m_hListenSock, POST_ACCEPTEX_COUNT) == SOCKET_ERROR)
	{
		LOG << "CListener::Start, listen error : " << WSAGetLastError() << std::endl;
		return false;
	}

	if (CIocpCtrl::Instance()->AssociateWithIocp(m_hListenSock, &m_stPerHandleData) == false)
	{
		LOG << "Listener::Start, AssociateWithIocp failed!" << std::endl;
		return false;
	}

	if(InitAcceptEx() == false)
	{
		LOG << "Listener::Start, InitAcceptEx failed!" << std::endl;
		return false;
	}

	return true;
}

bool CListener::InitAcceptEx()
{
	int nRet = 0;
	DWORD dwBytes = 0;

	m_lpfnAcceptEx = nullptr;
	GUID GuidAcceptEx = WSAID_ACCEPTEX;
	nRet = WSAIoctl(m_hListenSock, SIO_GET_EXTENSION_FUNCTION_POINTER, &GuidAcceptEx,
		sizeof(GuidAcceptEx), &m_lpfnAcceptEx, sizeof(m_lpfnAcceptEx), &dwBytes, nullptr, nullptr);
	if (nRet == SOCKET_ERROR)
	{
		LOG << "WSAIoctl AcceptEx failed!" << std::endl;
		return false;
	}

	m_lpfnGetAcceptExSockaddrs = nullptr;
	GUID GuidGetAcceptExSockaddrs = WSAID_GETACCEPTEXSOCKADDRS;
	nRet = WSAIoctl(m_hListenSock, SIO_GET_EXTENSION_FUNCTION_POINTER, &GuidGetAcceptExSockaddrs, sizeof(GuidGetAcceptExSockaddrs),
		&m_lpfnGetAcceptExSockaddrs, sizeof(m_lpfnGetAcceptExSockaddrs), &dwBytes, nullptr, nullptr);
	if (nRet == SOCKET_ERROR)
	{
		LOG << "WSAIoctl GetAcceptExSockaddr failed!" << std::endl;
		return false;
	}

	m_pPerIoDataArray = new SPerIoData[POST_ACCEPTEX_COUNT];
	if (m_pPerIoDataArray == nullptr)
	{
		LOG << "m_pPerIoDataArray = new SPerIoData[POST_ACCEPTEX_COUNT] failed!" << std::endl;
		return false;
	}
	for (int i = 0; i < POST_ACCEPTEX_COUNT; i++)
	{
		if (PostAcceptEx(&m_pPerIoDataArray[i]) == false)
		{
			LOG << i << " PostAcceptEx error!" << std::endl;
			return false;
		}
	}
	return true;
}

bool CListener::PostAcceptEx(SPerIoData* pstPerIoData)
{
	SOCKET hCliSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (hCliSock == INVALID_SOCKET)
	{
		LOG << "hCliSock is invalid!!!" << std::endl;
		return false;
	}
	memset(&pstPerIoData->stOverlapped, 0, sizeof(pstPerIoData->stOverlapped));
	pstPerIoData->hSock = hCliSock;
	pstPerIoData->nOp = IOCP_ACCEPT;

	DWORD dwBytes;
	bool bSucc = m_lpfnAcceptEx(m_hListenSock, hCliSock, pstPerIoData->Buf, 0, 
		sizeof(sockaddr_in) + 16, sizeof(sockaddr_in) + 16, &dwBytes, &pstPerIoData->stOverlapped);
	if (bSucc == false)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			LOG << "CListener::PostAcceptEx, m_lpfnAcceptEx failed!!!" << std::endl;
			closesocket(hCliSock);
			return false;
		}
	}
	return true;
}

void CListener::Stop()
{
	
}


void CListener::GetSockAddress(SPerIoData * pstPerIoData, sockaddr_in & RemoteAddr, sockaddr_in & LocalAddr)
{
	
}

void CListener::GetLocalSockAddress(sockaddr_in & LocalAddr)
{
	
}


void CListener::OnAccept(SPerIoData* pstPerIoData)
{
	LOG << "void CListener::OnAccept(bool bSucc, SPerIoData* pstPerIoData)" << std::endl;
	if (pstPerIoData == nullptr)
	{
		LOG << "CListener::OnAccept(SPerIoData* pstPerIoData), pstPerIoData is nullptr!" << std::endl;
		return;
	}
	CSock* poSock = new CSock;
	poSock->SetSock(pstPerIoData->hSock);
	poSock->SetConnect(true);
	if (poSock->AssociateWithIocp() == false)
	{
		LOG << "CListener::OnAccept, AssociateWithIocp failed!" << std::endl;
		poSock->Close();
	}
	if (poSock->PostRecv() == false)
	{
		LOG << "CListener::OnAccept, PostRecv failed!" << std::endl;
		poSock->Close();
	}
}