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

	// element access 重载访问运算符
	char& operator[](uint32 nPos);
	const char& operator[](uint32 nPos)const;


	//析构函数
	~CString();

	// capacity
	uint32 Length()const;
	uint32 Size()const;
	uint32 Capacity()const;
	//用于赋值和合并函数 字符串的堆内存扩大至指定大小 并且把原先字符串内容复制到新的内存堆中 
	void ReSize(uint32 nSize = 0);
	bool Empty() const;
	void Clear();

	// operations
	char* Data()const;

private:
	// 负责赋值操作 考虑传入本身的情况
	void _Assign(const char * pcStr, const uint32 nSize);

	// memory alloc free
	char* _Alloc(const uint32 nSize);
	void _Free(char* pData);

	inline uint32 _CalCap(const uint32 nSize);

	inline uint32 _Size() const;
	inline uint32 _Capacity() const;

	char *m_pcData;
	uint32 m_nLen;
	uint32 m_nCap;
};