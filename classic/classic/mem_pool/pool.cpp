#include "pool.h"

using namespace std;

#define MEM_POOL_SIZE	4096
#define MEM_ALIGN		16

void *	_AllocSmallBlock(MemPool * pool, uint32 nSize)
{
	uint32 nMpSize = sizeof(MemPool);
	MemPool * pNew = (MemPool *)wind_malloc(nMpSize + pool->nMax);

	pNew->oData.pEnd = (char*)pNew + nMpSize + pool->nMax;
	pNew->oData.nFailed = 0;
	pNew->oData.pLast = (char *)pNew + nMpSize + nSize;
	pNew->oData.pNext = nullptr;

	MemPool * p = pool->pCur;
	while (p->oData.pNext != nullptr) // 找到了最后一个 但是没有处理最后一个
	{
		if (++p->oData.nFailed >= MAX_FAILED)
		{
			pool->pCur = p->oData.pNext;
		}

		p = p->oData.pNext;
	}

	p->oData.pNext = pNew;
	if (++p->oData.nFailed >= MAX_FAILED)
	{
		pool->pCur = pNew;
	}

	return (char *)pNew + nMpSize;
}

void * _AllocSmall(MemPool * pool, uint32 nSize, bool bAlign)
{
	MemPool * p = pool->pCur;
	char * pBegin = nullptr;
	while (p != nullptr)
	{
		pBegin = p->oData.pLast;
		if (bAlign)
		{
			pBegin = wind_align(p->oData.pLast, ALIGNMENT);
		}

		// find success
		if (p->oData.pEnd - pBegin >= nSize)
		{
			p->oData.pLast = pBegin + nSize;
			return pBegin;
		}

		p = p->oData.pNext;
	}

	return _AllocSmallBlock(pool, nSize);
}

void * _AllocLarge(MemPool * pool, uint32 nSize)
{
	MemLarge ** pLarge = &pool->pLarge;
	while (*pLarge != nullptr && (*pLarge)->pData != nullptr)
	{
		pLarge = &(*pLarge)->pNext;
	}

	void * pNew = wind_malloc(nSize);
	ASSERT(pNew != nullptr);

	if (*pLarge != nullptr)
	{
		(*pLarge)->pData = pNew;
	}
	else
	{
		MemLarge * pNewLarge = (MemLarge*)wind_malloc(sizeof(MemLarge));
		pNewLarge->pData = pNew;
		pNewLarge->pNext = nullptr;

		*pLarge = pNewLarge;
	}
	LOG << "Large ptr addr : " << (uint64)pNew << endl;
	return pNew;
}

void * _AllocMem(MemPool * pool, uint32 nSize, bool bAlign, bool bInit)
{
	void * pData = nullptr;
	if (pool->nMax >= nSize)
	{
		pData = _AllocSmall(pool, nSize, bAlign);
	}
	else
	{
		pData = _AllocLarge(pool, nSize);
	}

	if (bInit)
	{
		memset(pData, 0, nSize);
	}

	return pData;
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
	pool->nMax = nSize - nMpSize;
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
		if (p->pData != nullptr)
		{
			LOG << "free large data addr : " << uint64(p->pData) << endl;
			wind_free(p->pData);
		}
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
	return _AllocMem(pool, nSize, false, false);
}

void * Calloc(MemPool * pool, uint32 nSize)
{
	return _AllocMem(pool, nSize, false, true);
}

void * AlignAlloc(MemPool * pool, uint32 nSize)
{
	return _AllocMem(pool, nSize, true, false);
}

void * AlignCalloc(MemPool * pool, uint32 nSize)
{
	return _AllocMem(pool, nSize, true, true);
}

bool FreeLarge(MemPool * pool, void * data)
{
	for (MemLarge * p = pool->pLarge; p != nullptr; p = p->pNext)
	{
		if (p->pData == data)
		{
			wind_free(data);
			p->pData = nullptr;
			return true;
		}
	}
	return false;
}
