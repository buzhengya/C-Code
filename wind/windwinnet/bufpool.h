#ifndef BUFPOOL_H
#define BUFPOOL_H

#include "base_define.h"
#include <list>
#include <map>
#include <set>
#include <string>

typedef std::list<char*> CListBuf;
typedef std::set<char*> CSetBuf;

class CBufPool
{
public:
	CBufPool(void);
	virtual ~CBufPool(void);

	void Init(uint32 dwSize);

	void Finish();

	uint32 GetSize();

	char* Fetch();

	void Release(char* pBuf);


protected:
	uint32		m_dwSize;
	CListBuf	m_oListBuf;
	CSetBuf		m_oSetBuf;
};
typedef std::map<uint32, CBufPool*> CMapBufPool;
#endif

