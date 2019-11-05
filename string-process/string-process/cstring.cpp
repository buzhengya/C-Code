#include "cstring.h"

CString::CString()
{
	m_pcData = nullptr;
	m_nLen = 0;
	m_nCap = 0;
}

CString::CString(const char * pcStr)
{
	_Assign(pcStr, strlen(pcStr));
}


CString::CString(const CString & oStr)
{
	_Assign(oStr.m_pcData, oStr.Size());
}


CString& CString::operator=(const CString& oStr)
{
	_Assign(oStr.Data(), oStr.Size());
	return *this;
}

CString & CString::operator=(char pcStr)
{
	_Assign(&pcStr, 1);
	return *this;
}

CString& CString::operator=(const char *pcStr)
{
	uint32 nSize = pcStr == nullptr ? 0 : strlen(pcStr);
	_Assign(pcStr, nSize);
	return *this;
}

char& CString::operator[](uint32 nPos)
{
	return m_pcData[nPos];
}

const char & CString::operator[](uint32 nPos) const
{
	return m_pcData[nPos];
}

char& CString::At(uint32 nPos)
{
	return m_pcData[nPos];
}

const char& CString::At(uint32 nPos) const
{
	return m_pcData[nPos];
}

char& CString::Front()
{
	return m_pcData[0];
}

const char& CString::Front() const
{
	return m_pcData[0];
}

char& CString::Back()
{
	return m_pcData[m_nLen - 1];
}

const char& CString::Back() const
{
	return m_pcData[m_nLen - 1];
}

CString& CString::operator+=(const char* pszData)
{
	// TODO: 在此处插入 return 语句
}

CString& CString::operator+=(const CString& oStr)
{
	// TODO: 在此处插入 return 语句
}

CString& CString::operator+=(const char szC)
{
	// TODO: 在此处插入 return 语句
}

CString& CString::Append(const CString& oStr)
{
	// TODO: 在此处插入 return 语句
}

CString& CString::Append(const CString& oStr, uint32 nPos, uint32 nSize)
{
	// TODO: 在此处插入 return 语句
}

CString& CString::Append(const char* pszData)
{
	// TODO: 在此处插入 return 语句
}

CString& CString::Append(const char* pszData, uint32 nSize)
{
	// TODO: 在此处插入 return 语句
}

CString& CString::Append(uint32 nSize, char szC)
{
	// TODO: 在此处插入 return 语句
}

CString::~CString()
{
	_Free(m_pcData);
}

uint32 CString::Length()const
{
	return _Size();
}

uint32 CString::Size()const
{
	return _Size();
}

uint32 CString::Capacity() const
{
	return _Capacity();
}

void CString::ReSize(const uint32 nSize, const char szC)
{
	if (_Size() < nSize)
	{
		m_nLen = nSize;
		return;
	}

	if (nSize > _Capacity())
	{
		_ReCap(_CalCap(nSize));
	}
	
	memset(m_pcData + m_nLen, szC, nSize - m_nLen);
	m_nLen = nSize;
	m_pcData[m_nLen] = '\0';
}

void CString::Reserve(const uint32 nCap)
{
	if (nCap <= _Capacity())
	{
		return;
	}

	_ReCap(nCap);
}

bool CString::Empty() const
{
	return _Size() == 0;
}

void CString::Clear()
{
	m_nLen = 0;
	if (_Capacity() > 0)
	{
		m_pcData[0] = '\0';
	}
}

char* CString::Data() const
{
	return m_pcData;
}

void CString::_Assign(const char * pcStr, const uint32 nSize)
{
	if (m_pcData != pcStr)
	{
		if (nSize > _Capacity())
		{
			_Free(m_pcData);
			m_nCap = _CalCap(nSize);
			m_pcData = _Alloc(m_nCap + 1);
		}

		m_nLen = nSize;
		if (pcStr != nullptr)
		{
			strncpy(m_pcData, pcStr, m_nLen);
			m_pcData[m_nLen] = '\0';
		}
	}
}

char* CString::_Alloc(const uint32 nSize)
{
	return new char(nSize);
}

void CString::_Free(char* pData)
{
	if (pData != nullptr)
	{
		delete[] pData;
	}
}

void CString::_ReCap(uint32 nCap)
{
	char* pOld = m_pcData;
	m_nCap = nCap;
	m_pcData = _Alloc(m_nCap + 1);

	strncpy(m_pcData, pOld, m_nLen);
	_Free(pOld);
}

inline uint32 CString::_CalCap(const uint32 nSize)
{
	uint32 nCap = 2;
	while (nCap <= nSize) nCap = nCap << 1;
	return nCap;
}

inline uint32 CString::_Size() const
{
	return m_nLen;
}

inline uint32 CString::_Capacity() const
{
	return m_nCap;
}

CString operator+(const CString & oStr1, const CString & oStr2)
{
	return CString();
}
