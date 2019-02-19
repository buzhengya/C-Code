#include "reader.h"

Reader::Reader(const std::string& srcText)
	:m_str(srcText), m_pos(0)
{
	m_len = m_str.size();
}

Reader::Reader(std::ifstream* pStream)
	: m_pos(0)
{

}

Reader::~Reader()
{
}

bool Reader::isEOF()
{
	return (m_pos >= m_len);
}

int Reader::getLine()
{
	return 0;
}

int Reader::getRow()
{
	return 0;
}

char Reader::ReadChar()
{
	if (m_pos == m_len)
	{
		return '\0';
	}
	char tRet = m_str[m_pos++];
	return tRet;
}

char Reader::PeekChar()
{
	if (m_pos == m_len)
	{
		return '\0';
	}
	char tRet = m_str[m_pos];
	return tRet;
}

void Reader::ignoreSpace()
{

}

void Reader::match(char ch, bool bIgnoreSpace)
{
	if (PeekChar() == ch)
	{
		m_pos++;
		return;
	}
	return;
}

void Reader::match(std::string str, bool bIgnoreSpace)
{
	bool bMatch = true;
	int tPos = m_pos;
	int tLen = str.size();
	for (int i = 0; i < tLen; ++i)
	{
		if (m_str[m_pos + i] != str[i])
		{
			//不能匹配
			std::cout << "matck failed. str :" << str << std::endl;
			m_pos = tPos;
			return;
		}
	}

	if (bMatch)
	{
		m_pos += tLen;
	}
}

bool Reader::tryMatch(char ch, bool bIgnoreSpace, bool bMatch)
{
	if (PeekChar() == ch)
	{
		m_pos++;
		return true;
	}
	return false;
}

bool Reader::tryMatch(std::string str, bool bIgnoreSpace, bool bMatch)
{
	int tPos = m_pos;
	int tLen = str.size();
	for (int i = 0; i < tLen; ++i)
	{
		if (m_str[m_pos + i] != str[i])
		{
			//不能匹配
			m_pos = tPos;
			return false;
		}
	}

	if (bMatch)
	{
		m_pos += tLen;
	}
	return bMatch;
}