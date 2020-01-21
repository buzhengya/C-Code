#pragma once
#include "base_define.h"
#include <list>
#include <set>
#include "singleton.h"
#include <map>

using namespace std;
using namespace wind;

class CBufPool
{
public:
	CBufPool(uint32 nSize);
	~CBufPool();

	void Fini();

	uint32 GetSize() {}

	char * Fetch();

	void Release(char * pBuf);

private:
	uint32 m_nSize;
	list<char*> m_listFreeBuf;
	set<char*>  m_setBuf;
	set<char*>  m_setFreeBuf;
};

class CBufPoolMgr : public TSingleton<CBufPoolMgr>
{
public:
	char * GetBufPool(uint32 nSize);

	void   ReleaseBufPool(char * pBuf, uint32 nSize);
private:
	map<uint32, CBufPool*> m_mapBufPool;
};