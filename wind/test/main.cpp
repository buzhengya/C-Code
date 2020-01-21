#include "log.h"
#include <thread>
#include <chrono>
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
using namespace std;

#define MSG_SIZE 65536

uint32 WorkerThread(LPVOID pParam)
{
	EXLOG_DEBUG << "test";
	Sleep(1000);
	EXLOG_DEBUG << "test";
	return 0;
}

int main()
{
	int nTest;
	memset(&nTest, 0, sizeof(nTest) + 1024);
	CStorage::Instance()->Init(1024 * 1024 * 128, "", "../log/test");
	thread t(WorkerThread, nullptr);
	Sleep(2000);
	t.join();
	return 0;
}