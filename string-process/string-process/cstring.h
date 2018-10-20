#pragma once
#include<iostream>
#include"define.h"
#include<string.h>

//����1�ַ���ĩβ��'\0'����ɶ� �����ܿ�д ����ν���� ���ɶ�������

class CString
{
	//����+�����
	friend CString operator+(const CString& oStr1, const CString& oStr2);
	friend CString operator+(const CString& oStr1, const char* pcStr);
	friend CString operator+(const char* pcStr, const CString& oStr);
	friend CString operator+(const CString& oStr, char cStr);
	friend CString operator+(char cStr, const CString& oStr);

	void Swap(CString& oStr1, CString& ostr2);

	//TODO �޸ĳ��Լ���IO��
	std::istream GetLine(std::istream oIs, CString &ostr);

	std::istream& operator>>(CString& oStr);

	std::ostream& operator<<(CString& oStr);

public:
	//���캯��
	CString();
	CString(const char *);

	//�������캯��
	CString(const CString &ostr);

	//������ֵ�����
	CString& operator=(const CString& ostr);

	//���˸����غ����� ����ʽ�����Ϸ� CString oStr=NULL; �ж�����غ���
	CString& operator=(char pcStr);
	CString& operator=(const char * pcStr);

	//���ط��������
	char& operator[](uint32 nPos);
	const char& operator[](uint32 nPos)const;

	//���رȽ������
	int Compare(const char *pcStr)const;
	int Compare(const CString& oStr)const;
	bool operator==(const char *pcStr)const;
	bool operator==(const CString& oStr)const;

	//��������
	~CString();

	//others
	uint32 Length()const;
	uint32 Size()const;
	uint32 Capacity()const;

	//���ڸ�ֵ�ͺϲ������Ļ����� 
	void Reserve(uint32 nSize = 0);
	bool Empty();
	void Clear();
	

	//�ַ�������
	const char* CStr2Char()const;


	//�����Ƿ�������ַ���
	uint32 Find(const char *pcStr);
	uint32 Find(const CString& oStr);

	//�ж��Ƿ�͸�������ʽƥ��
	bool Match(const char *pcStr);
	CString SubStr(uint32 nPos, uint32 nLen)const;

	//�޸��ַ���
	CString& Insert(uint32 nPos, const char* pcStr);
	CString& Insert(uint32 nPos, const CString& oStr);
	CString& Insert(uint32 nPos, char cStr, uint32 nLen = 1);

	CString& Erase(uint32 nPos,uint32 nLen);

	CString& Replace(uint32 nPos, uint32 nLen, const CString& oStr);
	CString& Replace(uint32 nPos, uint32 nLen, const char* pcStr);
	CString& Replace(uint32 nPos, uint32 nLen, char cStr, uint32 nCount);

	void Swap(CString& oStr);

private:
	//�ַ������ȼ���
	uint32 CalLen(const char* pcStr);

	//�������ó��ȵ�nLen��Ҫ��������ڴ�
	uint32 CalMem(uint32 nLen);

	//�������ָ����С���ڴ�
	char* Alloc(int nLen);

	//������ʱ �����ڴ���䲢���ַ����������ڴ�
	void Assign(const char * pcStr);
	void Assign(const char * pcStr,uint32 nLen);

	//���ַ������Ƹ�
	void Copy(const char* pcStr);
	void Copy(const char* pcStr, uint32 nLen);

	char *m_pcData;
	uint32 m_nLen;
	uint32 m_nCap;
};