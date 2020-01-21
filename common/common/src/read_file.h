#pragma once
#include "singleton.h"
#include "define.h"

#include <string>
#include <vector>
#include <fstream>

class CReadFile
{
public:
	CReadFile();
	~CReadFile();

	bool Init(const std::string& strFile);
	char GetNextChar();
	char PreReadChar();
	void DiscardChar();

private:

	bool _OpenFile();
	void _CloseFile();


	bool _GetNextLine();
	std::string _DelComment(std::string strSrc);

	std::string		m_strCurLine;
	uint32			m_nCurIndex = 0;

	std::ifstream	m_fStream;
	std::string		m_strFile;
};
