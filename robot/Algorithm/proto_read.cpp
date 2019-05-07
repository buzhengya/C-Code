#include "proto_read.h"
#include "tool.h"

bool CProtoRead::GetProto(string strFile, vector<string> vecProto)
{
	return false;
}

bool CProtoRead::_OpenFile()
{
	if (m_fStream.is_open())
	{
		_CloseFile();
	}

	m_fStream.open(m_strFile.c_str(), ios::in);
	return m_fStream.is_open();
}

void CProtoRead::_CloseFile()
{
	if (m_fStream.is_open())
	{
		m_fStream.close();
	}
}

bool CProtoRead::_ReadOneMsg(string & strMsg)
{
	string strLine = "";
	while(!getline(m_fStream, strLine))
	{
		strLine = DelComment(strLine);
		size_t nPos = strLine.find("/*");
		if (nPos != strLine.npos)
		{
			string strTmp = "";
			bool bFind = false;
			while(!getline(m_fStream, strTmp))
			{
				strTmp = DelComment(strTmp);
				if (strTmp.find("*/") != strTmp.npos)
				{
					bFind = true;
					break;
				}
			}
			
			if (!bFind) return false;

			strLine = strLine.substr(0, nPos);
			strTmp = strTmp.substr(strTmp.find("*/") + 2);

			strLine += strTmp;
		}

		if 
	}

	return false;
}

bool CProtoRead::GetNextLine(string & strLine)
{
	if (m_nCurIndex < m_strCurLine.size())
	{
		strLine = m_strCurLine.substr(m_nCurIndex);
		m_nCurIndex = m_strCurLine.size();
		return true;
	}

	if (!getline(m_fStream, m_strCurLine))
	{
		return false;
	}
	
	strLine = m_strCurLine;
	m_nCurIndex = m_strCurLine.size();
	return true;
}

bool CProtoRead::GetNextWord(string & strWord)
{
	if (GetNextWord(m_strCurLine, m_nCurIndex, strWord) == false)
	{
		if (GetNextLine(m_strCurLine) == false)
		{
			return false;
		}
		m_nCurIndex = 0;
	}

	return GetNextWord(m_strCurLine, m_nCurIndex, strWord);
}

bool CProtoRead::GetNextWord(string & strSrc, uint32 & nIndex, string & strDst)
{
	uint32 nStart = nIndex;
	for (; nStart < strSrc.size() && IsSpace(strSrc[nStart]); nStart++)
	{
	}

	if (nStart >= strSrc.size()) return false;

	uint32 nEnd = nStart;
	for (; nEnd < strSrc.size() && IsNumOrAlpha(strSrc[nEnd]); nEnd++)
	{
	}

	if (nEnd == nStart) nEnd++;

	strDst = strDst.substr(nStart, nEnd - nStart);
	nIndex = nEnd;
	return true;
}


string CProtoRead::DelComment(string strSrc)
{
	size_t nPos = strSrc.find("//");
	if (nPos == strSrc.npos)
	{
		return strSrc;
	}

	return strSrc.substr(0, nPos);
}
