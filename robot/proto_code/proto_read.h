#pragma once
// input:file name  output: vector<string> one string one message without comment. begin with message and end with '}'
#include "singleton.h"
#include "base_define.h"
#include <string>
#include <vector>
#include <fstream>

using namespace std;

class CProtoRead : public TSingleton<CProtoRead>
{
	enum READ_STATE
	{
		READ_NONE = 1,
		READ_MSG = 2,
		READ_LEFT_BRACKET = 3,
		READ_FIELD = 4,
		READ_RIGHT_BRACKET = 5,
	};
	friend class TSingleton<CProtoRead>;
public:
	bool GetProto(string strFile, vector<string> & vecProto);

private:
	CProtoRead();
	~CProtoRead();
	
	bool _OpenFile();
	void _CloseFile();

	bool _ReadOneMsg(string & strMsg);

	bool GetNextLine(string & strLine);
	string DelComment(string strSrc);

	string		m_strCurLine;
	uint32		m_nCurIndex;

	ifstream	m_fStream;
	string		m_strFile;
};