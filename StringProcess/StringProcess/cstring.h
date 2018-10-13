#pragma once
#pragma once
#include<string.h>
#include<iostream>

class CString
{
public:
	//构造函数
	CString();
	CString(const char *);
	CString(const CString &oStr, size_t n);

	//拷贝构造函数
	CString(const CString &ostr);

	//拷贝赋值运算符
	CString& operator=(const CString& ostr);

	CString& operator=(char pcStr);//有了该重载函数后 该形式将不合法 CString oStr=NULL; 有多个重载函数
	CString& operator=(const char * pcStr);

	//


	//析构函数
	~CString();

	//others
	size_t Length()const;
	size_t Size()const;

private:
	void Assign(const char * pcStr);
	char *m_pcData;
};