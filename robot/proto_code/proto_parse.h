#pragma once
#include <string>
#include "base_define.h"
#include <map>
#include "tool.h"
#include "singleton.h"
#include <vector>
#include <set>

using namespace std;

enum FIELD_TYPE
{
	FIELD_OPTIONAL = 0,
	FIELD_REPEATED = 1,
};

enum FIELD_VALUE_TYPE
{
	//FIELD_VALUE_INT		= 1,
	//FIELD_VALUE_STR		= 2,
	//FIELD_VALUE_FLOAT	= 3,
	//FIELD_VALUE_BOOL	= 4,
	//FIELD_VALUE_STRUCT	= 5,
	FIELD_VALUE_COMMON		= 1,
	FIELD_VALUE_SPECIAL		= 2,
};

enum WORD_CONDI_TYPE
{
	WORD_CONDI_IS_WORD	= 1,
	WORD_CONDI_GIVEN_WORD = 2,
};

class CProtoField
{
public:
	void		DealType(const string & strType);
	void		DealValueType(const string & strValueType);
	void		DealFieldName(const string & strName);
	void		DealIndex(const string & strIndex);
	void		DealDefault(const string & strDefault);

	uint32		GetIndex() { return m_nIndex; }
	string		GetName() { return m_strName; }
	string		GetValType() { return m_strValueType; }
	string		GetFieldType() { return m_eType == FIELD_OPTIONAL ? "optional" : "repeated"; }
	string		GetDefault() { return m_strDefault; }

	void		Clear();
	void		PrintField(uint32 nValTypeLen, uint32 nValNameLen);

private:
	string				m_strName;
	uint32				m_nIndex;
	FIELD_TYPE			m_eType;
	FIELD_VALUE_TYPE	m_eValueType;
	string				m_strValueType;
	string				m_strDefault;
};

enum MSG_TYPE
{
	MSG_NONE = 0,
	MSG_REQ = 1,
	MSG_ACK = 2,
	MSG_NTF = 3,
};

class CProtoMsg
{
	public:
		void		Clear();
		void		DealMsgName(const string & strName);
		void		AddField(CProtoField oProtoField);
		void		PrintMsg();
	
		MSG_TYPE	GetMsgType() const { return m_eType; }
		bool		GetFieldByIndex(uint32 nIndex, CProtoField & oProtoField);
		bool		GetFieldByName(string strName, CProtoField & oProtoField);
		string		GetMsgName() { return m_strMsgName; }

private:
	string				m_strMsgName;
	MSG_TYPE			m_eType;
	uint32				m_nMaxFieldValTypeLen;
	uint32				m_nMaxFieldValLen;

	map<string, CProtoField>	m_mapName2Field;
	map<uint32, string>			m_mapIndex2Name;
};

enum PARSE_MSG_STATE
{
	PARSE_MSG_HEAD			= 1,
	PARSE_MSG_END_OR_FIELD	= 2,
	PARSE_MSG_FINISH		= 3,
};

enum PARSE_FIELD_STATE
{
	PARSE_FIELD_HEAD			= 1,
	PARSE_FIELD_DEFAULT_OR_END	= 2,
	PARSE_FIELD_FINISH			= 3,
};

class CProtoParse :public TSingleton<CProtoParse>
{
	friend class TSingleton<CProtoParse>;
public:
	bool GetProtoMsg(const vector<string> & vecStrMsg, vector<CProtoMsg> & vecProtoMsg);
	bool IsCommonType(const string & strType);

private:
	bool _ParseOneMsg(string strMsg);
	bool _DealMsgParseHead();
	bool _DealMsgParseFieldOrEnd();

	bool _ParseOneField();
	bool _DealFieldParseHead();
	bool _DealFieldParseDefaultOrEnd();
	bool _ParseDefault();

	bool _IsNextWordRight(string & strWord, WORD_CONDI_TYPE eType, string strDst, bool bIsWord);
	bool _GetNextWord(string & strWord, bool bMove = true);
	bool _IsWord(const string & strWord);
	bool _IsWordChar(const char szCh);

private:
	CProtoParse() {}
	~CProtoParse() {}

	//parse one msg 
	string		m_strMsg;
	uint32		m_nIndex;
	PARSE_MSG_STATE		m_eParseMsgState;
	CProtoMsg			m_oProtoMsg;

	//parse one field
	PARSE_FIELD_STATE	m_eParseFieldState;
	CProtoField			m_oProtoField;

	//value type set
	set<string>			m_setValueType = { "double", "float", "int32", "uint32","int64","uint64","bool", "string" };
};
