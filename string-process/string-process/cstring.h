#pragma once
#include<iostream>
#include"define.h"
#include<string.h>

class CString
{
	friend CString operator+(const CString& oStr1,const CString& oStr2);

public:
	//���캯��
	CString();
	CString(const char *);

	//�������캯��
	CString(const CString &ostr);

	//������ֵ�����
	CString& operator=(const CString& ostr);

	CString& operator=(char pcStr);//���˸����غ����� ����ʽ�����Ϸ� CString oStr=NULL; �ж�����غ���
	CString& operator=(const char * pcStr);

	//���ط��������
	char& operator[](uint32 nPos);
	const char& operator[](uint32 nPos)const;


	//��������
	~CString();

	//others
	uint32 Length()const;
	uint32 Size()const;
	uint32 Capacity()const;
	//���ڸ�ֵ�ͺϲ����� �ַ����Ķ��ڴ�������ָ����С ���Ұ�ԭ���ַ������ݸ��Ƶ��µ��ڴ���� 
	void Reserve(uint32 nSize = 0);
	bool Empty();
	void Clear();

private:
	//������ʱ �����ڴ���䲢���ַ����������ڴ�
	void Assign(const char * pcStr);

	char *m_pcData;
	uint32 m_nLen;
	uint32 m_nCap;
};