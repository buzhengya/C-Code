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
	Assign(oStr.CStr2Char());
}


CString& CString::operator=(const CString& oStr)
{
	if (this != &oStr)
	{
		Reserve(oStr.Length());
		Copy(oStr.CStr2Char());
	}
	return *this;
}

CString & CString::operator=(char pcStr)
{
	Reserve(1);
	Copy(&pcStr, 1);
	return *this;
}

CString& CString::operator=(const char *pcStr)
{
	int nLen = CalLen(pcStr);
	Reserve(nLen);
	Copy(pcStr, nLen);
	return *this;
}

char& CString::operator[](uint32 nPos)
{
	if (nPos >= m_nLen)
	{
		throw "access CString char subscript";
	}
	return m_pcData[nPos];
}

const char & CString::operator[](uint32 nPos) const
{
	if (nPos >= m_nLen)
	{
		throw "access CString char subscript";
	}
	return m_pcData[nPos];
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
	return m_nLen;
}

uint32 CString::Size()const
{
	return m_nLen;
}

uint32 CString::Capacity() const
{
	return m_nCap;
}


void CString::Reserve(uint32 nSize)
{
	if (nSize >= m_nCap)
	{
		m_nCap = CalMem	(nSize);
		char * pcTmp = m_pcData;
		m_pcData = Alloc(m_nCap);
		Copy(pcTmp, nSize);
	}
}

bool CString::Empty()
{
	return m_nLen == 0;
}

void CString::Clear()
{
	m_nLen = 0;
	m_pcData[m_nLen] = '\0';
}

const char * CString::CStr2Char() const
{
	return m_pcData;
}

uint32 CString::CalLen(const char* pcStr)
{
	if (pcStr == nullptr)
	{
		return 0;
	}
	return strlen(pcStr);
}

uint32 CString::CalMem(uint32 nLen)
{
	uint32 nCap = 2;
	while (nCap <= nLen)
	{
		nCap = nCap << 1;
	}
	return nCap;
}

char* CString::Alloc(int nLen)
{
	return new char(nLen);
}

void CString::Assign(const char * pcStr)
{
	Assign(pcStr, CalLen(pcStr));
}

void CString::Assign(const char * pcStr, uint32 nLen)
{
	m_pcData = Alloc(CalMem(nLen));
	Copy(pcStr, nLen);
}

void CString::Copy(const char * pcStr)
{
	Copy(pcStr, CalLen(pcStr));
}

void CString::Copy(const char* pcStr, uint32 nLen)
{
	m_nLen = nLen;
	strncpy(m_pcData, pcStr, m_nLen);
	m_pcData[m_nLen] = '\0';
}

CString operator+(const CString & oStr1, const CString & oStr2)
{
	return CString();
}
