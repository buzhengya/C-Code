#include "http_net.h"
#include <iostream>
#include<WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")


bool CHttpNet::Init(string strIp, uint32 nPort, uint32 nTimeOut)
{
	WORD hVersion = MAKEWORD(2, 2);
	WSADATA data;
	WSAStartup(hVersion, &data);

	m_strIp = strIp;
	m_nPort = nPort;
	m_nTimeOut = nTimeOut;
	return true;
}

bool CHttpNet::SendMsg(char * pszSendBuf, uint32 nSendSize, char * pszRecvBuf, uint32 nRecvSize)
{
	SOCKET hClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (hClient == INVALID_SOCKET)
	{
		cout << "socket init is invalid!" << endl;
		return false;
	}

	sockaddr_in svrAddr;
	svrAddr.sin_family = AF_INET;
	svrAddr.sin_port = htons(m_nPort);
	inet_pton(AF_INET, m_strIp.c_str(), (void *)&svrAddr.sin_addr);

	int nTimeOut = m_nTimeOut;
	setsockopt(hClient, SOL_SOCKET, SO_SNDTIMEO, (char *)&nTimeOut, sizeof(nTimeOut));
	setsockopt(hClient, SOL_SOCKET, SO_RCVTIMEO, (char *)&nTimeOut, sizeof(nTimeOut));


	if (connect(hClient, (sockaddr *)&svrAddr, sizeof(sockaddr_in)) == SOCKET_ERROR)
	{
		cout << "socket connect error. ip : " << m_strIp << "  port : " << m_nPort << endl;
		closesocket(hClient);
		return false;
	}

	if (send(hClient, pszSendBuf, nSendSize, 0) == SOCKET_ERROR)
	{
		cout << "socket send error. ip : " << m_strIp << " port : " << m_nPort << " error : " << WSAGetLastError() << endl;
		closesocket(hClient);
		return false;
	}

	int nRet = recv(hClient, pszRecvBuf, nRecvSize, 0);
	if (nRet == SOCKET_ERROR)
	{
		cout << "socket recv error. ip : " << m_strIp << " port : " << m_nPort << " error : " << WSAGetLastError() << endl;
		closesocket(hClient);
		return false;
	}

	pszRecvBuf[nRet] = '\0';
	closesocket(hClient);
	return true;
}