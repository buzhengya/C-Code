#include "thread_buf.h"
#include <windows.h>
#include "log.h"

CThreadBuf::CThreadBuf()
{
}

CThreadBuf::~CThreadBuf()
{
}

void CThreadBuf::Init()
{
	m_nIndex = TlsAlloc();
}

void CThreadBuf::Finish()
{
	TlsFree(m_nIndex);
}

bool CThreadBuf::CreateLoopBuf(uint32 nSize)
{
	char * pBuf = (char *)LocalAlloc(LPTR, sizeof(CLoopBuf));
	if (pBuf == nullptr)
	{
		EXLOG_ERROR << "local alloc buf is nullptr";
		return false;
	}
	CLoopBuf * pLoopBuf = new (pBuf) CLoopBuf(nSize);
	if (TlsSetValue(m_nIndex, pLoopBuf) == 0)
	{
		EXLOG_ERROR << "tls set value is failed. error : " << GetLastError();
		return false;
	}
	return true;
}

bool CThreadBuf::ReleaseLoopBuf()
{
	CLoopBuf * pLoopBuf = (CLoopBuf *)TlsGetValue(m_nIndex);
	if (pLoopBuf != nullptr)
	{
		LocalFree(pLoopBuf);
	}
	return true;
}

CLoopBuf * CThreadBuf::PushData(char * szBuf, uint32 nSize)
{
	CLoopBuf * pLoopBuf = (CLoopBuf *)TlsGetValue(m_nIndex);
	if (pLoopBuf != nullptr)
	{
		if (pLoopBuf->Write(szBuf, nSize) == false)
		{
			return nullptr;
		}
	}
	return pLoopBuf;
}

