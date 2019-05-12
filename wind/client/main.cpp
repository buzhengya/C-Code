#include "connector.h"
#include "connectctrl.h"
#include "log.h"
#include <winsock2.h>
#include "client_session.h"
#include "winnet.h"

#pragma comment(lib,"ws2_32.lib")

CClientSession * pSession;

void Send() 
{
	if (pSession != nullptr)
	{
		this_thread::sleep_for(chrono::seconds(1));
		pSession->Send("hello world.", 13);
	}
}

int main()
{
	WSAData wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);
	CStorage::Instance()->Init(1024 * 100, "../log/", "client_");
	EXLOG_DEBUG << "test";
	CConnectCtrl::Instance()->Init();
	CConnector oConnector;
	pSession = new CClientSession();
	oConnector.SetBufferSize(1024, 1024);
	oConnector.SetSession(pSession);
	oConnector.Connect("192.168.1.6", 8089);
	CNetWin::Instance()->Init(1024);

	thread t(Send);
	while (true)
	{
		CNetWin::Instance()->Run(50);
		CWheelTimerMgr::Instance()->Run();
		this_thread::sleep_for(chrono::milliseconds(1));
	}
}