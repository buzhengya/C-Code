#include "listener.h"
#include "log.h"
#include "winnet.h"
#include "server_factory.h"
#include "wheel_timer.h"
#include <algorithm>

class CTest
{
public:
	CTest(int nOne, int nTwo):m_nOne(nOne), m_nTwo(nTwo) {}
	~CTest() {}
	
	bool operator<(const CTest & oTest) { return m_nOne > oTest.m_nOne || (m_nOne == oTest.m_nOne && m_nTwo < oTest.m_nTwo); }

	void	Print() { std::cout << m_nOne << "   " << m_nTwo; }

	int		m_nOne;
	int		m_nTwo;
};

int main()
{
	vector<CTest> vecTest;
	for (int i = 0; i <= 100; i++)
	{
		vecTest.push_back(CTest(rand() % 100, rand() % 100));
	}

	sort(vecTest.begin(), vecTest.end());

	for (auto & it : vecTest)
	{
		it.Print();
	}

	int nTest;
	memset(&nTest, 0, sizeof(nTest) + 1024);

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