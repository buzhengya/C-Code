#pragma once
#include<iostream>
#include"define.h"
#include<string.h>

// Q1 : how to deal exception ? A1 : throw ex! A2 : do not care!
class CString
{
	friend CString operator+(const CString& oStr1,const CString& oStr2);

public:
	//构造函数 赋值操作
	CString();
	CString(const char *);
	CString(const CString &ostr);
	CString& operator=(const CString& ostr);
	~CString();

	CString& operator=(char pcStr);
	CString& operator=(const char * pcStr);

	// element access 重载访问运算符 指针取值
	char& operator[](uint32 nPos);
	const char& operator[](uint32 nPos)const;
	char& At(uint32 nPos);
	const char& At(uint32 nPos)const;
	char& Front();
	const char& Front()const;
	char& Back();
	const char& Back()const;

	// modify 增加操作 (重设大小 + 增量赋值)
	CString& operator+=(const char * pszData);
	CString& operator+=(const CString& oStr);
	CString& operator+=(const char szC);

	CString& Append(const CString& oStr);
	CString& Append(const CString& oStr, uint32 nPos, uint32 nSize);
	CString& Append(const char * pszData);
	CString& Append(const char *pszData, uint32 nSize);
	CString& Append(uint32 nSize, char szC);

	void PushBack(char szC);

	void PopBack();

	void Copy(char* pOutput, uint32 nPos, uint32 nLen);

	CString& Assign(const CString& oStr);
	CString& Assign(const CString& oStr, uint32 nPos, uint32 nSize);
	CString& Assign(const char * pszData);
	CString& Assign(const char * pszData, uint32 nSize);
	CString& Assign(uint32 nSize, char szC);

	CString& Insert(uint32 nPos, const CString& oStr);
	CString& Insert(uint32 nPos, const CString& oStr, uint32 nSt, uint32 nSize);
	CString& Insert(uint32 nPos, const char * pszData);
	CString& Insert(uint32 nPos, const char * pszData, uint32 nSize);
	CString& Insert(uint32 nPos, uint32 nSize, char szC);

	CString& Erase(uint32 nPos = 0, uint32 nLen = npos);
	CString& Swap(CString& oStr);

	CString& Replace(uint32 nSt, uint32 nLen, const CString& oStr);
	CString& Replace(uint32 nSt, uint32 nLen, const CString& oStr, uint32 nPos, uint32 nSize);
	CString& Replace(uint32 nSt, uint32 nLen, const char * pszData);
	CString& Replace(uint32 nSt, uint32 nLen, const char * pszData, uint32 nSize);
	CString& Replace(uint32 nSt, uint32 nLen, uint32 nSize, char szC);

	uint32	Find(const CString& oStr, uint32 nPos = 0)const;
	uint32	Find(const char* pszData, uint32 nPos = 0)const;
	uint32	Find(const char* pszData, uint32 nLen, uint32 nPos = 0)const;
	uint32	Find(char c, uint32 nPos = 0)const;

	CString	SubStr(uint32 nPos = 0, uint32 nLen = npos)const;

	int32	Compare(const CString& oStr) const;
	//int32	Compare()

	// capacity
	uint32 Length()const;
	uint32 Size()const;
	uint32 Capacity()const;
	void ReSize(const uint32 nSize, const char szC = '\0'); //如果溢出则重设容量
	void Reserve(const uint32 nCap); //重设容量
	bool Empty() const;
	void Clear();

	// operations
	char* Data()const;

private:
	// 负责赋值操作 考虑传入本身的情况
	void _Assign(const char * pcStr, const uint32 nSize);
	void _Assign(uint32 nSize, char szC);

	// append
	void _Append(const char * pcStr, uint32 nSize);
	void _Append(uint32 nSize, char szC);

	// memory alloc free
	char*	_Alloc(const uint32 nSize);
	void	_Free(char* pData);

	// expand capacity by cap and recovery data
	void	_ReCap(uint32 nCap);

	// expand capacity and do not recovery data
	void	_ExpandCap(uint32 nSize);
	// expand capacity by size and recovery data
	void	_AdjustCap(uint32 nSize);

	inline uint32	_CalCap(const uint32 nSize);

	char *m_pcData;
	uint32 m_nLen;
	uint32 m_nCap;

	static const uint32 npos;
};