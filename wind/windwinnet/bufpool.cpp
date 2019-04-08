#include "bufpool.h"
#include "log.h"

CBufPool::CBufPool(uint32 nSize)
{
	m_nSize = nSize;
}

CBufPool::~CBufPool()
{

}

void CBufPool::Fini()
{
	for (auto & it : m_setBuf)
	{
		if (it != nullptr)
		{
			delete[] it;
		}
	}
}

char * CBufPool::Fetch()
{
	char * pBuf = nullptr;
	if (m_listFreeBuf.empty())
	{
		pBuf = new char[m_nSize];
		memset(pBuf, 0, m_nSize);
		m_setBuf.insert(pBuf);
	}
	else
	{
		pBuf = m_listFreeBuf.back();
		m_listFreeBuf.pop_back();
		m_setFreeBuf.erase(pBuf);
	}

	return pBuf;
}

void CBufPool::Release(char * pBuf)
{
	if (pBuf == nullptr)
	{
		return;
	}

	if (m_setBuf.find(pBuf) == m_setBuf.end())
	{
		return;
	}

	m_setFreeBuf.insert(pBuf);
	m_listFreeBuf.push_back(pBuf);
}

char * CBufPoolMgr::GetBufPool(uint32 nSize)
{
	CBufPool * pBufPool = nullptr;
	if (m_mapBufPool.find(nSize) == m_mapBufPool.end())
	{
		pBufPool = new CBufPool(nSize);
		m_mapBufPool[nSize] = pBufPool;
	}
	else
	{
		pBufPool = m_mapBufPool[nSize];
	}

	if (pBufPool == nullptr)
	{
		EXLOG_ERROR << "find null CBufPool.";
		return nullptr;
	}

	return pBufPool->Fetch();
}

void CBufPoolMgr::ReleaseBufPool(char * pBuf, uint32 nSize)
{
	CBufPool * pBufPool = nullptr;
	if (m_mapBufPool.find(nSize) == m_mapBufPool.end())
	{
		return;
	}

	pBufPool = m_mapBufPool[nSize];
	pBufPool->Release(pBuf);
}
