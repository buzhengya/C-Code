#pragma once
#include<iostream>

class CString
{
public:
	//构造函数
	CString();
	CString(const char *);
	CString(size_t nLen,char c);
	CString(const CString &oStr, size_t n);
	
	//拷贝构造函数
	CString(const CString &ostr);

	//拷贝赋值运算符
	CString& operator=(const CString& ostr);

	CString& operator=(const char * pcStr);
	//析构函数
	~CString();

	//others
	int Length();

	
private:
	size_t* pnCount;
	char *pcStr;
	int nLen;
};