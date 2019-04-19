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
	bool Start();
	void OnAccept(SPerIoData * pstPerIoData);
private:

	bool _InitAcceptex();
	bool _PostAcceptex(SPerIoData * pstPerIoData);

	string			m_strIp;
	uint32			m_nPort;
	SOCKET			m_hListen;
	sockaddr_in     m_stAddr;

	LPFN_ACCEPTEX   m_lpfnAcceptex;
	SPerIoData *	m_pstIoData;
	SPerKeyData		m_stKeyData;
};