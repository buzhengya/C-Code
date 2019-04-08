#include "iocpctrl.h"
#include "log.h"

void IocpThread()
{
	CIocpCtrl::Instance()->OnExecute();
}

bool CIocpCtrl::Init()
{
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
}

void CIocpCtrl::Fini()
{

}

void CIocpCtrl::OnExecute()
{

}

bool CIocpCtrl::AssociateWithIocp(SOCKET hSocket, SPerHandleData * pStData)
{
	return false;
}
