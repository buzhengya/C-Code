#include "mem_pool.h"
#include "../log/log.h"

#include <chrono>
#include <thread>

using namespace std;

// 内存池内存不足时 每次直接新增16个指定大小的堆内存
const uint32 MEM_GROW_STEP = 16;
const uint32 MAX_SIZE = 1024 * 1024 * 64; // MAX_SIZE 最大为 64M

char* CMemPool::Alloc()
{
	return _Alloc();
}

void CMemPool::Free(char* pszData)
{
	_Free(pszData);
}

int32 CMemPool::GetSize() const
{
	return m_nSize;
}

void CMemPool::DebugInfo() const
{
	uint32 nFree = m_vecFree.size();
	uint32 nTotal = m_setAll.size();
	float nUsage = 0;
	if (nTotal != 0)
	{
		nUsage = (float)(nTotal - nFree) / nTotal * 100;
	}
	LOG_DEBUG << "SIZE : " << m_nSize << " free : " << nFree << " total : " << nTotal << " usage : " << nUsage;
}

bool CMemPool::IsAllFree() const
{
	return m_setAll.size() == m_vecFree.size();
}

void CMemPool::ReleaseMem()
{
	_Reset();
}

char* CMemPool::_Alloc()
{
	if (m_vecFree.empty())
	{
		_ExpandPool();
	}

	ASSERT(!m_vecFree.empty());

	char* pszData = m_vecFree.back();
	m_vecFree.pop_back();
	return pszData;
}

void CMemPool::_Free(char* pszData)
{
	if (m_setAll.find(pszData) == m_setAll.end())
	{
		return;
	}

	m_vecFree.push_back(pszData);
}

void CMemPool::_ExpandPool()
{
	if (!m_vecFree.empty())
	{
		return;
	}

	uint32 nTotal = m_nSize * MEM_GROW_STEP;
	while (true)
	{
		char* pszData = new char[nTotal];
		if (pszData != nullptr)
		{
			for (int32 i = 0; i < MEM_GROW_STEP; ++i)
			{
				char* ptr = pszData + (i * m_nSize);
				m_setAll.emplace(ptr);
				m_vecFree.push_back(ptr);
			}
			break;
		}

		LOG_ERROR << "memory alloc failed. total : " << nTotal << " size : " << m_nSize;
		this_thread::sleep_for(chrono::milliseconds(1000));
	}
}

void CMemPool::_Reset()
{
	// free all memory
	for (auto* pszData : m_setAll)
	{
		if (pszData != nullptr)
		{
			delete[] pszData;
		}
	}

	m_setAll.clear();
	m_vecFree.clear();
	m_nSize = 0;
}

CMemPool::CMemPool(uint32 nSize) : m_nSize(nSize)
{
	ASSERT(nSize <= MAX_SIZE);
}

CMemPool::~CMemPool()
{
	_Reset();
}

std::unordered_map<uint32, CMemPool> CMemPoolMgr::s_mapMemPool;
using MapMemPool = std::unordered_map<uint32, CMemPool>;

CMemPoolMgr::~CMemPoolMgr()
{
	for (auto& it : m_mapMemPool)
	{
		it.second.ReleaseMem();
	}
	m_mapMemPool.clear();
}

char* CMemPoolMgr::Alloc(uint32 nSize)
{
	return _Alloc(_GetMap(), nSize);
}

void CMemPoolMgr::Free(char* pszData, uint32 nSize)
{
	_Free(_GetMap(), pszData, nSize);
}

void CMemPoolMgr::DebugInfo()
{
	_DebugInfo(_GetMap());
}

bool CMemPoolMgr::IsAllFree()
{
	return _IsAllFree(_GetMap());
}

char* CMemPoolMgr::_Alloc(MapMemPool& oMap, uint32 nSize)
{
	auto& oPool = _GetMemPool(oMap, nSize);
	return oPool.Alloc();
}

void CMemPoolMgr::_Free(MapMemPool& oMap, char* pszData, uint32 nSize)
{
	auto& oPool = _GetMemPool(oMap, nSize);
	oPool.Free(pszData);
}

CMemPool& CMemPoolMgr::_GetMemPool(MapMemPool& oMap, uint32 nSize)
{
	auto it = oMap.find(nSize);
	if (it == oMap.end())
	{
		oMap.emplace(piecewise_construct, forward_as_tuple(nSize), forward_as_tuple(nSize));
		it = oMap.find(nSize);
	}

	return it->second;
}

MapMemPool& CMemPoolMgr::_GetMap()
{
	if (m_bGlobal)
	{
		return s_mapMemPool;
	}
	return m_mapMemPool;
}

void CMemPoolMgr::_DebugInfo(const MapMemPool& oMap) const
{
	LOG_DEBUG << "total memory pool size : " << oMap.size();
	for (auto& it : oMap)
	{
		it.second.DebugInfo();
	}
}

bool CMemPoolMgr::_IsAllFree(const MapMemPool& oMap) const
{
	for (auto& it : oMap)
	{
		if (!it.second.IsAllFree())
		{
			return false;
		}
	}
	return true;

}
