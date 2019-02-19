#ifndef _CONN_DATA_ALLOCATOR_H_
#define _CONN_DATA_ALLOCATOR_H_
#include "base_define.h"
#include <deque>
#include "logger.h"
#include "bufpool.h"
#include "sock.h"
#include "conn_data_mgr.h"

static uint32 g_connectionId = 0;


class ConnDataAllocator
{
	enum
	{
		POOL_INFLATE_SIZE = 200,
	};
public:
	typedef std::deque<CConnData*> ObjectQueue;
	typedef ObjectQueue::iterator ObjectQueueItr;

	ConnDataAllocator()
	{
		m_nTotalSize = 0;
		Infalate();
	}

	~ConnDataAllocator()
	{
		for (auto it : m_mapBufPool)
		{
			
		}
		for (auto it : m_objects)
		{
			delete it;
		}
	}

	CConnData* Allocate(uint32 recvBufSize, uint32 sendBufSize)
	{
		if (m_objects.empty())
		{
			Infalate();
		}
		if (m_objects.empty())
		{
			LOG << "CConnDataAllocator::allocate failed!!!" << std::endl;
			return nullptr;
		}
		CConnData* pConnData = m_objects.front();
		m_objects.pop_front();

		CConnDataMgr* pConnDataMgr = CConnDataMgr::Instance();
		pConnDataMgr->AddUsedConnData(pConnData);

		pConnData->connId = g_connectionId++;
		pConnData->recvBuf = FindBufPool(recvBufSize)->Fetch();
		pConnData->sendBuf = FindBufPool(sendBufSize)->Fetch();
		return pConnData;
	}

	void Deallocate(CConnData* pConnData, uint32 recvBufSize, uint32 sendBufSize)
	{
		ReleaseBufPool(pConnData->recvBuf, recvBufSize);
		ReleaseBufPool(pConnData->sendBuf, sendBufSize);
		m_objects.push_back(pConnData);
	}

	bool Find(CSock * pSock)
	{
		
	}

	CBufPool* FindBufPool(uint32 dwSize, bool bCreate = true);

	
	void ReleaseBufPool(char * pBuf, uint32 dwSize);	

	inline size_t MaxSize() const 
	{
		return m_nTotalSize;
	}

private:
	
	void Infalate(int count = POOL_INFLATE_SIZE)
	{
		for (int i = 0; i < count; i++)
		{
			CConnData *pConnData = new CConnData;
			if (pConnData == nullptr)
			{
				LOG << "CConnDataAllocator::Infalate, new CConnData failed!!!" << std::endl;
				m_nTotalSize += i;
				return;
			}
			m_objects.push_back(pConnData);
		}
		m_nTotalSize += count;
	}

	
	ConnDataAllocator(const ConnDataAllocator&)
	{
		
	}

	ObjectQueue m_objects; //save unused ConnData
	int m_nTotalSize; 
	CMapBufPool		 m_mapBufPool;
};
#endif // 
