#include "sock.h"
#include "logger.h"
#include "iocpctrl.h"
#define SEND_SLEEP_TIME         40
#define SEND_RUN_TIME           40
CSock::CSock()
{
	m_stPerHandleData.bListen = false;
	m_stPerHandleData.ptr = this;
	m_stRecvIoData.nOp = IOCP_RECV;
	m_stSendIoData.nOp = IOCP_SEND;
	//m_stCloseIoData.nOp = IOCP_CLOSE;
}

CSock::~CSock()
{

}

//void CSock::Reset()
//{
//	m_hSock = INVALID_SOCKET;
//	m_bConnect = false;
//	m_pConnData = nullptr;
//}

//void CSock::AttachRecvBuf(char* pRecvBuf, uint32 dwRecvBufSize)
//{
//	
//}
//
//uint32 CSock::DetachRecvBuf(char** ppRecvBuf)
//{
//
//}
//
//void CSock::AttachSendBuf(char* pSendBuf, uint32 dwSendBufSize)
//{
//
//}
//
//uint32 CSock::DetachSendBuf(char** ppSendBuf)
//{
//
//}

//void CSock::SetConnData(CConnData* pConnData)
//{
//
//}
//
//uint32 CSock::GetConnectionID()
//{
//
//}


void CSock::Send(const char* pData, uint32 dwLen)
{
	if (IsConnect() == false)
	{
		return;
	}
	if (SyncSend(pData, dwLen) == 0)
	{
		LOG << "CSock::Send, SyncSend failed!!!" << std::endl;
	}
}

int32  CSock::SyncSend(const char * pData, uint32 dwLen)
{
	DWORD dwByteRecv = 0;
	memset(&m_stSendIoData.stOverlapped, 0, sizeof(m_stSendIoData.stOverlapped));
	memcpy(m_stSendIoData.Buf, pData, dwLen);
	m_stSendIoData.stWsaBuf.buf = (char*)m_stSendIoData.Buf;
	m_stSendIoData.stWsaBuf.len = dwLen;
	int nRet = WSASend(m_hSock, &m_stSendIoData.stWsaBuf, 1, &dwByteRecv, 0, &m_stSendIoData.stOverlapped, nullptr);
	if (nRet != 0)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			LOG << "CSock::PostSend, WSASend error : " << WSAGetLastError() << std::endl;
			return false;
		}
	}
	return dwLen;
}

//int32 CSock::AsyncSend(const char* pData, uint32 dwLen)
//{
//	return 0;
//}

//void CSock::DoClose()
//{
//	
//}

void CSock::Close()
{
	closesocket(m_hSock);
	m_bConnect = false;
	LOG << "socket : " << m_hSock << " close!!!" << std::endl;
}

bool CSock::AssociateWithIocp()
{
	return CIocpCtrl::Instance()->AssociateWithIocp(m_hSock, &m_stPerHandleData);
}

void CSock::OnRecv(DWORD dwBytes)
{
	LOG << "receive : " << m_stRecvIoData.Buf << std::endl;
	if (m_hSock == SOCKET_ERROR)
	{
		LOG << "SOCKET_ERROR!" << std::endl;
	}
	Send(m_stRecvIoData.Buf, dwBytes);
	if (PostRecv() == false)
	{
		LOG << "CSock::OnRecv, PostRecv failed!!!" << std::endl;
		Close();
	}
}

void CSock::OnSend(DWORD dwBytes)
{
	LOG << "void CSock::OnSend(DWORD dwBytes)" << std::endl;
}

//void CSock::OnClose(bool bPassive)
//{
//	LOG << "void CSock::OnClose(bool bPassive)" << std::endl;
//}
//
//void CSock::OnError(int32 nSDError, int32 nSysError)
//{
//	LOG << "void CSock::OnError(int32 nSDError, int32 nSysError)" << std::endl;
//}

bool CSock::PostRecv()
{
	DWORD dwByteRecv = 0;
	DWORD dwFlags = 0;
	memset(&m_stRecvIoData, 0, sizeof(m_stRecvIoData));
	m_stRecvIoData.stWsaBuf.buf = m_stRecvIoData.Buf;
	m_stRecvIoData.stWsaBuf.len = BUF_SIZE;

	int nRet = WSARecv(m_hSock, &m_stRecvIoData.stWsaBuf, 1, &dwByteRecv, &dwFlags, &m_stRecvIoData.stOverlapped, nullptr);
	if (nRet != 0)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			LOG << "CSock::PostRecv, WSARecv error : " << WSAGetLastError() << std::endl;
			return false;
		}
	}
	return true;
}

bool CSock::PostSend()
{
	DWORD dwByteRecv = 0;
	memset(&m_stSendIoData.stOverlapped, 0, sizeof(m_stSendIoData.stOverlapped));
	int nRet = WSASend(m_hSock, &m_stSendIoData.stWsaBuf, 1, &dwByteRecv, 0, &m_stSendIoData.stOverlapped, nullptr);
	if (nRet != 0)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			LOG << "CSock::PostSend, WSASend error : " << WSAGetLastError() << std::endl;
			return false;
		}
	}
	return true;
}

//bool CSock::PostClose(SOCKET hSock)
//{
//	
//}