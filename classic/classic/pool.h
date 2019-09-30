#pragma once
#include "..\common\define.h"

struct MemPool {

};

MemPool * CreatePool();

bool * DestoryPool(MemPool * pool);

bool * ResetPool(MemPool * pool);

void * Alloc(MemPool * pool, uint32 nSize);

void * AllignAlloc(MemPool * pool, uin32 nSize);
