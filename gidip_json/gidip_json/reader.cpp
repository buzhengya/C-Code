#include "reader.h"

Reader::Reader(const std::string& srcText)
	:m_str(srcText), m_pos(0)
{
	m_len = m_str.size();
}

Reader::Reader(std::ifstream* pStream)
	:m_pos(0)
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

}

int Reader::getRow()
{

}

char Reader::ReadChar()
{
	char tRet = m_str[m_pos];
	return tRet;
}

char Reader::PeekChar() 
{
	char tRet = m_str[m_pos];
	return tRet;
}

void Reader::ignoreSpace()
{

}

void Reader::match(char ch, bool bIgnoreSpace)
{

}

void Reader::match(std::string str, bool bIgnoreSpace)
{

}

bool Reader::tryMatch(char ch, bool bIgnoreSpace, bool bMatch)
{
	return true;
}

bool Reader::tryMatch(std::string str, bool bIgnoreSpace, bool bMatch)
{
	return true;
}


