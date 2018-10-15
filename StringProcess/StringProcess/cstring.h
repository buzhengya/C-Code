#pragma once
#pragma once
#include<string.h>
#include<iostream>
#include"define.h"


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
	uint32 Length()const;
	uint32 Size()const;

private:
	//负责内存分配 将pcStr指向的字符串复制给m_pcData m_pcData原先指向的数据不做处理
	void Assign(const char * pcStr);
	char *m_pcData;
	uint32 m_nLen;
	uint32 m_nCap;
};