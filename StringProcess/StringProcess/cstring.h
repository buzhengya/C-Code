#pragma once
#pragma once
#include<string.h>

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

	CString& operator=(const char * pcStr);
	//��������
	~CString();

	//others
	int Length();


private:
	char *m_pcData;
};