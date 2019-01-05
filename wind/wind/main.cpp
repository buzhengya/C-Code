#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <time.h>
#include<iostream>
#include<sys/timeb.h>
#pragma comment(lib,"ws2_32.lib")
using namespace std;

int main()
{
	time_t t = time(nullptr);
	std::cout << t;
	timeval tv;
	return 0;
}