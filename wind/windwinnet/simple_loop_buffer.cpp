#include "simple_loop_buffer.h"
#include <stdio.h>
#include <memory.h>
CSimpleLoopBuffer::CSimpleLoopBuffer() :
	m_pBuffer(nullptr),
	m_pNextRead(nullptr),
	m_pNextWrite(nullptr),
	m_pEnd(nullptr)
{
}

CSimpleLoopBuffer::~CSimpleLoopBuffer()
{
	if (nullptr != m_pBuffer)
	{
		delete[] m_pBuffer;
		m_pBuffer = nullptr;
	}
}

bool CSimpleLoopBuffer::Init(int32 nSize)
{
	m_pBuffer = new char(nSize + 1);
	if (m_pBuffer == nullptr)
	{
		return false;
	}
	m_pEnd = m_pBuffer + nSize + 1;
	m_pNextRead = m_pBuffer;
	m_pNextWrite = m_pBuffer;
}

int32 CSimpleLoopBuffer::PushBack(const char *pData, int32 nLen)
{
	if (m_pNextWrite >= m_pNextRead)
	{
		/*                          == 内存模型 ==
		         (empty)             m_pNextRead         m_pNextWrite     (empty)
		|----------------------------------|--------------------|---------------------|
		*/
		int nLeft = m_pNextRead - m_pBuffer;
		int nRight = m_pEnd - m_pNextWrite;
		if (nLeft + nRight <= nLen)
		{
			return -1;
		}
		if (m_pNextWrite + nLen <= m_pEnd)
		{
			memcpy(m_pNextWrite, pData, nLen);
			m_pNextWrite = m_pNextWrite + nLen;
			if (m_pNextWrite == m_pEnd)
			{
				m_pNextWrite = m_pBuffer;
			}
		}
		else
		{
			memcpy(m_pNextWrite, pData, nRight);
			memcpy(m_pBuffer, pData + nRight, nLen - nRight);
			m_pNextWrite = m_pBuffer + nLen - nRight;
		}
	}
	else
	{
		/*                          == 内存模型 ==
		                             m_pNextWrite   (empty)  m_pNextRead
		|----------------------------------|--------------------|---------------------|
		*/
		if (m_pNextRead - m_pNextWrite <= nLen)
		{
			return -2;
		}
		memcpy(m_pNextWrite, pData, nLen);
		m_pNextWrite += nLen;
	}
	return nLen;
}

int32 CSimpleLoopBuffer::PopFront(int32 nLen, char* szDstData)
{
	if(m_pNextWrite >= m_pNextRead)
	{
		/*                          == 内存模型 ==
		          (empty)             m_pNextRead         m_pNextWrite     (empty)
		|----------------------------------|--------------------|---------------------|
		*/
		if (m_pNextWrite - m_pNextRead < nLen)
		{
			return -1;
		}
		memcpy(szDstData, m_pNextRead, nLen);
		m_pNextRead += nLen;
	}
	else
	{
		/*                          == 内存模型 ==
		                             m_pNextWrite   (empty)  m_pNextRead
		|----------------------------------|--------------------|---------------------|
		*/
		int nRight = m_pEnd - m_pNextRead;
		int nLeft = m_pNextWrite - m_pBuffer;
		if (nLeft + nRight < nLen)
		{
			return -2;
		}
		if (m_pNextRead + nLen <= m_pEnd)
		{
			memcpy(szDstData, m_pNextRead, nLen);
			m_pNextRead += nLen;
			if (m_pNextRead == m_pEnd)
			{
				m_pNextRead = m_pBuffer;
			}
		}
		else
		{
			memcpy(szDstData, m_pNextRead, nRight);
			memcpy(szDstData + nRight, m_pBuffer, nLen - nRight);
			m_pNextRead = m_pBuffer + nLen - nRight;
		}
	}
	return nLen;
}