#pragma once
#include<iostream>
#include"define.h"
#include<string.h>

// Q1 : how to deal exception ? A1 : throw ex! A2 : do not care!
class CString
{
	friend CString operator+(const CString& oStr1,const CString& oStr2);

public:
	//���캯�� ��ֵ����
	CString();
	CString(const char *);
	CString(const CString &ostr);
	CString& operator=(const CString& ostr);
	~CString();

	CString& operator=(char pcStr);
	CString& operator=(const char * pcStr);

	// element access ���ط�������� ָ��ȡֵ
	char& operator[](uint32 nPos);
	const char& operator[](uint32 nPos)const;
	char& At(uint32 nPos);
	const char& At(uint32 nPos)const;
	char& Front();
	const char& Front()const;
	char& Back();
	const char& Back()const;

	// modify ���Ӳ��� (�����С + ������ֵ)
	CString& operator+=(const char * pszData);
	CString& operator+=(const CString& oStr);
	CString& operator+=(const char szC);

	CString& Append(const CString& oStr);
	CString& Append(const CString& oStr, uint32 nPos, uint32 nSize);
	CString& Append(const char * pszData);
	CString& Append(const char *pszData, uint32 nSize);
	CString& Append(uint32 nSize, char szC);

	void PushBack(char szC);

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

	// capacity
	uint32 Length()const;
	uint32 Size()const;
	uint32 Capacity()const;
	void ReSize(const uint32 nSize, const char szC = '\0'); //����������������
	void Reserve(const uint32 nCap); //��������
	bool Empty() const;
	void Clear();

	// operations
	char* Data()const;

private:
	// ����ֵ���� ���Ǵ��뱾������
	void _Assign(const char * pcStr, const uint32 nSize);
	void _Assign(uint32 nSize, char szC);

	// memory alloc free
	char*	_Alloc(const uint32 nSize);
	void	_Free(char* pData);

	void	_ReCap(uint32 nCap);

	inline uint32	_CalCap(const uint32 nSize);

	inline uint32	_Size() const;
	inline uint32	_Capacity() const;

	char *m_pcData;
	uint32 m_nLen;
	uint32 m_nCap;
};