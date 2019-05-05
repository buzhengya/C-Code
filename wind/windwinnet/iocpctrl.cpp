#include "iocpctrl.h"
#include "log.h"
#include "listener.h"

void IocpThread()
{
	CIocpCtrl::Instance()->GetThreadBuf().CreateLoopBuf();
	CIocpCtrl::Instance()->OnExecute();
	CIocpCtrl::Instance()->GetThreadBuf().ReleaseLoopBuf();
}

bool CIocpCtrl::Init()
{
	m_oThreadBuf.Init();

	m_hCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 0);
	if (m_hCompletionPort == nullptr)
	{
		EXLOG_ERROR << "CIocpCtrl Init CreateIoCompletionPort is nullptr";
		return false;
	}

	SYSTEM_INFO systemInfo;
	GetSystemInfo(&systemInfo);

	m_nNumOfWorkers = systemInfo.dwNumberOfProcessors * THREAD_PER_CPU;

	for (size_t i = 0; i < m_nNumOfWorkers; i++)
	{
		thread * t = new thread(IocpThread);
		m_vecWorkerThread.push_back(t);
	}
	return true;
}

void CIocpCtrl::Fini()
{
	DWORD  dwNumBytes = 0;
	for (size_t i = 0; i < m_nNumOfWorkers; i++)
	{
		PostQueuedCompletionStatus(m_hCompletionPort, dwNumBytes, 0, 0);
	}

	for (size_t i = 0; i < m_nNumOfWorkers; i++)
	{
		if (m_vecWorkerThread[i]->joinable())
		{
			m_vecWorkerThread[i]->join();
		}
	}
	m_oThreadBuf.Finish();
	EXLOG_INFO << "IocpCtrl finish!!!!!";
}

void CIocpCtrl::OnExecute()
{
	DWORD  dwNumBytes;
	SPerKeyData * pstKeyData;
	SPerIoData * pstIoData;
	while(true)
	{
		int32 nRet = GetQueuedCompletionStatus(m_hCompletionPort, &dwNumBytes, (ULONG_PTR*)&pstKeyData, (LPOVERLAPPED *)&pstIoData, INFINITE);

		if (pstKeyData == nullptr || pstIoData == nullptr)
		{
			EXLOG_INFO << "finish IocpCtrl.";
			return;
		}

		if (nRet == 0)
		{
			EXLOG_ERROR << "GetQueuedCompletionStatus failed. ret : " << nRet << " error : " << GetLastError();
			return;
		}

		if (pstKeyData->bListen == true)
		{
			CCPListener * pListenr = (CCPListener*)pstKeyData->ptr;
			if (pListenr == nullptr)
			{
				EXLOG_ERROR << "iocpctrl CCPListener is nullptr.";
				return;
			}
			pListenr->OnAccept(pstIoData);
		}
		else
		{
			CCPSock * pSock = (CCPSock *)pstKeyData->ptr;
			if (pSock == nullptr)
			{
				EXLOG_ERROR << "iocpctrl CCPSock is nullptr.";
				return;
			}

			switch (pstIoData->eOp)
			{
			case IOCP_SEND:
				EXLOG_DEBUG << "send msg successful.";
				break;

			case IOCP_RECV:
				pSock->OnRecv(dwNumBytes);
				break;

			default:
				break;
			}
		}
	}
}

bool CIocpCtrl::AssociateWithIocp(SOCKET hSocket, SPerKeyData * pStData)
{
	if (CreateIoCompletionPort((HANDLE)hSocket, m_hCompletionPort, (ULONG_PTR)pStData, 0) == INVALID_HANDLE_VALUE)
	{
		EXLOG_ERROR << "associate with iocp failed. error : " << GetLastError();
		return false;
	}
	return true;
}
