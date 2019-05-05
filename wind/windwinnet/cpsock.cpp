#include "cpsock.h"
#include "log.h"
#include "iocpctrl.h"
#include "conndatamgr.h"


void CCPSock::Init(CConnData * pConnData, SOCKET hSock, IPacketParser * pPacketParser /*= nullptr*/)
{
	if (pConnData == nullptr)
	{
		return;
	}
	m_pConnData = pConnData;
	m_pSendBuf = pConnData->szSendBuf;
	m_pRecvBuf = pConnData->szRecvBuf;
	m_nSendBufSize = pConnData->nSendSize;
	m_nRecvBufSize = pConnData->nRecvSize;

	m_pstPerKeyData = new SPerKeyData();
	m_pstPerKeyData->bListen = false;
	m_pstPerKeyData->ptr = this;

	m_hSock = hSock;

	m_pPacketParser = pPacketParser;

	CIocpCtrl::Instance()->AssociateWithIocp(m_hSock, m_pstPerKeyData);

	m_nDataRecv = 0;
	m_pstRecvIoData = new SPerIoData();
	m_pstRecvIoData->eOp = IOCP_RECV;
	m_pstRecvIoData->hSock = hSock;
	memset(&m_pstRecvIoData->stOverlapped, 0, sizeof(m_pstRecvIoData->stOverlapped));
	m_pstRecvIoData->stWsaBuf.buf = m_pRecvBuf;

	_PostRecv();
}

void CCPSock::Send(const char * pData, uint32 nLen)
{
	_SyncSend(pData, nLen);
}

void CCPSock::OnRecv(DWORD dwBytes)
{
	if (m_pPacketParser != nullptr)
	{
		m_pPacketParser->ParsePacket(m_pRecvBuf, dwBytes);
	}
}

void CCPSock::AttachRecvBuf(char * pRecvBuf, uint32 dwRecvBufSize)
{
	m_pRecvBuf = pRecvBuf;
	m_nRecvBufSize = dwRecvBufSize;
}

int32 CCPSock::_SyncSend(const char * pData, uint32 nLen)
{
	WSABUF wsaBuf;
	wsaBuf.buf = (char *)pData;
	wsaBuf.len = nLen;
	DWORD dwSend;

	if (WSASend(m_hSock, &wsaBuf, 1, &dwSend, 0, nullptr, nullptr) != 0)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			_OnError();
			return -1;
		}
	}

	return dwSend;
}

int32 CCPSock::_AsyncSend(const char * pData, uint32 nLen)
{
	return 0;
}

void CCPSock::_OnError()
{
	EXLOG_ERROR << "socket error : " << WSAGetLastError();
}

bool CCPSock::_PostRecv()
{
	DWORD dwNumBytes;
	DWORD dwFlags;
	if (WSARecv(m_hSock, &m_pstRecvIoData->stWsaBuf, 1, &dwNumBytes, &dwFlags, &m_pstRecvIoData->stOverlapped, nullptr) != 0)
	{
		int32 nRet = WSAGetLastError();
		if (nRet != WSA_IO_PENDING)
		{
			EXLOG_ERROR << "wsa recv error : " << nRet;
			return false;
		}
	}
	return true;
}

void PrintSocket(SOCKET hSock)
{
	sockaddr_in addr;
	int32 nLen = sizeof(addr);
	char szIp[INET_ADDRSTRLEN];

	getpeername(hSock, (sockaddr*)&addr, &nLen);	
	inet_ntop(AF_INET, &addr, szIp, INET_ADDRSTRLEN);
	EXLOG_INFO << "remote ip : " << szIp << " port : " << ntohs(addr.sin_port);

	getsockname(hSock, (sockaddr*)&addr, &nLen);
	inet_ntop(AF_INET, &addr, szIp, INET_ADDRSTRLEN);
	EXLOG_INFO << "local ip : " << szIp << " port : " << ntohs(addr.sin_port);
}
