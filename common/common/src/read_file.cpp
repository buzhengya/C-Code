#include "read_file.h"

using namespace std;

CReadFile::CReadFile()
{

}

CReadFile::~CReadFile()
{
	_CloseFile();
}

bool CReadFile::Init(const std::string& strFile)
{
	return true;
}

bool CReadFile::_OpenFile()
{
	if (m_fStream.is_open())
	{
		_CloseFile();
	}

	m_fStream.open(m_strFile.c_str());
	return m_fStream.is_open();
}

void CReadFile::_CloseFile()
{
	if (m_fStream.is_open())
	{
		m_fStream.close();
	}
}

bool CReadFile::_GetNextLine()
{
	if (m_nCurIndex < m_strCurLine.size())
	{
		m_strCurLine = m_strCurLine.substr(m_nCurIndex);
		m_nCurIndex = INVALID_32BIT;
		return true;
	}

	if (!getline(m_fStream, m_strCurLine))
	{
		return false;
	}

	m_strCurLine = _DelComment(m_strCurLine);
	if (m_strCurLine.find("/*") == string::npos)
	{
		return true;
	}

	string strTmp = m_strCurLine;
	m_strCurLine = m_strCurLine.substr(0, m_strCurLine.find("/*"));

	do
	{
		if (strTmp.find("*/") != string::npos)
		{
			int32 nIndex = strTmp.find("*/") + 2;
			m_strCurLine = m_strCurLine + strTmp.substr(nIndex);

			if (m_strCurLine.size() == 0)
			{
				continue;
			}
			return true;
		}
	} while (getline(m_fStream, strTmp));

	return false;
}

std::string CReadFile::_DelComment(std::string strSrc)
{
	size_t nPos = strSrc.find("//");
	if (nPos == strSrc.npos)
	{
		return strSrc;
	}

	return strSrc.substr(0, nPos);
}
