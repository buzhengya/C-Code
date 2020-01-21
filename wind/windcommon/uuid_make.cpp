#include "uuid_make.h"
#include "wtime.h"

#define MAX_SEQ_ID 4096
#define SEQ_BIT_LEN 12

uint64 CUUIDMaker::GenUUID()
{
	uint64 nId = CTime::Instance()->GetNowMilTime();
	m_nSeqId++;
	if (m_nSeqId >= 4096)
	{
		m_nSeqId = 0;
	}
	return (nId << SEQ_BIT_LEN) + m_nSeqId;
}
