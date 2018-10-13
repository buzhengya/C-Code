#include "cstring.h"

static int nCount = 0;

CString::CString()
{
	m_pcData = nullptr;
}

CString::CString(const char * pcStr)
{
	if(pcStr!=nullptr)
	{
		pcStr = new char(strlen(pcStr) + 1);
		strcpy(m_pcData, pcStr);
	}
	else
	{
		m_pcData = nullptr;
	}
}


CString::CString(const CString & oStr)
{
	if(oStr.m_pcData!=nullptr)
	{
		m_pcData = new char(strlen(oStr.m_pcData) + 1);
		strcpy(m_pcData, oStr.m_pcData);
	}
	else
	{
		m_pcData = nullptr;
	}
}


CString& CString::operator=(const CString& oStr)
{

}

CString& CString::operator=(const char *)
{
}

CString::~CString()
{

}

int CString::Length()
{
}