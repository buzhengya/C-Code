#ifndef _SELECT_DEFINE_H_
#define _SELECT_DEFINE_H_
#include<WinSock2.h>
#include<iostream>
#include<WS2tcpip.h>
#include "server_define.h"
#pragma comment(lib, "ws2_32.lib")

using namespace std;

DWORD WINAPI WorkerThread1(LPVOID IpParameter);
int Select();

#endif // _SELECT_DEFINE_H_
