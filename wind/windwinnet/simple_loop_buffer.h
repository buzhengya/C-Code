#ifndef _SIMPLE_LOOP_BUFFER_H_
#define _SIMPLE_LOOP_BUFFER_H_
#include "base_define.h"

//using namespace wind;

class CSimpleLoopBuffer
{
public:
	CSimpleLoopBuffer();
	~CSimpleLoopBuffer();

	bool Init(int32 nSize);

	int32 PushBack(const char *pData, int32 nLen);

	int32 PopFront(int32 nLen, char* szData);

private:
	char    *m_pBuffer;
	char    *m_pNextRead;
	char    *m_pNextWrite;
	char    *m_pEnd;
};
#endif // 
