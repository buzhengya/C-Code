#pragma once
#include "wnet.h"

using namespace wind;

class CCPListener
{
public:
	virtual void SetPacketParser(IPacketParser* poPacketParser) { m_pPacketParser = poPacketParser; }

	virtual void SetSessionFactory(ISessionFactory* poSessionFactory) { m_pSessionFactory = poSessionFactory; }

	virtual void SetBuffSize(uint32 nRecvBufSize, uint32 nSendBufSize);

	virtual void Start();

	virtual void Stop();

	void PostAccept();

	void OnAccept();
private:

	bool _InitAccept();

	IPacketParser* m_pPacketParser;
	ISessionFactory * m_pSessionFactory;
	uint32 m_nRecvBufSize;
	uint32 m_nSendBufSize;
	bool   m_bStart;
	SOCKET m_hListenSock;

};