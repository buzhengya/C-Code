#include "json.h"
#include <iostream>

using namespace std;

//std::string doubleToStr(double num)
//{
//	char buf[20];
//	memset(buf, 0, sizeof(buf));
//	itoa(num, buf, 10);
//	return buf;
//}

CJsonValue::CJsonValue()
	:m_Type(JSONVALUETYPE_NULL), m_ValueNumber(0.)
{}

CJsonValue::CJsonValue(bool Value)   ///< @brief 创建为bool类型
	: m_Type(JSONVALUETYPE_BOOL), m_ValueBool(Value)
{}

CJsonValue::CJsonValue(double Value) ///< @brief 创建为数值类型
	: m_Type(JSONVALUETYPE_NUMBER), m_ValueNumber(Value)
{}


CJsonValue::CJsonValue(JSONVALUETYPE Type) ///< @brief 创建为String/List/Dict
	: m_Type(Type)
{}

CJsonValue::~CJsonValue()
{}

void CJsonValue::WriteToStr(std::string& OutStr)
{
	switch (m_Type)
	{
	case JSONVALUETYPE_NULL:
		OutStr += "null";
		break;
	case JSONVALUETYPE_BOOL:
		if (m_ValueBool)
			OutStr += "true";
		else
			OutStr += "false";
		break;
	case JSONVALUETYPE_NUMBER:
		//OutStr += doubleToStr(m_ValueNumber);
		break;
	default:
		std::cout << "CJsonValue::writeToStr Error!" << std::endl;
	}
}

CJsonString * CJsonValue::ToString()
{
	if (CJsonString *pJsonStr = dynamic_cast<CJsonString*>(this))
	{
		return pJsonStr;
	}
	return nullptr;
}

CJsonList * CJsonValue::ToList()
{
	if (CJsonList *pJsonList = dynamic_cast<CJsonList*>(this))
	{
		return pJsonList;
	}
	return nullptr;
}

CJsonDict * CJsonValue::ToDict()
{
	if (CJsonDict *pJsonDict = dynamic_cast<CJsonDict*>(this))
	{
		return pJsonDict;
	}
	return nullptr;
}

void CJsonString::WriteToStr(std::string& OutStr)
{
	OutStr += "\"";
	for (int i = 0; i < m_Str.size(); ++i)
	{
		switch (m_Str[i])
		{
		case '"':
			OutStr += "\"";
			break;
		case '\\':
			OutStr += "\\";
			break;
		case '/':
			OutStr += "/";
			break;
		case '\b':
			OutStr += "\b";
			break;
		case '\f':
			OutStr += "\f";
			break;
		case '\n':
			OutStr += "\n";
			break;
		case '\r':
			OutStr += "\r";
			break;
		case '\t':
			OutStr += "\t";
			break;
		default:
			OutStr += m_Str[i];
			break;
		}
	}
	OutStr += "\"";
}

std::string CJsonString::GetStr()           ///< @brief 获得字符串
{
	return m_Str;
}

void CJsonString::SetStr(std::string Value) ///< @brief 设置字符串
{
	m_Str = Value;
}

CJsonString::CJsonString(std::string Value)
	:CJsonValue(JSONVALUETYPE_STRING), m_Str(Value)
{}

CJsonString::~CJsonString()
{}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * json list * * * * * * * * * * * * * * * * * * * * * * * * * * * *
CJsonList::CJsonList()
	:CJsonValue(JSONVALUETYPE_LIST)
{}

CJsonList::~CJsonList()
{
	Clear();
}

void CJsonList::WriteToStr(std::string& OutStr)
{
	OutStr += "[ ";
	for (int i = 0; i < m_vecObj.size(); ++i)
	{
		m_vecObj[i]->WriteToStr(OutStr);

		if (i != m_vecObj.size() - 1)
			OutStr += ", ";
	}
	OutStr += " ]";
}



CJsonValue* CJsonList::GetValue(int Index)
{
	if (Index >= GetCount())
		return NULL;
	return m_vecObj[Index];
}
void CJsonList::Append(CJsonValue* pNew)
{
	if (pNew)
		m_vecObj.push_back(pNew);
}

JSON_VALUE_TYPE CJsonList::GetValueType()
{
	JSON_VALUE_TYPE eType;
	if (GetCount() != 0)
	{
		switch (m_vecObj[0]->GetType())
		{
		case JSONVALUETYPE_NULL:
			eType = JSON_VALUE_ARRAY_NUM;
			break;

		case JSONVALUETYPE_BOOL:

		case JSONVALUETYPE_NUMBER:
			eType = JSON_VALUE_ARRAY_NUM;
			break;

		case JSONVALUETYPE_STRING:
			eType = JSON_VALUE_ARRAY_STR;
			break;

		case JSONVALUETYPE_DICT:
			eType = JSON_VALUE_ARRAY_OBJ;
			break;

		default:
			cout << "json list first type is illegal!" << endl;
			break;
		}
	}
	else
	{
		cout << "json list is empty!" << endl;
	}
	return eType;
}

/// @brief 清空
void CJsonList::Clear()
{
	std::vector<CJsonValue*>::iterator i = m_vecObj.begin();
	while (i != m_vecObj.end())
	{
		/*delete (*i);
		++i;*/
	}
	m_vecObj.clear();
}

/// @brief 返回对象数量
int CJsonList::GetCount()
{
	return m_vecObj.size();
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * json dictionary * * * * * * * * * * * * * * * * * * * * * * * * * * * *
CJsonDict::CJsonDict()
	:CJsonValue(JSONVALUETYPE_DICT)
{}

CJsonDict::~CJsonDict()
{

}

void CJsonDict::WriteToStr(std::string& OutStr)
{
	OutStr += "{";
	for (int i = 0; i < m_vecObj.size(); ++i)
	{
		CJsonValue* pObj = m_Cache[m_vecObj[i]];
		OutStr += "\"";
		OutStr += m_vecObj[i];
		OutStr += "\":";
		pObj->WriteToStr(OutStr);

		if (i != m_vecObj.size() - 1)
			OutStr += ", ";
	}
	OutStr += " }";
}

void CJsonDict::Output(VecJsonOutput & vecOutput, string strSturctName)
{
	CJsonStruct* pJsonStruct = new(CJsonStruct);
	pJsonStruct->SetStructName(strSturctName);
	for (const auto & it : m_vecObj)
	{
		string strKey = it;
		CJsonValue * pJsonVal = FindJson(strKey);
		string strVal = "";
		JSON_VALUE_TYPE eType;
		switch (pJsonVal->GetType())
		{
		case JSONVALUETYPE_NULL:
			eType = JSON_VALUE_NUM;
			break;

		case JSONVALUETYPE_BOOL:

		case JSONVALUETYPE_NUMBER:
			eType = JSON_VALUE_NUM;
			break;

		case JSONVALUETYPE_STRING:
			eType = JSON_VALUE_STR;
			break;

		case JSONVALUETYPE_LIST:
		{
			CJsonList * pJsonList = pJsonVal->ToList();
			if (pJsonList == nullptr)
			{
				cout << "to list is nullptr" << endl;
			}
			else
			{
				eType = pJsonList->GetValueType();
				if (pJsonList->GetValue(0)->GetType() == JSONVALUETYPE_DICT)
				{
					CJsonDict * pJsonDict = pJsonList->GetValue(0)->ToDict();
					if (pJsonDict == nullptr)
					{
						cout << "to dict is nullptr" << endl;
					}
					else
					{
						pJsonDict->Output(vecOutput,strKey);
					}
				}
			}
		}
		break;

		case JSONVALUETYPE_DICT:
		{
			eType = JSON_VALUE_OBJ;
			CJsonDict * pJsonDict = pJsonVal->ToDict();
			if (pJsonDict == nullptr)
			{
				cout << "to dict is nullptr" << endl;
			}
			else
			{
				pJsonDict->Output(vecOutput,strKey);
			}
		}
		break;

		default:
			cout << "json val type is illegal!" << endl;
			break;
		}
		JsonField field(strKey, eType);
		pJsonStruct->AddField(field);
	}
	vecOutput.push_back(pJsonStruct);
}


void CJsonDict::Clear()
{
	std::unordered_map<std::string, CJsonValue*>::iterator j = m_Cache.begin();
	while (j != m_Cache.end())
	{
		delete (j->second);
		j++;
	}
	m_Cache.clear();
	m_vecObj.clear();
}

CJsonValue * CJsonDict::FindJson(std::string strName)
{
	auto it = m_Cache.find(strName);
	if (it != m_Cache.end())
	{
		return it->second;
	}
	return nullptr;
}

bool CJsonDict::AddValue(std::string strName, CJsonValue * pVal)
{
	if (Contain(strName))
	{
		return false;
	}
	m_vecObj.push_back(strName);
	m_Cache[strName] = pVal;
	return true;
}

bool CJsonDict::Contain(std::string Name)
{
	return m_Cache.find(Name) != m_Cache.end();
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * json Parse * * * * * * * * * * * * * * * * * * * * * * * * * * * *
CJson::CJson()
	:m_Root(NULL)
{
}

CJson::CJson(const std::string& Str)
	: m_Root(NULL)
{
	Reader tReader(Str);
	m_Root = ParseValue(tReader);
}

CJsonValue* CJson::ParseValue(Reader& Context)    ///< @brief 解析一个值
{
	CJsonValue* tRet = NULL;

	Context.ignoreSpace();
	char tChar = Context.PeekChar();
	switch (tChar)
	{
	case '{':
		tRet = ParseDict(Context);
		break;
	case '[':
		tRet = ParseList(Context);
		break;
	case '"':
		tRet = ParseString(Context);
		break;
	case 't':
	{
		Context.match("true", false);
		tRet = new CJsonValue(true);
	}
	break;
	case 'f':
	{
		Context.match("false", false);
		tRet = new CJsonValue(false);
	}
	break;
	case 'n':
	{
		Context.match("null", false);
		tRet = new CJsonValue();
	}
	break;
	default:
		if (tChar == '-' || iswdigit(tChar))
		{
			tRet = ParseNumber(Context);
		}
		else
			cout << "CJson::ParseValue", "Unexpected char.";
		break;
	}
	return tRet;
}

CJsonValue* CJson::ParseNumber(Reader& Context)   ///< @brief 解析一个数值
{
	std::string tRet;
	Context.ignoreSpace();

	char tChar = Context.PeekChar();
	while (isdigit(tChar) || tChar == '-' || tChar == '.' || tChar == 'E' || tChar == 'e')
	{
		Context.ReadChar();

		tRet += tChar;

		if (Context.isEOF())
			break;
		else
			tChar = Context.PeekChar();
	}
	return new CJsonValue(atof(tRet.c_str()));
}

CJsonString* CJson::ParseString(Reader& Context)  ///< @brief 解析一个字符串
{
	std::string tRet;
	Context.match('"', true);
	char tChar;
	while ((tChar = Context.ReadChar()) != '"')
	{
		if (iscntrl(tChar))
		{
			cout << "CJson::ParseString", "Unexpected char.";
		}

		if (tChar == '\\')
		{
			tChar = Context.ReadChar();
			switch (tChar)
			{
			case '"':
				tRet += "\"";
				break;
			case '\\':
				tRet += "\\";
				break;
			case '/':
				tRet += "/";
				break;
			case 'b':
				tRet += "\b";
				break;
			case 'f':
				tRet += "\f";
				break;
			case 'n':
				tRet += "\n";
				break;
			case 'r':
				tRet += "\r";
				break;
			case 't':
				tRet += "\t";
				break;
			case 'u':
				std::cout << "CJson::ParseString", "Unexpected char.", Context.getLine(), Context.getRow();
				break;
			default:
			{
				std::cout << "CJson::ParseString", "Unexpected char.", Context.getLine(), Context.getRow();
				break;
			}
			}
		}
		else
		{
			tRet += tChar;
		}
	}
	return new CJsonString(tRet.c_str());
}

CJsonList* CJson::ParseList(Reader& Context)      ///< @brief 解析一个表
{
	CJsonList* tRet = new CJsonList();
	Context.match('[', true);
	while (1)
	{
		if (Context.tryMatch(']', true, true))
			break;

		CJsonValue* tValue = NULL;

		tValue = ParseValue(Context);


		tRet->Append(tValue);

		if (Context.tryMatch(',', true, true))
			continue;
	}
	return tRet;
}

CJsonDict* CJson::ParseDict(Reader& Context)      ///< @brief 解析一个字典
{
	CJsonDict* tRet = new CJsonDict();

	Context.match('{', true);

	while (1)
	{
		if (Context.tryMatch('}', true, true))
			break;

		//解析键名
		CJsonString* tKey = NULL;

		tKey = ParseString(Context);

		Context.match(':', true);

		//解析键值
		CJsonValue* tValue = NULL;

		tValue = ParseValue(Context);
		// 追加
		tRet->AddValue(tKey->GetStr(), tValue);

		delete tKey;

		if (Context.tryMatch(',', true, true))
			continue;
	}
	return tRet;
}

VecJsonOutput CJson::Output()
{
	VecJsonOutput vecJosnOutput;

	CJsonDict * pJsonDict = m_Root->ToDict();
	if (pJsonDict == nullptr)
	{
		cout << "CJson::Output to dict is nullptr!" << endl;
		return vecJosnOutput;
	}

	pJsonDict->Output(vecJosnOutput, "");
	for (const auto & itStruct : vecJosnOutput)
	{
		itStruct->OutputStruct();
	}
	return vecJosnOutput;
}

string CJsonStruct::Type2GoStuctStr(JSON_VALUE_TYPE eType, string strKey)
{
	switch (eType)
	{
	case JSON_VALUE_NUM:
		return "int32";
		break;

	case JSON_VALUE_STR:
		return "string";
		break;

	case JSON_VALUE_OBJ:
		return "S" + strKey;
		break;

	case JSON_VALUE_ARRAY_NUM:
		return "[]int32";
		break;

	case JSON_VALUE_ARRAY_STR:
		return "[]string";
		break;

	case JSON_VALUE_ARRAY_OBJ:
		return "[]S" + strKey;
		break;

	default:
		break;
	}

	return "unknown type";
}

void CJsonStruct::AddField(JsonField field)
{
	m_vecJsonOutput.push_back(field);
}

void CJsonStruct::OutputStruct()
{
	for (const auto & itField : m_vecJsonOutput)
	{
		cout << itField.m_strKey + "\t\t" + Type2GoStuctStr(itField.m_eType, itField.m_strKey) << endl;
	}
}
