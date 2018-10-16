#include "cstring.h"


CString::CString()
{
	m_pcData = nullptr;
	m_nLen = 0;
	m_nCap = 0;
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
	if (this != &oStr)
	{

	}
	return *this;
}

CString & CString::operator=(char pcStr)
{
	m_pcData[0] = pcStr;
	m_pcData[1] = '\0';
	return *this;
}

CString& CString::operator=(const char *pcStr)
{
	
	return *this;
}

char& CString::operator[](uint32 nPos)
{
	return m_pcData[nPos];
}

const char & CString::operator[](uint32 nPos) const
{
	// TODO: 在此处插入 return 语句
	return NULL;
}

CString::~CString()
{
	if (m_pcData != nullptr)
	{
		delete m_pcData;
	}
}

uint32 CString::Length()const
{
	return strlen(m_pcData);
}

uint32 CString::Size()const
{
	return strlen(m_pcData);
}

uint32 CString::Capacity() const
{
	return m_nCap;
}


void CString::Reserve(uint32 nSize)
{
}

bool CString::Empty()
{
	return m_nLen == 0;
}

void CString::Clear()
{
}

void CString::Assign(const char * pcStr)
{
	m_nLen = strlen(pcStr);
	m_nCap = 2;
	while (m_nCap <= m_nLen) m_nCap = m_nCap << 1;
	if (m_pcData != nullptr)
	{
		delete m_pcData;
	}
	m_pcData = new char(m_nCap);
	strncpy(m_pcData, pcStr, m_nLen);
	m_pcData[m_nLen] = '\0';
}

CString operator+(const CString & oStr1, const CString & oStr2)
{
	return CString();
}
