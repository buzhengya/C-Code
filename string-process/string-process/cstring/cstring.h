#pragma once
//#include<iostream>
#include "../common/define.h"
#include <string.h>

#include <istream>
#include <ostream>

class IMemPoolMgr;

uint32	StrLen(const char* pszData);
// Q1 : how to deal exception ? A1 : throw ex! A2 : do not care!
// A3 : library should throw exception if possible
class CString
{
	// operator+ use && for (str1 + str2 + str3...)
	friend CString operator+(const CString& oStr1,const CString& oStr2);
	friend CString operator+(const CString&& oStr1,const CString& oStr2);
	friend CString operator+(const CString& oStr1,const CString&& oStr2);

	friend CString operator+(const CString& oStr,const char* pszData);
	friend CString operator+(const CString&& oStr,const char* pszData);
	friend CString operator+(const char* pszData,const CString& oStr);
	friend CString operator+(const char* pszData,const CString& oStr);

	friend CString operator+(const CString& oStr, char szC);
	friend CString operator+(const CString&& oStr, char szC);
	friend CString operator+(char szC,const CString& oStr);
	friend CString operator+(char szC,const CString&& oStr);

	friend bool	operator==(const CString& oStr1, const CString& oStr2);
	friend bool operator==(const CString & oStr, const char* pszData);
	friend bool operator==(const char* pszData, const CString& oStr);

	friend bool	operator>(const CString& oStr1, const CString& oStr2);
	friend bool operator>(const CString & oStr, const char* pszData);
	friend bool operator>(const char* pszData, const CString& oStr);

	friend bool	operator<(const CString& oStr1, const CString& oStr2);
	friend bool operator<(const CString & oStr, const char* pszData);
	friend bool operator<(const char* pszData, const CString& oStr);

	friend bool	operator!=(const CString& oStr1, const CString& oStr2);
	friend bool operator!=(const CString & oStr, const char* pszData);
	friend bool operator!=(const char* pszData, const CString& oStr);

	friend bool	operator>=(const CString& oStr1, const CString& oStr2);
	friend bool operator>=(const CString & oStr, const char* pszData);
	friend bool operator>=(const char* pszData, const CString& oStr);

	friend bool	operator<=(const CString& oStr1, const CString& oStr2);
	friend bool operator<=(const CString & oStr, const char* pszData);
	friend bool operator<=(const char* pszData, const CString& oStr);

	friend void	Swap(CString& oStr1, CString& oStr2);

	// 判断是否结束字符读取的函数指针设计的不够好 考虑更好的设计
	friend std::istream& operator>>(std::istream& is, CString& oStr);
	friend std::ostream& operator<<(std::ostream& os, CString& oStr);

	friend std::istream& GetLine(std::istream& is, CString& oStr, char delim);
	friend std::istream& GetLine(std::istream&& is, CString& oStr, char delim);
	friend std::istream& GetLine(std::istream& is, CString& oStr);
	friend std::istream& GetLine(std::istream&& is, CString& oStr);

public:
	//构造函数 赋值操作
	CString();
	CString(const char *);
	CString(const char* pszData, uint32 nLen);
	CString(const CString& oStr);
	CString(const CString& oStr, uint32 nPos, uint32 nLen = npos);
	CString(uint32 nSize, char szC);
	CString& operator=(const CString& oStr) noexcept;
	CString& operator=(CString&& oStr) noexcept;
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

	CString& Assign(CString&& oStr); // 右值引用
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
	CString& Swap(CString&& oStr);

	CString& Replace(uint32 nSt, uint32 nLen, const CString& oStr);
	CString& Replace(uint32 nSt, uint32 nLen, const CString& oStr, uint32 nPos, uint32 nSize);
	CString& Replace(uint32 nSt, uint32 nLen, const char * pszData);
	CString& Replace(uint32 nSt, uint32 nLen, const char * pszData, uint32 nSize);
	CString& Replace(uint32 nSt, uint32 nLen, uint32 nSize, char szC);

	uint32	Find(const CString& oStr, uint32 nPos = 0)const;
	uint32	Find(const char* pszData, uint32 nPos = 0)const;
	uint32	Find(const char* pszData, uint32 nLen, uint32 nPos = 0)const;
	uint32	Find(char c, uint32 nPos = 0)const;

	uint32	RFind(const CString& oStr, uint32 nPos = 0)const;
	uint32	RFind(const char* pszData, uint32 nPos = 0)const;
	uint32	RFind(const char* pszData, uint32 nLen, uint32 nPos = 0)const;
	uint32	RFind(char c, uint32 nPos = 0)const;

	CString	SubStr(uint32 nPos = 0, uint32 nLen = npos)const;

	int32	Compare(const CString& oStr) const;
	int32	Compare(uint32 nPos, uint32 nLen, const CString& oStr) const;
	int32	Compare(uint32 nPos, uint32 nLen, const CString& oStr, uint32 nSubPos, uint32 nSubLen) const;
	int32	Compare(const char* pszData) const;
	int32	Compare(uint32 nPos, uint32 nLen, const char * pszData) const;
	int32	Compare(uint32 nPos, uint32 nLen, const char * pszData, uint32 nSubPos, uint32 nSubLen) const;

	// capacity
	uint32 Length()const;
	uint32 Size()const;
	uint32 Capacity()const;
	void ReSize(const uint32 nSize, const char szC = '\0'); //如果溢出则重设容量
	void Reserve(const uint32 nCap); //重设容量
	bool Empty() const;
	void Clear();

	// operations
	const char* Data()const;

private:
	// 负责赋值操作 考虑传入本身的情况
	void _Init();
	void _Assign(const char * pcStr, const uint32 nSize);
	void _Assign(uint32 nSize, char szC);
	void _Assign(CString& oStr);  // for 右值引用

	// append
	void _Append(const char * pcStr, uint32 nSize);
	void _Append(uint32 nSize, char szC);

	// memory alloc free
	char*	_Alloc(const uint32 nSize);
	void	_Free(char* pData, uint32 nSize);

	// expand capacity by cap and recovery data
	void	_ReCap(uint32 nCap);

	// expand capacity and do not recovery data
	void	_ExpandCap(uint32 nSize);
	// expand capacity by size and recovery data
	void	_AdjustCap(uint32 nSize);

	// expand for insert  expand nSize from nPos -> nPos + nSize
	void	_ExpandMid(uint32 nPos, uint32 nSize);

	// [nInit, m_nLen - 1] ==> [nRet, m_nLen - 1 + nRet - nInit]
	void	_Move(uint32 nInit, uint32 nRet);

	void	_Replace(uint32 nPos, uint32 nLen, const char* pszData, uint32 nSize);
	void	_Replace(uint32 nPos, uint32 nLen, uint32 nSize, char szC);

	void	_Insert(uint32 nPos, const char* pszData, uint32 nLen);
	void	_Insert(uint32 nPos, uint32 nSize, char szC);

	uint32	_Find(uint32 nPos, const char* pszData, uint32 nSize) const;
	uint32	_RFind(uint32 nPos, const char* pszData, uint32 nSize) const;

	int32	_Compare(const char* pszData1, uint32 nSize1, const char* pszData2, uint32 nSize2) const;

	inline uint32	_CalCap(const uint32 nSize);

	char *m_pcData = nullptr;
	uint32 m_nLen = 0;
	uint32 m_nCap = 0;

	static const uint32 npos;
	static IMemPoolMgr*	s_pMemPool;
};