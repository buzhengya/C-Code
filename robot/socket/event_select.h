#ifndef _EVENT_SELECT_H_
#define _EVENT_SELECT_H_
#include<WinSock2.h>
#include<iostream>
#include<WS2tcpip.h>
#include "server_define.h"
#pragma comment(lib, "ws2_32.lib")

using namespace std;

DWORD WINAPI WorkerThread2(LPVOID IpParameter);
void Cleanup(int nIndex);
int EventSelect();
#endif
