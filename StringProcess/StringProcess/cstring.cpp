#include "cstring.h"



CString::CString()
{
	Assign("");
}

CString::CString(const char * pcStr)
{
	Assign(pcStr);
}


CString::CString(const CString & oStr)
{
	Assign(oStr.m_pcData);
}


CString& CString::operator=(const CString& oStr)
{
	//�ȸ���������ԭ�ڴ� ���� s=s���������
	char * pTmp = nullptr;

	//���ԭָ�벻Ϊ�� ��ɾ��ָ����ڴ�
	if(m_pcData!=nullptr)
	{
		delete m_pcData;
	}

	m_pcData = pTmp;
	return *this;
}

CString & CString::operator=(char pcStr)
{
	return CString();
}

CString& CString::operator=(const char *)
{
	
	return *this;
}

CString::~CString()
{
	if(m_pcData!=nullptr)
	{
		delete m_pcData;
	}
}

uint32 CString::Length()const
{
	return 0;
	//return strlen(m_pcData);
}

uint32 CString::Size()const
{
	return 0;
	//return strlen(m_pcData);
}

void CString::Assign(const char * pcStr)
{
	/*uint32 nLen = strlen(pcStr);
	m_nCap = 1;
	while (m_nCap <= nLen) m_nCap = m_nCap << 1;
	if (m_pcData != nullptr)
	{
		delete m_pcData;
	}
	m_pcData = new char(m_nCap);
	strncpy(m_pcData, pcStr,nLen);
	m_pcData[m_nLen] = '\0';*/
}
