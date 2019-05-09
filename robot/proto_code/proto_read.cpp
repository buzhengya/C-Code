#include "proto_read.h"
#include "tool.h"
#include <iostream>

using namespace std;

bool CProtoRead::GetProto(string strFile, vector<string> & vecProto)
{
	m_strFile = strFile;
	if (!_OpenFile())
	{
		cout << "open file : " << strFile << " failed." << endl;
		return false;
	}

	string strMsg = "";

	while (_ReadOneMsg(strMsg))
	{
		vecProto.push_back(strMsg);
		strMsg = "";
	}

	for (auto & it : strMsg)
	{
		if (!IsSpace(it))
		{
			return false;
		}
	}

	_CloseFile();
	return true;
}

CProtoRead::CProtoRead()
{
	m_nCurIndex = INVALID_32BIT;
}

CProtoRead::~CProtoRead()
{
	_CloseFile();
}

bool CProtoRead::_OpenFile()
{
	if (m_fStream.is_open())
	{
		_CloseFile();
	}

	m_fStream.open(m_strFile.c_str());
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

	while (GetNextLine(strLine))
	{
		if (strLine.find("message") != string::npos)
		{
			strMsg = strLine.substr(strLine.find("message"));
			break;
		}
		strLine = "";
	}
	strLine = "";

	while (GetNextLine(strLine))
	{
		if (strLine.find("}") != string::npos)
		{
			strMsg += strLine.substr(0, strLine.find("}") + 1);
			break;
		}
		strMsg += strLine;
		strLine = "";
	}

	return strMsg.size() > 0 && strMsg[0] == 'm' && strMsg[strMsg.size() - 1] == '}';
}

bool CProtoRead::GetNextLine(string & strLine)
{
	if (m_nCurIndex < m_strCurLine.size())
	{
		strLine = m_strCurLine.substr(m_nCurIndex);
		m_nCurIndex = INVALID_32BIT;
		return true;
	}

	if (!getline(m_fStream, m_strCurLine))
	{
		return false;
	}
	
	m_strCurLine = DelComment(m_strCurLine);
	if (m_strCurLine.find("/*") == string::npos)
	{
		strLine = m_strCurLine;
		return true;
	}

	string strTmp = m_strCurLine;
	m_strCurLine = m_strCurLine.substr(0, m_strCurLine.find("/*"));
	
	do 
	{
		if (strTmp.find("*/") != string::npos)
		{
			m_nCurIndex = strTmp.find("*/") + 2;
			strLine = m_strCurLine + strTmp.substr(m_nCurIndex);
			m_strCurLine = strTmp;

			//这个是为了防止 */ 注释的最后一行没有数据了 m_nCurIndex却不是最大值
			if (m_nCurIndex >= m_strCurLine.size())
			{
				m_nCurIndex = INVALID_32BIT;
			}
			return true;
		}
	} while (getline(m_fStream, strTmp));

	return false;
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
