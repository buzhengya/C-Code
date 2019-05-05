#pragma once
#include "singleton.h"
#include "net_event.h"
#include "data_queue.h"
#include "thread_buf.h"

using namespace wind;

class CEventMgr : public TSingleton<CEventMgr>
{
	friend class CEventMgr;
public:
	void Init();

	bool PushRecvEvt(CConnData *pConnData, char *szBuf, uint32 nSize);

	SNetEvent * PopNetEvt();
private:
	void _PushNetEvt(SNetEvent *pstNetEvt);

	void _ReleaseEvt(SNetEvent *pstNetEvt);
	SNetEvent * _GetNetEvt();

	CLockFreeQueue<SNetEvent*>	m_queFreeEvt;
	CLockFreeQueue<SNetEvent*>	m_queNetEvt;
	CThreadBuf *				m_pThreadBuf;
};
