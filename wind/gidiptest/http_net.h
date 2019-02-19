#pragma once
#include "singleton.h"
#include "base_define.h"
#include<string>
#include<WinSock2.h>

#define BUF_SIZE 2048

using namespace wind;
using namespace std;

class CHttpNet : public TSingleton<CHttpNet>
{
public:
	CHttpNet() {}

	~CHttpNet() {}

	bool Init(string strIp, uint32 nPort, uint32 nTimeOut);

	void SetIp(const string strIp) { m_strIp = strIp; }

	string GetIp()const { return m_strIp; }

	void SetPort(const uint32 nPort) { m_nPort = nPort; }

	uint32 GetPort() { return m_nPort; }

	bool SendMsg(char * pszSendBuf, uint32 nSendSize, char * pszRecvBuf, uint32 nRecvSize);

private:

	string m_strIp;
	uint32 m_nPort;
	uint32 m_nTimeOut;
};