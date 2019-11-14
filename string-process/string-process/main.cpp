#include "cstring/cstring.h"
#include "cstring/string_test.h"
#include<string>
#include<iostream>
#include<fstream>
#include "log/log.h"

using namespace std;
int main()
{
	CString oStr;
	CString* p = (CString*)malloc(sizeof CString);
	p = new(p)CString();
	TestOperator(10);
}
