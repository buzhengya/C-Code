#include "bufpool.h"
#include <sstream>
#include "logger.h"

CBufPool::CBufPool(void)
{
	m_dwSize = 0;
}

CBufPool::~CBufPool(void)
{
	Finish();
}

void CBufPool::Init(uint32 dwSize)
{
	m_dwSize = dwSize;
}

void CBufPool::Finish()
{
	for (auto it : m_oSetBuf)
	{
		if (it != nullptr)
		{
			delete it;
		}
	}
	m_oListBuf.clear();
	m_oSetBuf.clear();
}

uint32 CBufPool::GetSize()
{
	return m_dwSize;
}

char* CBufPool::Fetch()
{
	char *pszBuf = nullptr;
	if (!m_oListBuf.empty())
	{
		pszBuf = m_oListBuf.front();
		m_oListBuf.pop_front();
	}
	else
	{
		pszBuf = new char[m_dwSize];
		if (pszBuf == nullptr)
		{
			LOG << "CBufPool::Fetch(), new char failed!!!" << std::endl;
			return nullptr;
		}
		memset(pszBuf, 0, m_dwSize);
		m_oSetBuf.insert(pszBuf);
	}
	return pszBuf;
}

void CBufPool::Release(char* pBuf)
{
	if (m_oSetBuf.find(pBuf) != m_oSetBuf.end())
	{
		m_oListBuf.push_back(pBuf);
	}
}