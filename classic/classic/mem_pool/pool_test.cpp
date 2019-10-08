#include "pool_test.h"
#include "..\common\rand_tool.h"
#include <vector>

using namespace std;

#define MEM_POOL_SIZE 2048
#define STRUCT_SIZE sizeof(MemPool)


bool _CheckMemPool(MemPool * pool)
{
	ASSERT(pool->nMax == MEM_POOL_SIZE);
	//ASSERT(pool->pLarge != nullptr);



	MemPool * p = pool;
	while (p != nullptr && p->oData.nFailed == MAX_FAILED)
	{
		char * pLast = p->oData.pLast;
		char * pEnd = p->oData.pEnd;
		ASSERT(pLast >= (char*)p + STRUCT_SIZE && pLast <= pEnd);
		ASSERT(pEnd == (char *)p + STRUCT_SIZE + MEM_POOL_SIZE);

		p = p->oData.pNext;
	}

	ASSERT(pool->pCur == p);

	while (p != nullptr)
	{
		ASSERT(p->oData.nFailed < MAX_FAILED);

		char * pLast = p->oData.pLast;
		char * pEnd = p->oData.pEnd;
		ASSERT(pLast >= (char*)p + STRUCT_SIZE && pLast <= pEnd);
		ASSERT(pEnd == (char *)p + STRUCT_SIZE + MEM_POOL_SIZE);

		p = p->oData.pNext;
	}

	return true;
}

bool _CheckMemAlign(void * ptr)
{
	return ((uint64)ptr & (ALIGNMENT - 1)) == 0;
}

bool _CheckReset(MemPool * pool)
{
	MemLarge * pLarge = pool->pLarge;
	while (pLarge != nullptr)
	{
		ASSERT(pLarge->pData == nullptr);
		pLarge = pLarge->pNext;
	}

	ASSERT(pool == pool->pCur);
	ASSERT(pool->nMax == MEM_POOL_SIZE);

	MemPool * p = pool;
	while (p != nullptr)
	{
		ASSERT(p->oData.nFailed == 0);
		ASSERT(p->oData.pLast == (char *)p + STRUCT_SIZE);
		ASSERT(p->oData.pEnd == (char *)p + MEM_POOL_SIZE + STRUCT_SIZE);
		p = p->oData.pNext;
	}

	return true;
}

// ���� �ںв��� �� ����ͻ��մ��ڴ�Ȳ����� δ�ͷŵ������Ƿ�����
struct MemPoolTest
{
	char * pData;
	uint32 nSize;
	char * pBackup;
};
vector<MemPoolTest> vecTest;

// ����ѷ�����ڴ��Ƿ�������
bool _CheckAllData()
{
	for (const auto & oTest : vecTest)
	{
		ASSERT(memcmp(oTest.pData, oTest.pBackup, oTest.nSize) == 0);
	}

	return true;
}

// ֻ���С�ڴ�
bool _CheckSmallDta()
{
	for (const auto & oTest : vecTest)
	{
		if (oTest.nSize > MEM_POOL_SIZE) continue;
		ASSERT(memcmp(oTest.pData, oTest.pBackup, oTest.nSize) == 0);
	}

	return true;
}

void _AddNewData(void * pData, uint32 nSize)
{
	MemPoolTest oTest;
	oTest.pData = (char *)pData;
	oTest.nSize = nSize;
	oTest.pBackup = (char *)wind_malloc(nSize);
	memcpy(oTest.pBackup, oTest.pData, nSize);

	vecTest.push_back(oTest);
}

void _AddAndCheck(MemPool * pool, uint32 nCount)
{
	// ���������С�����ݵ��ڴ�
	void * pData = nullptr;
	uint32 nRandRight = MEM_POOL_SIZE + MEM_POOL_SIZE / 3;
	for (int32 i = 0; i < nCount; i++)
	{
		uint32 nSize = GetRand(1, nRandRight);
		LOG << "DEBUG size : " << nSize << endl;
		if ((i & 3) != 0)
		{
			pData = Alloc(pool, nSize);
		}
		else if ((i & 2) != 0)
		{
			pData = Calloc(pool, nSize);
		}
		else if ((i & 1) != 0)
		{
			pData = AlignAlloc(pool, nSize);
			ASSERT(_CheckMemAlign(pData));
		}
		else
		{
			pData = AlignCalloc(pool, nSize);
			ASSERT(_CheckMemAlign(pData));
		}
		_AddNewData(pData, nSize);
	}

	// У�������ڴ�
	ASSERT(_CheckAllData());
	
	// У���ڴ������
	ASSERT(_CheckMemPool(pool));


}

void TestPool(uint32 nCount)
{
	MemPool * pool = CreatePool(MEM_POOL_SIZE + STRUCT_SIZE);
	ASSERT(pool != nullptr);

	_AddAndCheck(pool, nCount);

	// FreeLarge У��С�ڴ�
	for (const auto & oTest : vecTest)
	{
		if (oTest.nSize > MEM_POOL_SIZE)
		{
			ASSERT(FreeLarge(pool, oTest.pData));
		}
	}
	_CheckSmallDta();

	// У���ڴ������
	ASSERT(_CheckMemPool(pool));

	// ResetPool У��
	vecTest.clear();
	ASSERT(ResetPool(pool));
	ASSERT(_CheckReset(pool));


	LOG << "==========================================================================" << endl;
	_AddAndCheck(pool, nCount);

	// DestoryPool У��
	ASSERT(DestoryPool(pool));
}
