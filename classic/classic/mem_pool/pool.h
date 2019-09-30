#pragma once
#include "..\common\define.h"

struct MemLarge {
	void *		pData;
	MemLarge *	pNext;
};

struct MemPool;

struct PoolData {
	char *		pLast;
	char *		pEnd;
	MemPool *	pNext;
	uint32		nFailed;
};

struct MemPool {
	PoolData	oData;
	MemLarge *	pLarge;
	uint32		nMax;
	MemPool *	pCur;
};

MemPool * CreatePool(uint32 nSize);

bool DestoryPool(MemPool * pool);

bool ResetPool(MemPool * pool);

void * Alloc(MemPool * pool, uint32 nSize);

void * Calloc(MemPool * pool, uint32 nSize);

void * AlignAlloc(MemPool * pool, uint32 nSize);

void * AlignCalloc(MemPool * pool, uint32 nSize);

bool   FreeLarge(MemPool * pool, void * data);

// 文件操作待续
