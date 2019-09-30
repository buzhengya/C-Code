#include "pool.h"

using namespace std;

#define MEM_POOL_SIZE	4096
#define MEM_ALIGN		16
#define MAX_FAILED		4

void *	_AllocSmallBlock(MemPool * pool, uint32 nSize)
{
	uint32 nMpSize = 
}

void * _AllocSmall(MemPool * pool, uint32 nSize, bool bAlign)
{


}

void * _AllocLarge(MemPool * pool, uint32 nSize)
{
	void * pNew = wind_malloc(nSize);
	ASSERT(pNew != nullptr);

	MemLarge ** pLarge = &pool->pLarge;
	while (*pLarge != nullptr)
	{
		pLarge = &(*pLarge)->pNext;
	}

	(*pLarge)->pData = pNew;
	return pNew;
}

void * _AllocMem(MemPool * pool, uint32 nSize, bool bAlign, bool bInit)
{
	void * pData = nullptr;
	if (pool->nMax > nSize)
	{
		return _AllocSmall(pool, nSize, bAlign);
	}

	return 
}

MemPool * CreatePool(uint32 nSize)
{
	uint32 nMpSize = sizeof(MemPool);
	if (nSize < nMpSize)
	{
		LOG << "size is too small size : " << nSize << " MemPool size : " << nMpSize << endl;
		return nullptr;
	}
	// 限制分配的内存大小
	nSize = nSize > MEM_POOL_SIZE ? MEM_POOL_SIZE : nSize;

	MemPool * pool = (MemPool*)wind_malloc(nSize);
	ASSERT(pool != nullptr);

	pool->pLarge = nullptr;
	pool->nMax = nSize - sizeof(nMpSize);
	pool->pCur = pool;

	pool->oData.pLast = (char *)pool + nMpSize;
	pool->oData.pEnd = (char *)pool + nSize;
	pool->oData.pNext = nullptr;
	pool->oData.nFailed = 0;

	return pool;
}

bool DestoryPool(MemPool * pool)
{
	// large
	for (MemLarge * p = pool->pLarge; p != nullptr; p = p->pNext)
	{
		wind_free(p->pData);
		p->pData = nullptr;
	}

	// data (list data can not destory before get next)
	MemPool * pNext = nullptr;
	for (MemPool * p = pool; p != nullptr; p = pNext)
	{
		// get next
		pNext = p->oData.pNext;

		// delete this
		wind_free(p);
	}
	return true;

}

bool ResetPool(MemPool * pool)
{
	// large
	for (MemLarge * p = pool->pLarge; p != nullptr; p = p->pNext)
	{
		wind_free(p->pData);
		p->pData = nullptr;
	}

	// data
	pool->pCur = pool;
	for (MemPool * p = pool; p != nullptr; p = p->oData.pNext)
	{
		p->oData.pLast = (char*)p + sizeof(MemPool);
		p->oData.nFailed = 0;
	}
	return true;
}

void * Alloc(MemPool * pool, uint32 nSize)
{
	return _AllocSmall(pool, nSize, false, false);
}

void * Calloc(MemPool * pool, uint32 nSize)
{
	return _AllocSmall(pool, nSize, false, true);
}

void * AlignAlloc(MemPool * pool, uint32 nSize)
{
	return _AllocSmall(pool, nSize, true, false);
}

void * AlignCalloc(MemPool * pool, uint32 nSize)
{
	return _AllocSmall(pool, nSize, true, true);
}

bool FreeLarge(MemPool * pool, void * data)
{
	for (MemLarge * p = pool->pLarge; p != nullptr; p = p->pNext)
	{
		if (p->pData == data)
		{
			wind_free(data);
			return true;
		}
	}
	return false;
}
