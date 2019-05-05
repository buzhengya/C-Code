#pragma once
#include "base_define.h"

class CLoopBuf
{
public:
	CLoopBuf(uint32 nSize);
	
	~CLoopBuf();

	bool Write(char *pszBuf, uint32 nSize);

	bool Read(char *pszBuf, uint32 nSize);
private:
	char * m_pszBuf;
	uint32 m_nSize;
	char * m_pszStart;
	char * m_pszEnd;
};