#include "cstring/cstring.h"
#include "cstring/string_test.h"
#include<string>
#include<iostream>
#include<fstream>
#include "log/log.h"

using namespace std;

class CBase
{
public:
	CBase(int32 nVal3): m_nVal3(nVal3) {}

	virtual void Func3() { cout << "Func3" << endl; }
	virtual ~CBase() {}
	virtual void Func1() { cout << "Base Func1" << endl; }

private:
	int32 m_nVal3;
};

class CTest : public CBase
{
public:
	CTest(int32 nVal1, int32 nVal2, int32 nVal3) : m_nVal1(nVal1), m_nVal2(nVal2), CBase(nVal3) {}

	virtual void Func1() { cout << "Func1 : " << endl; }
	virtual void Func2() { cout << "Func2 : " << endl; }
	virtual ~CTest() {}
private:
	int32 m_nVal1;
	int32 m_nVal2;
};

using namespace std;
int main()
{
	CBase* pBase = new CTest(1, 2, 3);
	typedef void(*Fun)();

	int* pVir = (int*)(pBase);


	for (int32 i = 0; i < 4; ++i)
	{
		cout << "addr" << i << " : " << *(pVir + i) << endl;
	}

	Fun pFun = nullptr;
	for (int32 i = 0; i < 4; ++i)
	{
		if (i == 1) continue;
		pFun = (Fun)*((int*)(*pVir) + i);
		pFun();
	}
	return 0;
}
