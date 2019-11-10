#pragma once
#include "define.h"
#include <vector>
#include <set>
#include <unordered_map>

// ATTENTION!!!  �̲߳���ȫ ������߳�ͬʱ�����ö��� ������ֲ�������������
class CMemPool
{
public:
	CMemPool(uint32 nSize);
	CMemPool() = delete;
	CMemPool(CMemPool&) = delete;
	CMemPool& operator=(CMemPool&) = delete;
	~CMemPool();

	char*		Alloc();
	void		Free(char* pszData);
	int32		GetSize() const;
	void		DebugInfo() const;
	bool		IsAllFree() const;

	void		ReleaseMem();

private:
	char*	_Alloc();
	void	_Free(char* pszData);

	void	_ExpandPool();

	void	_Reset();

	std::vector<char*>	m_vecFree; // memory for free
	std::set<char*>		m_setAll; // memory for all (to judge weather memory belong this)
	int32				m_nSize;
};

class IMemPoolMgr
{
public:
	virtual char* Alloc(uint32 nSize) = 0;
	virtual void  Free(char* pData, uint32 nSize) = 0;
	virtual void  DebugInfo() = 0;
	virtual bool  IsAllFree() = 0;
};

// Q:ΪʲôҪ��������ڴ�ع��� 
//A: static ��Ϊȫ�ֵ��ڴ�� �ڽ��̽���ǰ��ʹ��ͬһ���ڴ�� ���ȫ���Ƿ����ڴ�й©
// temp ��Ϊ��ʱ���ڴ�� ���ھֲ� ��֤�ֲ����ڴ治��й¶
class CMemPoolMgr : public IMemPoolMgr
{
	using MapMemPool = std::unordered_map<uint32, CMemPool>;
public:
	CMemPoolMgr(bool bGlobal = true) : m_bGlobal(bGlobal) {};
	CMemPoolMgr(CMemPoolMgr&) = delete;
	CMemPoolMgr operator=(CMemPoolMgr&) = delete;
	~CMemPoolMgr();

	virtual char*	Alloc(uint32 nSize);
	virtual void	Free(char* pszData, uint32 nSize);

	virtual void	DebugInfo();
	virtual bool	IsAllFree();

private:
	char*		_Alloc(MapMemPool& oMap, uint32 nSize);
	void		_Free(MapMemPool& oMap, char* pszData, uint32 nSize);
	CMemPool&	_GetMemPool(MapMemPool& oMap, uint32 nSize);
	MapMemPool& _GetMap();
	void		_DebugInfo(const MapMemPool& oMap) const;
	bool		_IsAllFree(const MapMemPool& oMap) const;

	bool				m_bGlobal;
	MapMemPool			m_mapMemPool;
	static MapMemPool	s_mapMemPool;
};

static CMemPoolMgr POOL_MGR;
