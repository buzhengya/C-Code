#include "listener.h"
#include "log.h"

int main()
{
	WSAData wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);
	CStorage::Instance()->Init(1024 * 100, "../log/", "server_");
	EXLOG_DEBUG << "test";
	CIocpCtrl::Instance()->Init();
	CCPListener oListener("10.93.186.63",8089);
	oListener.Start();
	this_thread::sleep_for(chrono::seconds(10));
	CIocpCtrl::Instance()->Fini();
	this_thread::sleep_for(chrono::minutes(10));
}