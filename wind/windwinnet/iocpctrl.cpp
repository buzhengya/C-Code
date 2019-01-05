#include "iocpctrl.h"
#include <process.h>
#include "listener.h"
#define THREAD_PER_CPU 2

int CIocpCtrl::m_acceptCount = 0;

CIocpCtrl::CIocpCtrl() :
	m_hCompletionPort(nullptr),
	m_nNumberOfWorkers(0),
	m_WorkerArray(nullptr)
{
}

CIocpCtrl::~CIocpCtrl()
{
	Finish();
}

bool CIocpCtrl::Init()
{
	m_hCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 0);
	if (m_hCompletionPort == INVALID_HANDLE_VALUE)
	{
		LOG << "CIocpCtrl::Init(), CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 0) error." << std::endl;
		return false;
	}

	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	m_nNumberOfWorkers = sysInfo.dwNumberOfProcessors * THREAD_PER_CPU;

	m_WorkerArray = new HANDLE[m_nNumberOfWorkers];
	for (int i = 0; i < m_nNumberOfWorkers; i++)
	{
		uint32 dwThreadID = 0;
		m_WorkerArray[i] = (HANDLE)_beginthreadex(nullptr, 0, ThreadFunc, this, 0, &dwThreadID);
		if (m_WorkerArray[i] == INVALID_HANDLE_VALUE)
		{
			m_nNumberOfWorkers = i;
			LOG << "CIocpCtrl::Init(), _beginthreadex failed!" << std::endl;
			return false;
		}
	}
	return true;
}

void CIocpCtrl::Finish()
{
	if (m_hCompletionPort != INVALID_HANDLE_VALUE)
	{
		PostQueuedCompletionStatus(m_hCompletionPort, 0, 0, nullptr);
	}
}

unsigned CIocpCtrl::ThreadFunc(LPVOID pParam)
{
	CIocpCtrl* poIocpCtrl = (CIocpCtrl*)pParam;
	poIocpCtrl->OnExecute();
	return 0;
}

void CIocpCtrl::OnExecute()
{
	DWORD dwByteTransferred = 0;
	SPerHandleData* pstHandleData;
	SPerIoData* pstIoData;
	bool bRet = false;
	while(true)
	{
		bRet = GetQueuedCompletionStatus(m_hCompletionPort, &dwByteTransferred, (ULONG_PTR *)&pstHandleData, 
			(LPOVERLAPPED *)&pstIoData, INFINITE);
		if (pstHandleData == nullptr)
		{
			LOG << "exit!!!" << std::endl;
			return;
		}
		if (pstHandleData->bListen == true)
		{
			CListener* poListen = (CListener*)pstHandleData->ptr;
			poListen->OnAccept(pstIoData);
		}
		else
		{
			CSock* poSock = (CSock*)pstHandleData->ptr;
			if (poSock == nullptr)
			{
				continue;
			}
			if(bRet == false || pstIoData == nullptr)
			{
				//LOG << bRet << "  " <<pstIoData << std::endl;
				poSock->Close();
				continue;
			}
			switch (pstIoData->nOp)
			{
			case IOCP_RECV:
				poSock->OnRecv(dwByteTransferred);
				break;
			case IOCP_SEND:
				poSock->OnSend(dwByteTransferred);
				break;
			}
		}
	}
}

bool CIocpCtrl::AssociateWithIocp(SOCKET hSock, SPerHandleData* pstData)
{
	if (CreateIoCompletionPort((HANDLE)hSock, m_hCompletionPort, (DWORD)pstData, 0) == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	return true;
}