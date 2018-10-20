#pragma once
#include<iostream>
#include"define.h"
#include<string.h>

//问题1字符串末尾的'\0'必须可读 但不能可写 这如何解决？ 不可读！！！

class CString
{
	//重载+运算符
	friend CString operator+(const CString& oStr1, const CString& oStr2);
	friend CString operator+(const CString& oStr1, const char* pcStr);
	friend CString operator+(const char* pcStr, const CString& oStr);
	friend CString operator+(const CString& oStr, char cStr);
	friend CString operator+(char cStr, const CString& oStr);

	void Swap(CString& oStr1, CString& ostr2);

	//TODO 修改成自己的IO类
	std::istream GetLine(std::istream oIs, CString &ostr);

	std::istream& operator>>(CString& oStr);

	std::ostream& operator<<(CString& oStr);

public:
	//构造函数
	CString();
	CString(const char *);

	//拷贝构造函数
	CString(const CString &ostr);

	//拷贝赋值运算符
	CString& operator=(const CString& ostr);

	//有了该重载函数后 该形式将不合法 CString oStr=NULL; 有多个重载函数
	CString& operator=(char pcStr);
	CString& operator=(const char * pcStr);

	//重载访问运算符
	char& operator[](uint32 nPos);
	const char& operator[](uint32 nPos)const;

	//重载比较运算符
	int Compare(const char *pcStr)const;
	int Compare(const CString& oStr)const;
	bool operator==(const char *pcStr)const;
	bool operator==(const CString& oStr)const;

	//析构函数
	~CString();

	//others
	uint32 Length()const;
	uint32 Size()const;
	uint32 Capacity()const;

	//用于赋值和合并函数的基函数 
	void Reserve(uint32 nSize = 0);
	bool Empty();
	void Clear();
	

	//字符串处理
	const char* CStr2Char()const;


	//查找是否包含该字符串
	uint32 Find(const char *pcStr);
	uint32 Find(const CString& oStr);

	//判断是否和该正则表达式匹配
	bool Match(const char *pcStr);
	CString SubStr(uint32 nPos, uint32 nLen)const;

	//修改字符串
	CString& Insert(uint32 nPos, const char* pcStr);
	CString& Insert(uint32 nPos, const CString& oStr);
	CString& Insert(uint32 nPos, char cStr, uint32 nLen = 1);

	CString& Erase(uint32 nPos,uint32 nLen);

	CString& Replace(uint32 nPos, uint32 nLen, const CString& oStr);
	CString& Replace(uint32 nPos, uint32 nLen, const char* pcStr);
	CString& Replace(uint32 nPos, uint32 nLen, char cStr, uint32 nCount);

	void Swap(CString& oStr);

private:
	//字符串长度计算
	uint32 CalLen(const char* pcStr);

	//负责计算该长度的nLen需要分配多少内存
	uint32 CalMem(uint32 nLen);

	//负责分配指定大小的内存
	char* Alloc(int nLen);

	//负责构造时 进行内存分配并将字符串赋给该内存
	void Assign(const char * pcStr);
	void Assign(const char * pcStr,uint32 nLen);

	//将字符串复制给
	void Copy(const char* pcStr);
	void Copy(const char* pcStr, uint32 nLen);

	char *m_pcData;
	uint32 m_nLen;
	uint32 m_nCap;
};