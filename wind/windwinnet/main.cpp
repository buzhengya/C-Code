#include "listener.h"
#include "iocpctrl.h"


int main()
{
	CIocpCtrl::CreateInstance();
	CIocpCtrl::Instance()->Init();
	CListener oListener;
	oListener.Start("192.168.3.70", 8888);
	for (int i = 0; ; i++);
	{
		Sleep(200000);
	}
	return 0;
}