#include "event_select.h"

int g_nTotalConn2 = 0;
SOCKET g_arrSockCli[MAXIMUM_WAIT_OBJECTS];
WSAEVENT g_arrEventCli[MAXIMUM_WAIT_OBJECTS];


DWORD WINAPI WorkerThread2(LPVOID IpParameter)
{
	int nRet, nIndex;
	WSANETWORKEVENTS NetworkEvents;
	char szMessage[MSG_SIZE];

	while (true)
	{
		nRet = WSAWaitForMultipleEvents(g_nTotalConn2, g_arrEventCli, false, 1000, false);
		if (nRet == WSA_WAIT_FAILED || nRet==WSA_WAIT_TIMEOUT)
		{
			Sleep(100);
			continue;
		}

		nIndex = nRet - WSA_WAIT_EVENT_0;
		WSAEnumNetworkEvents(g_arrSockCli[nIndex], g_arrEventCli[nIndex], &NetworkEvents);

		if (NetworkEvents.lNetworkEvents &FD_READ)
		{
			nRet = recv(g_arrSockCli[nIndex], szMessage, MSG_SIZE, 0);
			if (nRet == 0 || (nRet == SOCKET_ERROR && WSAGetLastError() == WSAECONNRESET))
			{
				Cleanup(nIndex);
			}
			else
			{
				szMessage[nRet] = 0;
				cout << szMessage << endl;
				send(g_arrSockCli[nIndex], szMessage, strlen(szMessage), 0);
			}
		}
		if (NetworkEvents.lNetworkEvents & FD_WRITE)
		{
			Cleanup(nIndex);
		}
	}
	return 0;
}

void Cleanup(int nIndex)
{
	closesocket(g_arrSockCli[nIndex]);
	WSACloseEvent(g_arrEventCli[nIndex]);

	if (nIndex < g_nTotalConn2 - 1)
	{
		g_arrSockCli[nIndex] = g_arrSockCli[g_nTotalConn2 - 1];
		g_arrEventCli[nIndex] = g_arrEventCli[g_nTotalConn2 - 1];
	}
	g_nTotalConn2--;
}

int EventSelect()
{
	WSADATA wsaData;
	SOCKET sockListen, sockCli;
	sockaddr_in addrLocal, addrCli;
	DWORD dwThreadId;
	int nAddrSize = sizeof(sockaddr_in);

	if (WSAStartup(0x202, &wsaData) != 0)
	{
		cout << "WSAStartup error" << endl;
		return 0;
	}

	sockListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sockListen == INVALID_SOCKET)
	{
		cout << "socket error!" << endl;
		return 0;
	}

	addrLocal.sin_family = AF_INET;
	addrLocal.sin_port = htons(8888);
	inet_pton(AF_INET, "192.168.3.70", &addrLocal.sin_addr);
	if (bind(sockListen, (sockaddr *)&addrLocal, nAddrSize) == SOCKET_ERROR)
	{
		cout << "bind error!" << endl;
		return 0;
	}

	if (listen(sockListen, 8) == SOCKET_ERROR)
	{
		cout << "listen error!" << endl;
		return 0;
	}

	CreateThread(NULL, 0, WorkerThread2, NULL, 0, &dwThreadId);

	char szCliIp[30];
	for (int i = 0;; i++)
	{
		sockCli = accept(sockListen, (sockaddr *)&addrCli, &nAddrSize);
		if (sockCli == INVALID_SOCKET)
		{
			cout << "accept error!" << endl;
			continue;
		}

		inet_ntop(AF_INET, &addrCli.sin_addr, szCliIp, 30);
		cout << i << " accept client " << szCliIp << ":" << ntohs(addrCli.sin_port) << endl;

		if (g_nTotalConn2 >= MAXIMUM_WAIT_OBJECTS)
		{
			closesocket(sockCli);
			continue;
		}
		g_arrSockCli[g_nTotalConn2] = sockCli;
		g_arrEventCli[g_nTotalConn2] = WSACreateEvent();
		WSAEventSelect(g_arrSockCli[g_nTotalConn2], g_arrEventCli[g_nTotalConn2], FD_READ | FD_CLOSE);
		g_nTotalConn2++;
	}

	WSACleanup();
}