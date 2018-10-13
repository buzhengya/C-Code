#include<iostream>
#include<string>
#include<vector>
#include<stdarg.h>
#include "string.h"


using namespace std;
CString oGlobal;
CString TestCopy(CString oArg)
{
	cout << "0" << endl;
	CString oLocal = oArg;
	cout << "1" << endl;
	CString *poHeap = new CString(oGlobal);
	cout << "2" << endl;
	*poHeap = oLocal;
	cout << "3" << endl;
	CString arrStr[4] = { oLocal,*poHeap };
	cout << "4" << endl;
	return *poHeap;
}

void F(CString oStr)
{
	cout << oStr.Length() << endl;
}

void TestPtr(CString oArg)
{
	CString oStr1;
	oStr1 = oArg;
	cout << "1" << endl;
}
int main()
{
	TestPtr(CString());
	return 0;
}