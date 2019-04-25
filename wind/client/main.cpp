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
	while (true)
	{
		oConnector.Connect("10.93.186.63", 8089);
		this_thread::sleep_for(chrono::seconds(3));
	}
}