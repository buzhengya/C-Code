#include "cstring.h"

const char END_CHAR = '\0';
const uint32 CString::npos = -1;

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
	return Append(pszData);
}

CString& CString::operator+=(const CString& oStr)
{
	return Append(oStr);
}

CString& CString::operator+=(const char szC)
{
	return Append(1, szC);
}

CString& CString::Append(const CString& oStr)
{
	_Append(oStr.Data(), oStr.Size());
	return *this;
}

CString& CString::Append(const CString& oStr, uint32 nPos, uint32 nSize)
{
	_Append(oStr.Data() + nPos, nSize);
	return *this;
}

CString& CString::Append(const char* pszData)
{
	_Append(pszData, strlen(pszData));
	return *this;
}

CString& CString::Append(const char* pszData, uint32 nSize)
{
	_Append(pszData, nSize);
	return *this;
}

CString& CString::Append(uint32 nSize, char szC)
{
	_Append(nSize, szC);
	return *this;
}

void CString::PushBack(char szC)
{
	Append(1, szC);
}

CString& CString::Assign(const CString& oStr)
{
	_Assign(oStr.Data(), oStr.Size());
	return *this;
}

CString& CString::Assign(const CString& oStr, uint32 nPos, uint32 nSize)
{
	_Assign(oStr.Data() + nPos, oStr.Size());
	return *this;
}

CString& CString::Assign(const char* pszData)
{
	_Assign(pszData, strlen(pszData));
	return *this;
}

CString& CString::Assign(const char* pszData, uint32 nSize)
{
	_Assign(pszData, nSize);
	return *this;
}

CString& CString::Assign(uint32 nSize, char szC)
{
	_Assign(nSize, szC);
	return *this;
}

CString& CString::Insert(uint32 nPos, const CString& oStr)
{
	// TODO: 在此处插入 return 语句
}

CString& CString::Insert(uint32 nPos, const CString& oStr, uint32 nSt, uint32 nSize)
{
	// TODO: 在此处插入 return 语句
}

CString& CString::Insert(uint32 nPos, const char* pszData)
{
	// TODO: 在此处插入 return 语句
}

CString& CString::Insert(uint32 nPos, const char* pszData, uint32 nSize)
{
	// TODO: 在此处插入 return 语句
}

CString& CString::Insert(uint32 nPos, uint32 nSize, char szC)
{
	// TODO: 在此处插入 return 语句
}

CString::~CString()
{
	_Free(m_pcData);
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

void CString::ReSize(const uint32 nSize, const char szC)
{
	if (m_nLen < nSize)
	{
		m_nLen = nSize;
		return;
	}

	if (nSize > m_nCap)
	{
		_ReCap(_CalCap(nSize));
	}
	
	memset(m_pcData + m_nLen, szC, nSize - m_nLen);
	m_nLen = nSize;
	m_pcData[m_nLen] = END_CHAR;
}

void CString::Reserve(const uint32 nCap)
{
	if (nCap <= m_nCap)
	{
		return;
	}

	_ReCap(nCap);
}

bool CString::Empty() const
{
	return m_nLen == 0;
}

void CString::Clear()
{
	m_nLen = 0;
	if (m_nCap > 0)
	{
		m_pcData[0] = END_CHAR;
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
		_ExpandCap(nSize);

		m_nLen = nSize;
		if (pcStr != nullptr)
		{
			strncpy(m_pcData, pcStr, m_nLen);
		}
		m_pcData[m_nLen] = END_CHAR;
	}
}

void CString::_Assign(uint32 nSize, char szC)
{
	_ExpandCap(nSize);

	m_nLen = nSize;
	if (nSize > 0)
	{
		memset(m_pcData, szC, nSize);
	}
	m_pcData[m_nLen] = END_CHAR;
}

void CString::_Append(const char* pcStr, uint32 nSize)
{
	uint32 nTotal = nSize + m_nLen;
	_AdjustCap(nTotal);

	strncpy(m_pcData + m_nLen, pcStr, nSize);
	m_pcData[nTotal] = END_CHAR;
	m_nLen = nTotal;
}

void CString::_Append(uint32 nSize, char szC)
{
	uint32 nTotal = nSize + m_nLen;
	_AdjustCap(nTotal);

	memset(m_pcData + m_nLen, szC, nSize);
	m_pcData[nTotal] = END_CHAR;
	m_nLen = nTotal;
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

void CString::_ExpandCap(uint32 nSize)
{
	if (nSize > m_nCap)
	{
		_Free(m_pcData);
		m_nCap = _CalCap(nSize);
		m_pcData = _Alloc(m_nCap + 1);
	}
}

void CString::_AdjustCap(uint32 nSize)
{
	if (nSize <= m_nCap)
	{
		return;
	}

	char* pOld = m_pcData;
	m_nCap = _CalCap(nSize);
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

CString operator+(const CString & oStr1, const CString & oStr2)
{
	return CString();
}
