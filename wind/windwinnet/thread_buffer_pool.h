#ifndef _THREAD_BUFFER_POOL_H_
#define _THREAD_BUFFER_POOL_H_
#include "simple_loop_buffer.h"
#include "net_config.h"
#include <Windows.h>
#include <stdlib.h>



class ThreadBufferPool
{
public:
	ThreadBufferPool()
	{
		m_tlsIndex = TlsAlloc();
	}

	bool CreateThreadBuffer()
	{
		LPVOID pBuf = (LPVOID)LocalAlloc(LPTR, sizeof(CSimpleLoopBuffer));
		if (!TlsSetValue(m_tlsIndex, pBuf))
		{
			LocalFree((HLOCAL)pBuf);
			return false;
		}
		CSimpleLoopBuffer * pLoopBuf = new (pBuf)CSimpleLoopBuffer();
		pLoopBuf->Init(g_nLoopSize);
		return true;
	}


	void ReleaseThreadBuffer()
	{
		if (CSimpleLoopBuffer * pBuf = (CSimpleLoopBuffer*)TlsGetValue(m_tlsIndex))
		{
			pBuf->~CSimpleLoopBuffer();
			LocalFree((HLOCAL)pBuf);
		}
	}

	CSimpleLoopBuffer * PushData(const char * pData, int nLen)
	{
		if (CSimpleLoopBuffer* pLoopBuf = (CSimpleLoopBuffer*)TlsGetValue(m_tlsIndex))
		{
			int32 nResult = pLoopBuf->PushBack(pData, nLen);
			if (nResult != nLen)
			{
				LOG << "Loopbuffer is busy!ThreadID:" << GetCurrentThreadId() << ", Len :" << nLen << ", Error :" << nResult << std::endl;
				do
				{
					Sleep(1);
				} while (pLoopBuf->PushBack(pData, nLen) != nLen);
			}
			return pLoopBuf;
		}
		return 0;
	}
private:

	DWORD  m_tlsIndex;
};
#endif
