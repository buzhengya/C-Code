#include "conndatamgr.h"
#include "log.h"

CConnData::CConnData(uint32 nRecvBufSize, uint32 nSendBufSize)
{
	szSendBuf = nullptr;
	szRecvBuf = nullptr;
	nSendSize = nSendBufSize;
	nRecvSize = nRecvBufSize;
}

CConnData::~CConnData()
{
}

void CConnData::Release()
{

}

void CConnDataMgr::Init()
{
	m_nCurConnId = 0;
}

CConnData * CConnDataMgr::AllocConnData(uint32 nRecvSize, uint32 nSendSize)
{
	CConnData * pConnData = new CConnData();
	if (pConnData == nullptr)
	{
		EXLOG_ERROR << "AllocConnData failed. pConnData is nullptr";
		return nullptr;
	}

	pConnData->szSendBuf = new char[nRecvSize];
	pConnData->szRecvBuf = new char[nSendSize];
	pConnData->nSendSize = nSendSize;
	pConnData->nRecvSize = nRecvSize;
	pConnData->nConnId = m_nCurConnId++;

	if (pConnData->szRecvBuf == nullptr || pConnData->szSendBuf == nullptr)
	{
		EXLOG_ERROR << "alloc send buf or recv buf failed.";
		return nullptr;
	}

	return pConnData;
}