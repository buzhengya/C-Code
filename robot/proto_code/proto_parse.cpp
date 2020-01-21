#include "proto_parse.h"
#include <iostream>
#include <iomanip>

bool CProtoParse::GetProtoMsg(const vector<string>& vecStrMsg, vector<CProtoMsg>& vecProtoMsg)
{
	for (auto & it : vecStrMsg)
	{
		if (!_ParseOneMsg(it))
		{
			cout << "parse msg : " << it << " failed" << endl;
			return false;
		}
		vecProtoMsg.push_back(m_oProtoMsg);
	}

	return true;
}

bool CProtoParse::_ParseOneMsg(string strMsg)
{
	m_strMsg = strMsg;
	m_nIndex = 0;
	m_eParseMsgState = PARSE_MSG_HEAD;
	m_oProtoMsg.Clear();

	string strWord = "";
	bool   bExit = false;
	while (bExit == false)
	{
		switch (m_eParseMsgState)
		{

		case PARSE_MSG_HEAD:
			bExit = !_DealMsgParseHead();
			break;

		case PARSE_MSG_END_OR_FIELD:
			bExit = !_DealMsgParseFieldOrEnd();
			break;

		case PARSE_MSG_FINISH:
			if (_GetNextWord(strWord) && strWord.size() != 0)
			{
				// finish and has word left
				for (auto & it : strWord)
				{
					if (!IsSpace(it))
					{
						cout << "parse msg finish, but still word left. word : " << strWord << endl;
						return false;
					}
				}
			}
			bExit = true;
			break;

		default:
			cout << "parse msg state : " << m_eParseMsgState << " is invalid. msg : " << strMsg << endl;
			return false;
			break;
		}
	}

	return m_eParseMsgState == PARSE_MSG_FINISH;
}

bool CProtoParse::_DealMsgParseHead()
{
	string strWord = "";

	if (!_IsNextWordRight(strWord, WORD_CONDI_GIVEN_WORD, "message", true))
	{
		cout << "parse msg head can not find message. is : " << strWord << endl;
		return false;
	}

	strWord = "";
	if (!_IsNextWordRight(strWord, WORD_CONDI_IS_WORD, "", true))
	{
		cout << "parse msg head message name is not a word. is : " << strWord << endl;
		return false;
	}
	m_oProtoMsg.DealMsgName(strWord);

	strWord = "";
	if (!_IsNextWordRight(strWord, WORD_CONDI_GIVEN_WORD, "{", false))
	{
		cout << "parse msg head char { can not find. is : " << strWord << endl;
		return false;
	}

	m_eParseMsgState = PARSE_MSG_END_OR_FIELD;
	return true;
}

bool CProtoParse::_DealMsgParseFieldOrEnd()
{
	string strWord = "";
	
	if (_GetNextWord(strWord, false) == false)
	{
		cout << "parse msg find field or end. but can not find valid word" << endl;
		return false;
	}

	if (strWord == "}")
	{
		//discard char '}'
		_GetNextWord(strWord);
		m_eParseMsgState = PARSE_MSG_FINISH;
		return true;
	}

	return _ParseOneField();
}

bool CProtoParse::_ParseOneField()
{
	m_eParseFieldState = PARSE_FIELD_HEAD;
	m_oProtoField.Clear();

	string strWord = "";
	bool bExit = false;
	while (bExit == false)
	{
		switch (m_eParseFieldState)
		{
		case PARSE_FIELD_HEAD:
			bExit = !_DealFieldParseHead();
			break;

		case PARSE_FIELD_DEFAULT_OR_END:
			bExit = !_DealFieldParseDefaultOrEnd();
			break;

		case PARSE_FIELD_FINISH:
			m_oProtoMsg.AddField(m_oProtoField);
			bExit = true;
			break;

		default:
			cout << "parse field state : " << m_eParseFieldState << " is invalid. msg : " << m_strMsg << endl;
			break;
		}
	}

	return m_eParseFieldState == PARSE_FIELD_FINISH;
}

bool CProtoParse::_DealFieldParseHead()
{
	string strWord = "";

	if (!_IsNextWordRight(strWord, WORD_CONDI_GIVEN_WORD, "optional", true) && strWord != "repeated")
	{
		cout << "parse field head can not find field type. is : " << strWord << endl;
		return false;
	}
	m_oProtoField.DealType(strWord);
	
	strWord = "";
	if (!_IsNextWordRight(strWord, WORD_CONDI_IS_WORD, "", true))
	{
		cout << "parse field head can not find field value type. is : " << strWord << endl;
		return false;
	}
	m_oProtoField.DealValueType(strWord);

	strWord = "";
	if (!_IsNextWordRight(strWord, WORD_CONDI_IS_WORD, "", true))
	{
		cout << "parse field head can not find field name. is : " << strWord << endl;
		return false;
	}
	m_oProtoField.DealFieldName(strWord);

	strWord = "";
	if (!_IsNextWordRight(strWord, WORD_CONDI_GIVEN_WORD, "=", false))
	{
		cout << "parse field head can not find char = . is : " << strWord << endl;
		return false;
	}

	strWord = "";
	if (!_GetNextWord(strWord))
	{
		cout << "parse field head can not find index. is : " << strWord << endl;
		return false;
	}
	for (auto & it : strWord)
	{
		if (!IsNum(it))
		{
			cout << "parse field head can not find valid index. is : " << strWord << endl;
			return false;
		}
	}
	m_oProtoField.DealIndex(strWord);

	m_eParseFieldState = PARSE_FIELD_DEFAULT_OR_END;
	return true;
}

bool CProtoParse::_DealFieldParseDefaultOrEnd()
{
	string strWord = "";

	if (!_GetNextWord(strWord, false))
	{
		cout << "parse field find default or end failed. can not get next word." << endl;
		return false;
	}

	if (strWord == ";")
	{
		//discard char ';'
		_GetNextWord(strWord);
		m_eParseFieldState = PARSE_FIELD_FINISH;
		return true;
	}

	return _ParseDefault();
}

bool CProtoParse::_ParseDefault()
{
	string strWord = "";

	if (!_IsNextWordRight(strWord, WORD_CONDI_GIVEN_WORD, "[", false))
	{
		cout << "parse field default can not find char [ . word : " << strWord << endl;
		return false;
	}

	strWord = "";
	if (!_IsNextWordRight(strWord, WORD_CONDI_GIVEN_WORD, "default", true))
	{
		cout << "parse field default can not find word 'default'. word : " << strWord << endl;
		return false;
	}

	strWord = "";
	if (!_IsNextWordRight(strWord, WORD_CONDI_GIVEN_WORD, "=", false))
	{
		cout << "parse field default can not find char = . word : " << strWord << endl;
		return false;
	}

	strWord = "";
	if (!_IsNextWordRight(strWord, WORD_CONDI_IS_WORD, "", true))
	{
		cout << "parse field default can not find default value. word : " << strWord << endl;
		return false;
	}
	m_oProtoField.DealDefault(strWord);

	strWord = "";
	if (!_IsNextWordRight(strWord, WORD_CONDI_GIVEN_WORD, "]", false))
	{
		cout << "parse field default can not find char ] . word : " << strWord << endl;
		return false;
	}

	strWord = "";
	if (!_IsNextWordRight(strWord, WORD_CONDI_GIVEN_WORD, ";", true))
	{
		cout << "parse field default can not find char ; . word : " << strWord << endl;
		return false;
	}

	m_eParseFieldState = PARSE_FIELD_FINISH;
	return true;
}

bool CProtoParse::_IsNextWordRight(string & strWord, WORD_CONDI_TYPE eType, string strDst, bool bIsWord)
{
	if (!_GetNextWord(strWord))
	{
		return false;
	}

	if (eType == WORD_CONDI_GIVEN_WORD)
	{
		return strWord == strDst;
	}

	return bIsWord == _IsWord(strWord);
}

bool CProtoParse::_GetNextWord(string & strWord, bool bMove)
{
	size_t nStart = m_nIndex;
	for (;  nStart < m_strMsg.size() && IsSpace(m_strMsg[nStart]); nStart++)
	{
	}

	if (nStart >= m_strMsg.size())
	{
		m_nIndex = nStart;
		return false;
	}

	size_t nEnd = nStart;
	if (nEnd < m_strMsg.size() && !_IsWordChar(m_strMsg[nEnd]))
	{
		nEnd++;
	}
	else
	{
		for (;nEnd < m_strMsg.size() && _IsWordChar(m_strMsg[nEnd]); nEnd++)
		{
		}
	}

	if (bMove)
	{
		m_nIndex = nEnd;
	}
	strWord = m_strMsg.substr(nStart, nEnd - nStart);
	return true;
}

bool CProtoParse::_IsWord(const string & strWord)
{
	for (const auto & it : strWord)
	{
		if (!_IsWordChar(it))
		{
			return false;
		}
	}
	return true;
}

bool CProtoParse::_IsWordChar(const char szCh)
{
	return szCh == '_' || IsNumOrAlpha(szCh);
}

bool CProtoParse::IsCommonType(const string & strType)
{
	if (m_setValueType.find(strType) != m_setValueType.end())
	{
		return true;
	}
	return false;
}

void CProtoMsg::Clear()
{
	m_strMsgName = "";
	m_eType = MSG_NONE;
	m_mapName2Field.clear();
	m_mapIndex2Name.clear();

	m_nMaxFieldValTypeLen = 0;
	m_nMaxFieldValLen = 0;
}

void CProtoMsg::DealMsgName(const string & strName)
{
	m_strMsgName = strName;
	if (m_strMsgName.find("_req") != string::npos)
	{
		m_eType = MSG_REQ;
	}
	else if (m_strMsgName.find("_ack") != string::npos)
	{
		m_eType = MSG_ACK;
	}
	else if (m_strMsgName.find("_ntf") != string::npos)
	{
		m_eType = MSG_NTF;
	}
	else
	{
		m_eType = MSG_NONE;
	}
}

void CProtoMsg::AddField(CProtoField oProtoField)
{
	m_mapIndex2Name[oProtoField.GetIndex()] = oProtoField.GetName();
	m_mapName2Field[oProtoField.GetName()] = oProtoField;

	string strTmp = oProtoField.GetValType();
	m_nMaxFieldValTypeLen = strTmp.size() > m_nMaxFieldValTypeLen ? strTmp.size() : m_nMaxFieldValTypeLen;

	strTmp = oProtoField.GetName();
	m_nMaxFieldValLen = strTmp.size() > m_nMaxFieldValLen ? strTmp.size() : m_nMaxFieldValLen;
}

void CProtoMsg::PrintMsg()
{
	cout << "message " << m_strMsgName << endl;
	cout << "{" << endl;

	for (auto & it : m_mapIndex2Name)
	{
		if (m_mapName2Field.find(it.second) != m_mapName2Field.end())
		{
			m_mapName2Field[it.second].PrintField(m_nMaxFieldValTypeLen, m_nMaxFieldValLen);
		}
	}

	cout << "}" << endl;
	cout << endl;
}

bool CProtoMsg::GetFieldByIndex(uint32 nIndex, CProtoField & oProtoField)
{
	if (m_mapIndex2Name.find(nIndex) != m_mapIndex2Name.end())
	{
		return GetFieldByName(m_mapIndex2Name[nIndex], oProtoField);
	}
	return false;
}

bool CProtoMsg::GetFieldByName(string strName, CProtoField & oProtoField)
{
	if (m_mapName2Field.find(strName) != m_mapName2Field.end())
	{
		oProtoField = m_mapName2Field[strName];
		return true;
	}
	return false;
}

void CProtoField::DealType(const string & strType)
{
	if (strType == "optional")
	{
		m_eType = FIELD_OPTIONAL;
	}
	else
	{
		m_eType = FIELD_REPEATED;
	}
}

void CProtoField::DealValueType(const string & strValueType)
{
	m_strValueType = strValueType;
	if (CProtoParse::Instance()->IsCommonType(strValueType))
	{
		m_eValueType = FIELD_VALUE_COMMON;
	}
	else
	{
		m_eValueType = FIELD_VALUE_SPECIAL;
	}
}

void CProtoField::Clear()
{
	m_strName = "";
	m_strValueType = "";
	m_strDefault = "";
	m_eType = FIELD_OPTIONAL;
	m_eValueType = FIELD_VALUE_COMMON;
	m_nIndex = 0;
}

void CProtoField::PrintField(uint32 nValTypeLen, uint32 nValNameLen)
{
	cout << "    " << GetFieldType() << " ";
	cout << setiosflags(ios::left) << setw(nValTypeLen + 4) << m_strValueType;

	cout << setiosflags(ios::left) << setw(nValNameLen + 4) << m_strName << "    =";

	cout << setiosflags(ios::right) << setw(6) << m_nIndex;

	if (m_strDefault != "")
	{
		cout << "[default = " << m_strDefault << "]";
	}

	cout << ";" << endl;
}

void CProtoField::DealFieldName(const string & strName)
{
	m_strName = strName;
}

void CProtoField::DealIndex(const string & strIndex)
{
	for (const auto & it : strIndex)
	{
		if (!IsNum(it))
		{
			return;
		}
		m_nIndex += m_nIndex * 10 + (it - '0');
	}
}

void CProtoField::DealDefault(const string & strDefault)
{
	m_strDefault = strDefault;
}
