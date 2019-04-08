#include "cpsock.h"
#include "log.h"

void CCPSock::Send(const char * pData, uint32 nLen)
{
	_SyncSend(pData, nLen);
}

void CCPSock::OnRecv(DWORD dwBytes)
{
	
}

void CCPSock::AssociateWithIocp()
{
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

}

void CCPSock::_OnError()
{
	EXLOG_ERROR << "socket error : " << WSAGetLastError();
}

bool CCPSock::_PostRecv()
{
	WSABUF wsaBuf;

	if (WSARecv(m_hSock,&wsaBuf,1,))
}

