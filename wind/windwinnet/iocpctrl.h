#ifndef _IOCPCTRL_H_
#define _IOCPCTRL_H_
#include "base_define.h"
#include "singleton.h"
#include <WinSock2.h>
#include "sock.h"
#include "logger.h"
#include "thread_buffer_pool.h"

using namespace wind;

class CIocpCtrl : public TSingleton<CIocpCtrl>
{
	friend class TSingleton<CIocpCtrl>;
	~CIocpCtrl();

public:
	static unsigned WINAPI ThreadFunc(LPVOID pParam);

	bool Init();

	void Finish();

	void OnExecute();

	bool AssociateWithIocp(SOCKET hSock, SPerHandleData* pstData);


	ThreadBufferPool * GetThreadBufferPool()
	{
		return &m_threadBufPool;
	}

protected:
	CIocpCtrl();

	HANDLE		m_hCompletionPort;//IO完成端口的句柄
	int32		m_nNumberOfWorkers;//当前IO完成端口关联的工作线程数量
	HANDLE*		m_WorkerArray;

	ThreadBufferPool  m_threadBufPool;

	static int m_acceptCount;
};
#endif // 
