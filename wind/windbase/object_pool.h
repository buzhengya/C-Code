#pragma once
#include "base_define.h"
#include <string>
#include <sstream>
#include "logger.h"

using namespace std;

enum
{
	POOL_INIT_SIZE = 128,
	POOL_GROW_SIZE = 32,
	POOL_MAX_SIZE = 256 * 1024 * 1024,   //default 256M 
};

class IObjPool
{
public:
	virtual bool Init(int32 nInitCount, int32 nGrowCount, int32 nMaxSzie) = 0;
	virtual void Fini() = 0;
	virtual string GetObjName() = 0;
	virtual string GetDebugInfo() = 0;
};

template<class T>
class CObjPool : public IObjPool
{
public :
	explicit CObjPool()
	{

	}

	virtual ~CObjPool()
	{
		Fini();
	}

	virtual bool Init(int32 nInitCount, int32 nGrowCount = POOL_GROW_SIZE, int32 nMaxSize = POOL_MAX_SIZE)
	{
		m_nInitCount = nInitCount;
		m_nGrowCount = nGrowCount;
		m_nMaxSize = nMaxSize;
		m_nObjSize = sizeof(T);
		m_strObjName = typeid(T).name();

		AllocRaw(m_nInitCount);
		return true;
	}

	void Fini()
	{
		for (auto it = m_listBlock.begin(); it != m_listBlock.end(); it++)
		{
			if (*it != nullptr)
			{
				delete *it;
			}
		}
	}

	virtual string GetObjName() override //强调该函数是重写覆盖基类的函数
	{
		return m_strObjName;
	}

	virtual string GetDebugInfo() override
	{
		if (0 == m_nObjSize)
		{
			return "object size is zero";
		}

		stringstream oSS;
		int nTotalObjs = m_nTotalSize / m_nObjSize;
		int nFreeObjs = m_listFreeChunk.size();
		int nUsage = 0;
		if (nTotalObjs != 0)
		{
			nUsage = int(((nTotalObjs - nFreeObjs)*1.0f / nTotalObjs) * 100);
		}

		oSS << m_strObjName << "(" << m_nObjSize << "), "
			<< "blockNum=" << m_listBlock.size() << ", "
			<< "totalObjs=" << m_nTotalSize / m_nObjSize << ", "
			<< "freeObjs=" << m_listFreeChunk.size() << ", "
			<< "totalMem=" << m_nTotalSize << ", "
			<< "usage=" << nUsage << "%";

		return oSS.str();
	}

	virtual int32 GetTotalSize() const { return m_nTotalSize; }

	T * FetchObj()
	{
		if (m_listFreeChunk.empty())
		{
			AllocRaw(m_nGrowCount);
		}

		if (m_listFreeChunk.empty())
		{
			return nullptr;
		}
		T * pObj = m_listFreeChunk.front();
		m_listFreeChunk.pop_front();
		m_nUseCount++;

		new (pObj) T();
		return pObj;
	}

	void ReleaseObj(T * pObj)
	{
		if (pObj != nullptr)
		{
			m_nUseCount--;
			m_listFreeChunk.push_back(pObj);
		}
	}

private:
	void AllocRaw(const int nCount)
	{
		if (m_nTotalSize + m_nObjSize * nCount > m_nMaxSize)
		{
			LOG << "CObjPool::AllocRaw is overflow. name : " << m_strObjName << " total size : " << m_nTotalSize << endl;
			return;
		}
		T * arrObj = (T*)malloc(m_nObjSize*nCount);
		if (arrObj == nullptr)
		{
			LOG << "CObjPool::AllocRaw is nullptr. obj name : " << m_strObjName << " total size : " << m_nTotalSize << endl;
			return;
		}

		m_nTotalSize += m_nObjSize * nCount;
		for (int i = 0; i < nCount; i++)
		{
			m_listFreeChunk.push_back(arrObj + i);
			m_listBlock.push_back(arrObj + i);
		}
	}

	list<T*> m_listBlock;
	list<T*> m_listFreeChunk;

	int32 m_nInitCount;
	int32 m_nGrowCount;
	int32 m_nMaxSize;
	int32 m_nTotalSize;
	int32 m_nObjSize;
	int32 m_nUseCount;

	string m_strObjName;
};