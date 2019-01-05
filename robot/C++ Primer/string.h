#pragma once
#include<iostream>

class CString
{
public:
	//���캯��
	CString();
	CString(const char *);
	CString(size_t nLen,char c);
	CString(const CString &oStr, size_t n);
	
	//�������캯��
	CString(const CString &ostr);

	//������ֵ�����
	CString& operator=(const CString& ostr);

	CString& operator=(const char * pcStr);
	//��������
	~CString();

	//others
	int Length();

	
private:
	size_t* pnCount;
	char *pcStr;
	int nLen;
};