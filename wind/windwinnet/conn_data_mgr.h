#ifndef _CONN_DATA_MGR_H_
#define _CONN_DATA_MGR_H_
#include "base_define.h"
#include "conn_data_allocator.h"
#include "connection.h"
#include "singleton.h"
#include <vector>
#include "connection.h"
#include "net_config.h"

#define RUN_CONNECTION_LIST_SIZE 50

struct CConnData
{
	CConnData(int32 recvBufSize = DEFAULT_RECVBUF_SIZE, int32 sendBufSize = DEFAULT_SENDBUF_SIZE);

	~CConnData();

	// don't modify below values in your application 
	CConnection connection;
	CSock sock;
	char * sendBuf;
	char * recvBuf;

	uint32 connId;
};



class CConnDataMgr : public wind::TSingleton<CConnDataMgr>
{
	friend class wind::TSingleton<CConnDataMgr>;
	CConnDataMgr()
	{
		m_connDataAllocator = nullptr;

		m_initState = 0;
	}
	~CConnDataMgr()
	{
	};
	typedef std::vector<CConnData*> UsedConnDataList;

public:

	bool Init();

	void Finish();

	CConnData * Alloc(uint32 recvBufSize, uint32 sendBufSize);

	bool Find(CSock * pSock);

	void Release(CConnData * pConnData);

	uint8	AddRunConection(CConnection* pConnection);

	void	RunConection();
protected:
	void	__RunConection(std::list<CConnection*>& listRunConnection);
private:

	inline void AddUsedConnData(CConnData * pConnData)
	{
		m_usedConnData.push_back(pConnData);
	}
	void RemoveUsedConnData(CConnData * pConnData)
	{

	}

	bool FindUsedConnSock(CSock* pSock)
	{

	}




	friend class ConnDataAllocator;
	void ReleaseUsedConnData();

	ConnDataAllocator * m_connDataAllocator;
	UsedConnDataList m_usedConnData;
	std::list<CSock * >  m_delaySockList;
	time_t					m_nLastCheckTime;
	int m_initState;
	UINT8	m_byRunIndex;
	uint32	m_dwLastRunTime;
	std::list<CConnection*>  m_listRunConnection[RUN_CONNECTION_LIST_SIZE];
};
#endif