#include "connector.h"
#include "connectctrl.h"
#include "log.h"
#include <winsock2.h>

#pragma comment(lib,"ws2_32.lib")

int main()
{
	WSAData wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);
	CStorage::Instance()->Init(1024 * 100, "../log/", "client_");
	EXLOG_DEBUG << "test";
	CConnectCtrl::Instance()->Init();
	CConnector oConnector;
	oConnector.Connect("192.168.3.70", 8888);
	this_thread::sleep_for(chrono::minutes(10));
}