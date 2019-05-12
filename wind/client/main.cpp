#include "connector.h"
#include "connectctrl.h"
#include "log.h"
#include <winsock2.h>
#include "client_session.h"
#include "winnet.h"

#pragma comment(lib,"ws2_32.lib")

int main()
{
	WSAData wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);
	CStorage::Instance()->Init(1024 * 100, "../log/", "client_");
	EXLOG_DEBUG << "test";
	CConnectCtrl::Instance()->Init();
	CConnector oConnector;
	CClientSession * pSession = new CClientSession();
	oConnector.SetBufferSize(1024, 1024);
	oConnector.SetSession(pSession);
	oConnector.Connect("192.168.1.6", 8089);
	CNetWin::Instance()->Init(1024);

	while (true)
	{
		CNetWin::Instance()->Run(50);
		CWheelTimerMgr::Instance()->Run();
		pSession->Send("hello world.", 13);
		this_thread::sleep_for(chrono::milliseconds(100));
	}
}