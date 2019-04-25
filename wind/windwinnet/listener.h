#pragma once
#include "wnet.h"
#include <WinSock2.h>
#include <MSWSock.h>
#include "cpsock.h"
#include "iocpctrl.h"

using namespace wind;

class CCPListener
{
public:
	CCPListener(string strIp, uint32 nPort);
	~CCPListener() {}
	bool Start(uint32 nSendSize, uint32 nRecvSize);
	void OnAccept(SPerIoData * pstPerIoData);
	void SetSessionFactory(ISessionFactory * pSessionFactory);
private:
	void _PrintSockAddr(SPerIoData * pstPerIoData);
	bool _InitAcceptex();
	bool _PostAcceptex(SPerIoData * pstPerIoData);

	string			m_strIp;
	uint32			m_nPort;
	SOCKET			m_hListen;
	sockaddr_in     m_stAddr;

	LPFN_ACCEPTEX   m_lpfnAcceptex;
	LPFN_GETACCEPTEXSOCKADDRS m_lpfnSockAddr;

	SPerIoData *	m_pstIoData;
	SPerKeyData		m_stKeyData;

	uint32 m_nSendSize;
	uint32 m_nRecvSize;

	ISessionFactory * m_pSessionFactory;
};