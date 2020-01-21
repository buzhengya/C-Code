#pragma once
#include "base_define.h"
#include "singleton.h"
#include "proto_parse.h"
#include "proto_output.h"
#include <string>
#include <vector>

using namespace std;

class CProtoCpp : public TSingleton<CProtoCpp>, public CProtoOutput
{
	friend class TSingleton<CProtoCpp>;

public:
	bool GenCppCode(vector<CProtoMsg> & vecProtoMsg, const string & strDstFile);

	void SetModule(const string& strModule) { m_strModuleName = strModule; }

	void SetSession(const string& strSession) { m_strSession = strSession; }

	void SetHandler(const string& strHandler) { m_strHandler = strHandler; }
private:
	CProtoCpp() {}
	~CProtoCpp() {}

	bool _DealReqMsg(CProtoMsg & oProtoMsg);

	bool _DealAckOrNtfMsg(CProtoMsg& oProtoMsg);

	bool _GenRegister();
	bool _GenPipeDeclare();
	bool _GenPipeDefine();
	bool _GenModuleDeclare();
	bool _GenModuleDefine();

	void _ReplaceStr(string & strSrc);
	void _ReplaceStr(string & strSrc, const string & strSub, const string & strDst); //strSrc ÖÐµÄ strSub Ìæ»»Îª strDst

	void _Clear();

	void _Write2File();
	void _Write2File(std::vector<std::string> & vecOutput);

	std::vector<string>		m_vecRegister;
	std::vector<string>		m_vecPipeDeclare;
	std::vector<string>		m_vecPipeDefine;
	std::vector<string>		m_vecModuleDeclare;
	std::vector<string>		m_vecModuleDefine;

	string					m_strSession;
	string					m_strHandler;
	string					m_strModuleName;
	string					m_strReqName;
	string					m_strAckName;
	string					m_strFuncName;
	string					m_strProtoId;
};