#pragma once
#pragma once
#include<iostream>
#include"define.h"


class CString
{
public:
	//���캯��
	CString();
	CString(const char *);
	CString(const CString &oStr, size_t n);

	//�������캯��
	CString(const CString &ostr);

	//������ֵ�����
	CString& operator=(const CString& ostr);

	CString& operator=(char pcStr);//���˸����غ����� ����ʽ�����Ϸ� CString oStr=NULL; �ж�����غ���
	CString& operator=(const char * pcStr);

	//


	//��������
	~CString();

	//others
	uint32 Length()const;
	uint32 Size()const;

private:
	//������ʱ �����ڴ���䲢���ַ����������ڴ�
	void Assign(const char * pcStr);
	char *m_pcData;
	uint32 m_nLen;
	uint32 m_nCap;
};