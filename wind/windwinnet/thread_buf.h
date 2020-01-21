#pragma once
#include "base_define.h"
#include "loop_buf.h"

class CThreadBuf
{
public:
	CThreadBuf();
	
	~CThreadBuf();
	
	void Init();

	void Finish();
 
	bool CreateLoopBuf(uint32 nSize = 1024*1024*16);

	bool ReleaseLoopBuf();

	CLoopBuf * PushData(char * szBuf, uint32 nSize);
private:
	uint32 m_nIndex;
};
