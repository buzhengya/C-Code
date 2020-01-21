#include "loop_buf.h"
#include <string.h>
#include "log.h"
CLoopBuf::CLoopBuf(uint32 nSize)
{
	m_nSize = nSize;
	m_pszBuf = new char(nSize);
	m_pszStart = m_pszBuf;
	m_pszEnd = m_pszStart;
}

CLoopBuf::~CLoopBuf()
{
}

bool CLoopBuf::Read(char * pszBuf, uint32 nSize)
{
	EXLOG_DEBUG << "-----begin read------ start : " << m_pszStart - m_pszBuf << " end : " << m_pszEnd - m_pszBuf;
	uint32 nLen = (m_pszEnd + m_nSize - m_pszStart) % m_nSize;
	if (nLen < nSize)
	{
		return false;
	}
	if (m_pszStart < m_pszEnd)
	{
		memcpy(pszBuf, m_pszStart, nSize);
		m_pszStart += nSize;
	}
	else
	{
		if (m_pszBuf + m_nSize - m_pszStart >= nSize)
		{
			memcpy(pszBuf, m_pszStart, nSize);
		}
		else
		{
			uint32 nRight = m_pszBuf + m_nSize - m_pszStart;
			memcpy(pszBuf, m_pszStart, nRight);
			memcpy(pszBuf + nRight, m_pszBuf, nSize - nRight);
		}

		m_pszStart = m_pszBuf + (m_pszStart - m_pszBuf + nSize) % m_nSize;
	}

	EXLOG_DEBUG << "-----end read----- start : " << m_pszStart - m_pszBuf << " end : " << m_pszEnd - m_pszBuf;
	return true;
}

bool CLoopBuf::Write(char * pszBuf, uint32 nSize)
{
	EXLOG_DEBUG << "-----begin write------ start : " << m_pszStart - m_pszBuf << " end : " << m_pszEnd - m_pszBuf;
	uint32 nFree = (m_pszStart + m_nSize - m_pszEnd - 1) % m_nSize;
	if (nFree < nSize)
	{
		return false;
	}

	if (m_pszEnd < m_pszStart)
	{
		memcpy(m_pszEnd, pszBuf, nSize);
		m_pszEnd += nSize;
	}
	else
	{
		uint32 nRight = m_pszBuf + m_nSize - m_pszEnd;
		if (nRight >= nSize)
		{
			memcpy(m_pszEnd, pszBuf, nSize);
		}
		else
		{
			memcpy(m_pszEnd, m_pszBuf, nRight);
			memcpy(m_pszBuf, pszBuf + nRight, nSize - nRight);
		}

		m_pszEnd = m_pszBuf + (m_pszEnd - m_pszBuf + nSize) % m_nSize;
	}
	EXLOG_DEBUG << "-----end write----- start : " << m_pszStart - m_pszBuf << " end : " << m_pszEnd - m_pszBuf;
	return true;
}
