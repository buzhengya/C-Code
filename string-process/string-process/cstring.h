#pragma once
#include<iostream>
#include"define.h"
#include<string.h>

class CString
{
	friend CString operator+(const CString& oStr1,const CString& oStr2);

public:
	//构造函数
	CString();
	CString(const char *);

	//拷贝构造函数
	CString(const CString &ostr);

	//拷贝赋值运算符
	CString& operator=(const CString& ostr);

	CString& operator=(char pcStr);//有了该重载函数后 该形式将不合法 CString oStr=NULL; 有多个重载函数
	CString& operator=(const char * pcStr);

	//重载访问运算符
	char& operator[](uint32 nPos);
	const char& operator[](uint32 nPos)const;


	//析构函数
	~CString();

	//others
	uint32 Length()const;
	uint32 Size()const;
	uint32 Capacity()const;
	//用于赋值和合并函数 字符串的堆内存扩大至指定大小 并且把原先字符串内容复制到新的内存堆中 
	void Reserve(uint32 nSize = 0);
	bool Empty();
	void Clear();

private:
	//负责构造时 进行内存分配并将字符串赋给该内存
	void Assign(const char * pcStr);

	char *m_pcData;
	uint32 m_nLen;
	uint32 m_nCap;
};