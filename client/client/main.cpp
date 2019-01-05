#include<WinSock2.h>
#include<iostream>
#include<WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
using namespace std;


int main(int argc, char* argv[])
{
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA data;
	if (WSAStartup(sockVersion, &data) != 0)
	{
		return 0;
	}

	char szIP[30];

	SOCKET sockClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sockClient == INVALID_SOCKET)
	{
		cout << "invalid socket!" << endl;
		return 0;
	}

	sockaddr_in svrAddr;
	svrAddr.sin_family = AF_INET;
	svrAddr.sin_port = htons(8888);
	inet_pton(AF_INET, "192.168.3.70", (void *)&svrAddr.sin_addr);
	if (connect(sockClient, (sockaddr *)&svrAddr, sizeof(svrAddr)) == SOCKET_ERROR)
	{
		cout << "connect error" << endl;
		closesocket(sockClient);
		return 0;
	}
	inet_ntop(AF_INET, &svrAddr.sin_addr, szIP, 30);
	Sleep(100);

	char szMsg[256];
	int nRet;
	while (true)
	{
		//cout << "Send:" << endl;
		//cin >> szMsg;
		memcpy(szMsg, "xxx", 4);
		send(sockClient, szMsg, strlen(szMsg), 0);

		nRet = recv(sockClient, szMsg, strlen(szMsg), 0);
		if (nRet > 0 && nRet < 256)
		{
			szMsg[nRet] = 0;
		}
		cout << "Receive:" << szMsg << endl;
		Sleep(1000);
	}

	closesocket(sockClient);
	WSACleanup();
	return 0;
}