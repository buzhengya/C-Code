#include "select.h"
#include<WS2tcpip.h>
#include<time.h>

int g_nTotalConn1 = 0;
SOCKET g_arrSocketCli[FD_SETSIZE];

DWORD WINAPI WorkerThread1(LPVOID IpParameter)
{
	fd_set fdRead;
	timeval tv = { 1,0 };
	char szMsg[MSG_SIZE];

	while(true)
	{
		FD_ZERO(&fdRead);
		for (int i = 0; i < g_nTotalConn1; i++)
		{
			FD_SET(g_arrSocketCli[i], &fdRead);
		}
		SYSTEMTIME t;
		GetSystemTime(&t);
		cout << "before select:" << t.wSecond * 1000 + t.wMilliseconds << endl;
		int nRet = select(0, &fdRead, NULL, NULL, &tv);
		GetSystemTime(&t);
		cout << "after select:" << t.wSecond * 1000 + t.wMilliseconds << endl;
		if( nRet == 0)
		{
			Sleep(100);
			continue;
		}
		for (int i = 0; i < g_nTotalConn1; i++)
		{
			if (FD_ISSET(g_arrSocketCli[i], &fdRead))
			{
				nRet = recv(g_arrSocketCli[i], szMsg, MSG_SIZE, 0);
				if (nRet == 0 || (nRet == SOCKET_ERROR && WSAGetLastError() == WSAECONNRESET))
				{
					cout << "Client socket " << g_arrSocketCli[i] << " closed." << endl;
					closesocket(g_arrSocketCli[i]);
					if (i + 1 < g_nTotalConn1)
					{
						g_arrSocketCli[i--] = g_arrSocketCli[g_nTotalConn1 - 1];
					}
					g_nTotalConn1--;
				}
				else
				{
					szMsg[nRet] = 0;
					send(g_arrSocketCli[i], szMsg, strlen(szMsg), 0);
				}
			}
		}
	}
	return 0;
}

int Select()
{
	WSADATA wsaData;
	SOCKET sockListen, sockCli;
	sockaddr_in addrLocal, addrCli;
	int nAddrSize = sizeof(sockaddr_in);
	DWORD dwThread;

	if (WSAStartup(0x202, &wsaData) != 0)
	{
		return 0;
	}

	sockListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sockListen == INVALID_SOCKET)
	{
		cout << "socket error" << endl;
		return 0;
	}

	addrLocal.sin_family = AF_INET;
	addrLocal.sin_port = htons(PORT);
	inet_pton(AF_INET, "192.168.3.70", &addrLocal.sin_addr);
	if (bind(sockListen, (sockaddr *)&addrLocal, nAddrSize) == SOCKET_ERROR)
	{
		cout << "bind error" << endl;;
		return 0;
	}

	if (listen(sockListen, 8) == SOCKET_ERROR)
	{
		cout << "listen error" << endl;
		return 0;
	}

	CreateThread(NULL, 0, WorkerThread1, NULL, 0, &dwThread);

	char szIP[24];
	while (true)
	{
		sockCli = accept(sockListen, (sockaddr *)&addrCli, &nAddrSize);
		inet_ntop(AF_INET, &addrCli.sin_addr, szIP, 24);
		cout << "accept client:" << szIP << endl;

		g_arrSocketCli[g_nTotalConn1++] = sockCli;
	}
	return 0;
}