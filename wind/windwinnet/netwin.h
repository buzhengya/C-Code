#ifndef _NETWIN_H_
#define _NETWIN_H_
#include "wnet.h"
#include "base_define.h"
#include "singleton.h"
using namespace wind;

//class CGNNetWin : public INet, public TSingleton<CGNNetWin>
//{
//	//friend class TSingleton<CGNNetWin>;
//	CGNNetWin();
//	virtual ~CGNNetWin();
//
//public:
//
//	virtual void WAPI AddRef(void);
//
//	virtual uint32  WAPI QueryRef(void);
//
//	virtual void WAPI Release(void);
//
//	virtual SVersion WAPI GetVersion(void);
//
//	virtual const char * WAPI GetModuleName(void);
//
//	virtual IConnector* WAPI CreateConnector(uint32 dwNetIOType);
//
//	virtual IListener*  WAPI CreateListener(uint32 dwNetIOType);
//
//	virtual bool WAPI Run(INT32 nCount);
//
//	bool Init();
//
//	void Fini();
//
//	void ReleaseConnector(CConnector* poConnector);
//
//	CConnector* FindConnector(uint32 dwID);
//
//	typedef std::vector<CCPSock * > ConnectedSockets;
//
//	void ProcRecvData(CConnData * pConnData, const char * pData, int len);
//
//
//	virtual std::string WAPI DebugInfo() override;
//protected:
//	bool _CreateComponent();
//
//	bool _InitComponent();
//
//	void _FiniComponent();
//
//	void _DesroryComponent();
//
//	void _ClearConnector();
//
//	void _ProcEstablishEvt(SEstablishRespEvt* pstEvent);
//	void _ProcAssociateEvt(SAssociateEvt* pstEvent);
//	void _ProcConnErrEvt(SConnErrRespEvt* pstEvent);
//	void _ProcErrorEvt(SErrorRespEvt* pstEvent);
//	void _ProcTerminateEvt(STerminateRespEvt* pstEvent);
//	void _ProcRecvEvt(SRecvRespEvt* pstEvent);
//	void _ProcSendEvt(SSendRespEvt* pstEvent);
//	//// 2009-03-24 cwy add for interface expanding, add bind function
//	void _ProcBindErrEvt(SBindErrRespEvt* pstEvent);
//
//
//protected:
//	uint32			m_dwRef;
//	uint32			m_dwNextConnectorID;
//	CMapConnector	m_oMapConnector;
//	char*			m_pRecvBuf;
//	uint32          m_dwLastTick;
//};

#endif // 
