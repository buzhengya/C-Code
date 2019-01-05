#include "conn_data_allocator.h"
#include "logger.h"


CBufPool* ConnDataAllocator::FindBufPool(uint32 dwSize, bool bCreate)
{
	CMapBufPool::iterator it = m_mapBufPool.find(dwSize);
	if (it != m_mapBufPool.end())
	{
		return it->second;
	}
	if (bCreate == false)
	{
		return nullptr;
	}
	CBufPool* pBufPool = new CBufPool;
	if (pBufPool == nullptr)
	{
		LOG << "ConnDataAllocator::FindBufPool, newCBufPool failed!!!" << std::endl;
		return nullptr;
	}
	pBufPool->Init(dwSize);
	m_mapBufPool[dwSize] = pBufPool;
	return pBufPool;
}

void ConnDataAllocator::ReleaseBufPool(char * pBuf, uint32 dwSize)
{
	CBufPool* pBufPool = FindBufPool(dwSize, false);
	if (pBufPool != nullptr)
	{
		pBufPool->Release(pBuf);
	}
}