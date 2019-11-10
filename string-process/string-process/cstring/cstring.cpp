#include "cstring.h"
#include "../common/mem_pool.h"
#include <utility>

using namespace std;

const char END_CHAR = '\0';
const uint32 CString::npos = -1;
IMemPoolMgr* CString::s_pMemPool = &POOL_MGR;
const uint32 MIN_STR_SIZE = 16;

CString::CString()
{
	_Init();
}

CString::CString(const char * pcStr)
{
	_Init();
	_Assign(pcStr, StrLen(pcStr));
}

CString::CString(const char* pszData, uint32 nLen)
{
	_Init();
	_Assign(pszData, nLen);
}


CString::CString(const CString & oStr)
{
	_Init();
	_Assign(oStr.m_pcData, oStr.Size());
}

CString::CString(const CString& oStr, uint32 nPos, uint32 nLen)
{
	_Init();
	if (nLen == npos)
	{
		nLen = oStr.Size() - nPos;
	}
	_Assign(oStr.Data() + nPos, nLen);
}

CString::CString(uint32 nSize, char szC)
{
	_Init();
	_Assign(nSize, szC);
}


CString& CString::operator=(const CString& oStr) noexcept
{
	_Assign(oStr.Data(), oStr.Size());
	return *this;
}

CString& CString::operator=(CString&& oStr) noexcept
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
	uint32 nSize = pcStr == nullptr ? 0 : StrLen(pcStr);
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
	_Append(pszData, StrLen(pszData));
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

void CString::PopBack()
{
	if (m_nLen > 0)
	{
		m_pcData[m_nLen - 1] = END_CHAR;
		m_nLen -= 1;
	}
}

void CString::Copy(char* pOutput, uint32 nPos, uint32 nLen)
{
}

CString& CString::Assign(CString&& oStr)
{
	_Assign(oStr);
	return *this;
}

CString& CString::Assign(const CString& oStr)
{
	_Assign(oStr.Data(), oStr.Size());
	return *this;
}

CString& CString::Assign(const CString& oStr, uint32 nPos, uint32 nSize)
{
	_Assign(oStr.Data() + nPos, nSize);
	return *this;
}

CString& CString::Assign(const char* pszData)
{
	_Assign(pszData, StrLen(pszData));
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
	_Insert(nPos, oStr.Data(), oStr.Size());
	return *this;
}

CString& CString::Insert(uint32 nPos, const CString& oStr, uint32 nSt, uint32 nSize)
{
	_Insert(nPos, oStr.Data() + nSize, nSize);
	return *this;
}

CString& CString::Insert(uint32 nPos, const char* pszData)
{
	_Insert(nPos, pszData, StrLen(pszData));
	return *this;
}

CString& CString::Insert(uint32 nPos, const char* pszData, uint32 nSize)
{
	_Insert(nPos, pszData, nSize);
	return *this;
}

CString& CString::Insert(uint32 nPos, uint32 nSize, char szC)
{
	_Insert(nPos, nSize, szC);
	return *this;
}

CString& CString::Erase(uint32 nPos, uint32 nLen)
{
	if (nLen == npos)
	{
		nLen = m_nLen;
	}
	uint32 nSt = nPos;
	uint32 nEnd = nPos + nLen;
	for (; nSt <= nEnd; ++nSt)
	{
		m_pcData[nSt] = m_pcData[nSt + nLen];
	}
	m_nLen -= nLen;
	m_pcData[m_nLen] = END_CHAR;
	return *this;
}

CString& CString::Swap(CString& oStr)
{
	CString& oTmp = std::move(oStr);
	oStr = std::move(*this);
	*this = std::move(oTmp);
	return *this;
}

CString& CString::Swap(CString&& oStr)
{
	_Free(m_pcData, m_nCap + 1);
	*this = std::move(oStr);
	return *this;
}

CString& CString::Replace(uint32 nSt, uint32 nLen, const CString& oStr)
{
	_Replace(nSt, nLen, oStr.Data(), oStr.Size());
	return *this;
}

CString& CString::Replace(uint32 nSt, uint32 nLen, const CString& oStr, uint32 nPos, uint32 nSize)
{
	_Replace(nSt, nLen, oStr.Data() + nPos, nSize);
	return *this;
}

CString& CString::Replace(uint32 nSt, uint32 nLen, const char* pszData)
{
	_Replace(nSt, nLen, pszData, StrLen(pszData));
	return *this;
}

CString& CString::Replace(uint32 nSt, uint32 nLen, const char* pszData, uint32 nSize)
{
	_Replace(nSt, nLen, pszData, nSize);
	return *this;
}

CString& CString::Replace(uint32 nSt, uint32 nLen, uint32 nSize, char szC)
{
	_Replace(nSt, nLen, nSize, szC);
	return *this;
}

uint32 CString::Find(const CString& oStr, uint32 nPos) const
{
	return _Find(nPos, oStr.Data(), oStr.Size());
}

uint32 CString::Find(const char* pszData, uint32 nPos) const
{
	return _Find(nPos, pszData, StrLen(pszData));
}

uint32 CString::Find(const char* pszData, uint32 nLen, uint32 nPos) const
{
	return _Find(nPos, pszData, nLen);
}

uint32 CString::Find(char c, uint32 nPos) const
{
	return _Find(nPos, &c, 1);
}

uint32 CString::RFind(const CString& oStr, uint32 nPos) const
{
	return _RFind(nPos, oStr.Data(), oStr.Size());
}

uint32 CString::RFind(const char* pszData, uint32 nPos) const
{
	return _RFind(nPos, pszData, StrLen(pszData));
}

uint32 CString::RFind(const char* pszData, uint32 nLen, uint32 nPos) const
{
	return _RFind(nPos, pszData, nLen);
}

uint32 CString::RFind(char c, uint32 nPos) const
{
	return _RFind(nPos, &c, 1);
}

CString CString::SubStr(uint32 nPos, uint32 nLen) const
{
	if (nLen == npos)
	{
		nLen = m_nLen - nPos;
	}
	return CString(m_pcData + nPos, nLen);
}

int32 CString::Compare(const CString& oStr) const
{
	return _Compare(m_pcData, m_nLen, oStr.Data(), oStr.Size());
}

int32 CString::Compare(uint32 nPos, uint32 nLen, const CString& oStr) const
{
	return _Compare(m_pcData + nPos, nLen, oStr.Data(), oStr.Size());
}

int32 CString::Compare(uint32 nPos, uint32 nLen, const CString& oStr, uint32 nSubPos, uint32 nSubLen) const
{
	return _Compare(m_pcData + nPos, nLen, oStr.Data() + nSubPos, nSubLen);
}

int32 CString::Compare(const char* pszData) const
{
	return _Compare(m_pcData, m_nLen, pszData, StrLen(pszData));
}

int32 CString::Compare(uint32 nPos, uint32 nLen, const char* pszData) const
{
	return _Compare(m_pcData + nPos, nLen, pszData, StrLen(pszData));
}

int32 CString::Compare(uint32 nPos, uint32 nLen, const char* pszData, uint32 nSubPos, uint32 nSubLen) const
{
	return _Compare(m_pcData + nPos, nLen, pszData + nSubPos, nSubLen);
}

CString::~CString()
{
	_Free(m_pcData, m_nCap + 1);
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

const char* CString::Data() const
{
	return m_pcData;
}

void CString::_Init()
{
	m_pcData = nullptr;
	m_nLen = 0;
	m_nCap = 0;
}

void CString::_Assign(const char * pcStr, const uint32 nSize)
{
	if (m_pcData != pcStr)
	{
		_ExpandCap(nSize);

		m_nLen = nSize;
		if (pcStr != nullptr)
		{
			memcpy(m_pcData, pcStr, m_nLen);
		}

		if (m_pcData != nullptr)
		{
			m_pcData[m_nLen] = END_CHAR;
		}
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
	if (m_pcData != nullptr)
	{
		m_pcData[m_nLen] = END_CHAR;
	}
}

void CString::_Assign(CString& oStr)
{
	_Free(m_pcData, m_nCap + 1);
	m_pcData = oStr.m_pcData;
	m_nLen = oStr.m_nLen;
	m_nCap = oStr.m_nCap;

	oStr.m_pcData = nullptr;
	oStr.m_nLen = 0;
	oStr.m_nCap = 0;
}

void CString::_Append(const char* pcStr, uint32 nSize)
{
	uint32 nTotal = nSize + m_nLen;
	_AdjustCap(nTotal);

	memcpy(m_pcData + m_nLen, pcStr, nSize);
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
	return s_pMemPool->Alloc(nSize);
}

void CString::_Free(char* pData, uint32 nSize)
{
	if (pData != nullptr)
	{
		s_pMemPool->Free(pData, nSize);
	}
}

void CString::_ReCap(uint32 nCap)
{
	char* pOld = m_pcData;
	uint32 nOldCap = m_nCap;
	m_nCap = nCap;
	m_pcData = _Alloc(m_nCap + 1);

	memcpy(m_pcData, pOld, m_nLen);
	_Free(pOld, nOldCap + 1);
}

void CString::_ExpandCap(uint32 nSize)
{
	if (nSize > m_nCap)
	{
		_Free(m_pcData, m_nCap + 1);
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
	uint32 nOldCap = m_nCap;
	m_nCap = _CalCap(nSize);
	m_pcData = _Alloc(m_nCap + 1);

	memcpy(m_pcData, pOld, m_nLen);
	_Free(pOld, nOldCap + 1);
}

// nPos ==> nPos + nSize    m_nLen - 1 ==> m_nLen - 1 + nSize
void CString::_ExpandMid(uint32 nPos, uint32 nSize)
{
	uint32 nTotal = m_nCap + nSize;
	if (nTotal > m_nCap)
	{
		char* pOld = m_pcData;
		uint32 nOldCap = m_nCap;
		m_nCap = _CalCap(nTotal);
		m_pcData = _Alloc(m_nCap + 1);

		memcpy(m_pcData, pOld, nPos);
		memcpy(m_pcData + nPos + nSize, pOld + nPos, m_nLen - nPos);
		m_nLen += nSize;
		m_pcData[m_nLen] = END_CHAR;
		_Free(pOld, nOldCap + 1);
	}
	else
	{
		// ÄÚ´æÇ¨ÒÆ
		uint32 nEnd = m_nLen - 1 + nSize;
		uint32 nSt = nPos + nSize;
		for (; nEnd >= nSt; --nEnd)
		{
			m_pcData[nEnd] = m_pcData[nEnd - nSize];
		}
		m_nLen += nSize;
		m_pcData[m_nLen] = END_CHAR;
	}
}

void CString::_Move(uint32 nInit, uint32 nRet)
{
	if (nInit == nRet)
	{
		return;
	}

	if (nInit > nRet) // ____nRet____nInit  left move
	{
		uint32 nSt = nRet;
		uint32 nDiff = nInit - nRet;
		uint32 nEnd = m_nLen - nDiff;
		for (; nSt < nEnd; ++nSt)
		{
			m_pcData[nSt] = m_pcData[nSt + nDiff];
		}
		m_nLen -= nDiff;
		m_pcData[m_nLen] = END_CHAR;
	}
	else // ___nInit___nRet____ right move
	{
		uint32 nSt = nRet;
		uint32 nDiff = nRet - nInit;
		uint32 nEnd = m_nLen - 1 + nDiff;
		for (; nEnd >= nSt; --nEnd)
		{
			m_pcData[nEnd] = m_pcData[nEnd - nDiff];
		}
		m_nLen += nDiff;
		m_pcData[m_nLen] = END_CHAR;
	}
}

void CString::_Replace(uint32 nPos, uint32 nLen, const char* pszData, uint32 nSize)
{
	_Move(nPos, nPos + nLen - nSize);
	memcpy(m_pcData + nPos, pszData, nSize);
}

void CString::_Replace(uint32 nPos, uint32 nLen, uint32 nSize, char szC)
{
	_Move(nPos, nPos + nLen - nSize);
	memset(m_pcData + nPos, szC, nSize);
}

void CString::_Insert(uint32 nPos, const char* pszData, uint32 nLen)
{
	_ExpandMid(nPos, nLen);
	memcpy(m_pcData + nPos, pszData, nLen);
}

void CString::_Insert(uint32 nPos, uint32 nSize, char szC)
{
	_ExpandMid(nPos, nSize);
	memset(m_pcData + nPos, szC, nSize);
}

uint32 CString::_Find(uint32 nPos, const char* pszData, uint32 nSize) const
{
	for (uint32 nSt = nPos; nSt + nSize <= m_nLen; ++nSt)
	{
		uint32 i = 0;
		for (; i < nSize; ++i)
		{
			if (m_pcData[nSt + i] != pszData[i])
			{
				break;
			}
		}

		if (i == nSize)
		{
			return nSt;
		}
	}

	return npos;
}

uint32 CString::_RFind(uint32 nPos, const char* pszData, uint32 nSize) const
{
	for (uint32 nSt = nPos; nSt >= nSize - 1; --nSt)
	{
		uint32 i = 0;
		for (; i < nSize; ++i)
		{
			if (m_pcData[nSt - (nSize - 1 - i)] != pszData[i])
			{
				break;
			}
		}

		if (i == nSize)
		{
			return nSt - nSize + 1;
		}
	}

	return npos;
}

int32 CString::_Compare(const char* pszData1, uint32 nSize1, const char* pszData2, uint32 nSize2) const
{
	int32 nRet = strncmp(pszData1, pszData2, nSize1);
	if (nRet == 0)
	{
		return nSize1 > nSize2 ? 1 : -1;
	}

	return nRet;
}

inline uint32 CString::_CalCap(const uint32 nSize)
{
	uint32 nCap = MIN_STR_SIZE;
	while (nCap - 1 < nSize) nCap = nCap << 1;
	return nCap - 1;
}

uint32 StrLen(const char* pszData)
{
	if (pszData == nullptr)
	{
		return 0;
	}

	return strlen(pszData);
}

CString operator+(const CString & oStr1, const CString & oStr2)
{
	CString oStr(oStr1);
	return oStr.Append(oStr2);
}

CString operator+(const CString&& oStr1, const CString& oStr2)
{
	CString oStr(oStr1);
	return oStr.Append(oStr2);
}

CString operator+(const CString& oStr1, const CString&& oStr2)
{
	CString oStr(oStr1);
	return oStr.Append(oStr2);
}

CString operator+(const CString& oStr, const char* pszData)
{
	CString oTmp(oStr);
	return oTmp.Append(pszData);
}

CString operator+(const CString&& oStr, const char* pszData)
{
	CString oTmp(oStr);
	return oTmp.Append(pszData);
}

CString operator+(const char* pszData, const CString& oStr)
{
	CString oTmp(oStr);
	return oTmp.Append(pszData);
}

CString operator+(const CString& oStr, char szC)
{
	CString oTmp(oStr);
	return oTmp.Append(1, szC);
}

CString operator+(const CString&& oStr, char szC)
{
	CString oTmp(oStr);
	return oTmp.Append(1, szC);
}

CString operator+(char szC, const CString& oStr)
{
	CString oTmp(oStr);
	return oTmp.Append(1, szC);
}

CString operator+(char szC, const CString&& oStr)
{
	CString oTmp(oStr);
	return oTmp.Append(1, szC);
}

bool operator==(const CString& oStr1, const CString& oStr2)
{
	return oStr1.Compare(oStr2) == 0;
}

bool operator==(const CString& oStr, const char* pszData)
{
	return oStr.Compare(pszData) == 0;
}

bool operator==(const char* pszData, const CString& oStr)
{
	return oStr.Compare(pszData) == 0;
}

bool operator>(const CString& oStr1, const CString& oStr2)
{
	return oStr1.Compare(oStr2) > 0;
}

bool operator>(const CString& oStr, const char* pszData)
{
	return oStr.Compare(pszData) > 0;
}

bool operator>(const char* pszData, const CString& oStr)
{
	return oStr.Compare(pszData) > 0;
}

bool operator<(const CString& oStr1, const CString& oStr2)
{
	return oStr1.Compare(oStr2) < 0;
}

bool operator<(const CString& oStr, const char* pszData)
{
	return oStr.Compare(pszData) < 0;
}

bool operator<(const char* pszData, const CString& oStr)
{
	return oStr.Compare(pszData) < 0;
}

bool operator!=(const CString& oStr1, const CString& oStr2)
{
	return oStr1.Compare(oStr2) != 0;
}

bool operator!=(const CString& oStr, const char* pszData)
{
	return oStr.Compare(pszData) != 0;
}

bool operator!=(const char* pszData, const CString& oStr)
{
	return oStr.Compare(pszData) != 0;
}

bool operator>=(const CString& oStr1, const CString& oStr2)
{
	return oStr1.Compare(oStr2) >= 0;
}

bool operator>=(const CString& oStr, const char* pszData)
{
	return oStr.Compare(pszData) >= 0;
}

bool operator>=(const char* pszData, const CString& oStr)
{
	return oStr.Compare(pszData) >= 0;
}

bool operator<=(const CString& oStr1, const CString& oStr2)
{
	return oStr1.Compare(oStr2) <= 0;
}

bool operator<=(const CString& oStr, const char* pszData)
{
	return oStr.Compare(pszData) <= 0;
}

bool operator<=(const char* pszData, const CString& oStr)
{
	return oStr.Compare(pszData) <= 0;
}

void Swap(CString& oStr1, CString& oStr2)
{
	oStr1.Swap(oStr2);
}

typedef int32 (*IsExist)(char szC);
void _Read(std::istream& is, CString& oStr, IsExist fn)
{
	oStr.Clear();
	char szC;
	
	while (true)
	{
		is.get(szC);
		if (is.eof() || fn(szC))
		{
			break;
		}
		else
		{
			oStr.PushBack(szC);
		}
	}
}

int32 IsSpace(char szC)
{
	return isspace(szC);
}

istream& operator>>(istream& is, CString& oStr)
{
	_Read(is, oStr, IsSpace);
	return is;
}

ostream& operator<<(ostream& os, CString& oStr)
{
	if (oStr.Data() != nullptr)
	{
		os << oStr.Data();
	}
	return os;
}

std::istream& GetLine(std::istream& is, CString& oStr, char delim)
{
	oStr.Clear();
	char szC;
	
	while (true)
	{
		is.get(szC);
		if (is.eof() || szC == delim || szC == '\n')
		{
			break;
		}
		else
		{
			oStr.PushBack(szC);
		}
	}
	return is;
}

std::istream& GetLine(std::istream&& is, CString& oStr, char delim)
{
	oStr.Clear();
	char szC;
	
	while (true)
	{
		is.get(szC);
		if (is.eof() || szC == delim || szC == '\n')
		{
			break;
		}
		else
		{
			oStr.PushBack(szC);
		}
	}
	return is;

}

int32 IsLineEnd(char szC)
{
	return szC == '\n';
}
std::istream& GetLine(std::istream& is, CString& oStr)
{
	_Read(is, oStr, IsLineEnd);
	return is;
}

std::istream& GetLine(std::istream&& is, CString& oStr)
{
	_Read(is, oStr, IsLineEnd);
	return is;
}
