#ifndef _LISTENER_H_
#define _LISTENER_H_
#include "wnet.h"
#include <WinSock2.h>
#include "sock.h"
#include <MSWSock.h>
#pragma comment(lib, "ws2_32.lib")
class CListener
{
public:
	CListener();
	~CListener();

	//inline void SetPacketParser(IPacketParser* poPacketParser) { m_poPacketParser = poPacketParser; }

	//inline void SetSessionFactory(ISessionFactory* poSessionFactory) { m_poSessionFactory = poSessionFactory; }

	//void SetBufferSize(uint32 dwRecvBufSize, uint32 dwSendBufSize) { m_dwRecvBufSize = dwRecvBufSize; m_dwSendBufSize = dwSendBufSize; }

	bool Start(const char* pszIP, uint16 wPort);

	void Stop();

	bool PostAcceptEx(SPerIoData* pstPerIoData);

	void OnAccept(SPerIoData* pstPerIoData);

	void GetSockAddress(SPerIoData * pstPerIoData, sockaddr_in & RemoteAddr, sockaddr_in &  LocalAddr);

	void GetLocalSockAddress(sockaddr_in &  LocalAddr);
protected:

	bool InitAcceptEx();


protected:
	SPerHandleData              m_stPerHandleData;
	SOCKET                      m_hListenSock;
	LPFN_ACCEPTEX               m_lpfnAcceptEx;
	LPFN_GETACCEPTEXSOCKADDRS   m_lpfnGetAcceptExSockaddrs;
	SPerIoData*                 m_pPerIoDataArray;

	//IPacketParser*				m_poPacketParser;
	//ISessionFactory*			m_poSessionFactory;
	//uint32                      m_dwRecvBufSize;
	//uint32                      m_dwSendBufSize;
	volatile bool               m_bStart;
};
#endif // 
