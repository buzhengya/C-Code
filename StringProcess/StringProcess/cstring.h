#pragma once
#pragma once
#include<string.h>
#include<iostream>

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
	size_t Length()const;
	size_t Size()const;

private:
	void Assign(const char * pcStr);
	char *m_pcData;
};