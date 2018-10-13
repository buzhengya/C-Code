#include "cstring.h"

static int nCount = 0;

CString::CString()
{
	m_pcData = nullptr;
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
	//�Ƚ���ֵ�ڴ渴�� ���� s=s���������
	char * pTmp = nullptr;
	if(oStr.m_pcData!=nullptr)
	{
		int nLen = strlen(oStr.m_pcData);
		pTmp = new char(nLen + 1);
		strncpy(pTmp, oStr.m_pcData, nLen + 1);
	}

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

size_t CString::Length()const
{
	return strlen(m_pcData);
}

size_t CString::Size()const
{
	return strlen(m_pcData);
}

void CString::Assign(const char * pcStr)
{
	if (pcStr != nullptr)
	{
		int nLen = strlen(pcStr);
		m_pcData = new char(nLen + 1);
		strncpy(m_pcData, pcStr, nLen + 1);
	}
	else
	{
		m_pcData = nullptr;
	}
}
