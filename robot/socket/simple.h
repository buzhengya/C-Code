#ifndef _SIMPLE_DEFINE_H_
#define _SIMPLE_DEFINE_H_
#include <WinSock2.h>
#include <iostream>
#include<WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
using namespace std;



int Simple()
{
	WORD sockVersion = MAKEWORD(2, 2); //表名想使用得windows socket版本 前一位时主版本 后一位时副版本
	WSADATA wsaData;
	if (WSAStartup(sockVersion, &wsaData) != 0)
	{
		return 0;
	}

	SOCKET sockListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sockListen == INVALID_SOCKET)
	{
		cout << "socket error" << endl;
		return 0;
	}

	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(8888);
	inet_pton(AF_INET, "192.168.3.70", &sin.sin_addr);
	if (bind(sockListen, (sockaddr *)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		cout << "bind error !" << endl;
	}

	if (listen(sockListen, 5) == SOCKET_ERROR)
	{
		cout << "listen error!" << endl;
		return 0;
	}

	SOCKET sockClient;
	sockaddr_in remoteAddr;
	int nAddrLen = sizeof(remoteAddr);
	char szRevData[255];
	while (true)
	{
		cout << "等待连接..." << endl;
		sockClient = accept(sockListen, (SOCKADDR *)&remoteAddr, &nAddrLen);
		if (sockClient == INVALID_SOCKET)
		{
			cout << "accept error";
			continue;
		}
		char szIP[16];
		inet_ntop(AF_INET, (void *)&remoteAddr.sin_addr, szIP, 16);
		cout << "接受到一个连接:" << szIP << endl;

		int nRet = recv(sockClient, szRevData, 254, 0);
		if (nRet > 0)
		{
			szRevData[nRet] = 0;
			cout << szRevData << endl;
		}

		char * szSendData = "hello, TCP client!\n";
		send(sockClient, szSendData, strlen(szSendData), 0);

		closesocket(sockClient);
	}
	closesocket(sockListen);
	WSACleanup();
	return 0;
}
#endif