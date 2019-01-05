#ifndef _OVERLAP1_H_
#define _OVERLAP1_H_
#include <WinSock2.h>
#include<WS2tcpip.h>
#include <iostream>
#include "server_define.h"
#pragma comment(lib, "ws2_32.lib")

using namespace std;
DWORD WINAPI WorkerThread3(LPVOID);
void Cleanup3(int);
int Overlap1();
#endif
