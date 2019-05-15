#include "listener.h"
#include "log.h"
#include "winnet.h"
#include "server_factory.h"
#include "wheel_timer.h"

int main()
{
	WSAData wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);
	CStorage::Instance()->Init(1024 * 100, "../log/", "server_");
	EXLOG_DEBUG << "test";
	CIocpCtrl::Instance()->Init();
	CCPListener oListener("192.168.1.6",8089);
	oListener.SetSessionFactory(new CServerFactory());
	oListener.Start(1024 * 1024, 1024 * 1024);
	
	CNetWin::Instance()->Init(1024);
	while (true)
	{
		CNetWin::Instance()->Run(50);
		CWheelTimerMgr::Instance()->Run();
		this_thread::sleep_for(chrono::milliseconds(1));
	}
}