#include "json.h"
#include <iostream>

using namespace std;

std::string doubleToStr(double num)
{
	char buf[20];
	memset(buf, 0, sizeof(buf));
	_itoa(num, buf, 10);
	return buf;
}

JsonValue::JsonValue()
	:m_Type(JSONVALUETYPE_NULL), m_ValueNumber(0.)
{}

JsonValue::JsonValue(bool Value)   ///< @brief 创建为bool类型
	: m_Type(JSONVALUETYPE_BOOL), m_ValueBool(Value)
{}

JsonValue::JsonValue(double Value) ///< @brief 创建为数值类型
	: m_Type(JSONVALUETYPE_NUMBER), m_ValueNumber(Value)
{}


JsonValue::JsonValue(JSONVALUETYPE Type) ///< @brief 创建为String/List/Dict
	: m_Type(Type)
{}

JsonValue::~JsonValue()
{}

void JsonValue::WriteToStr(std::string& OutStr)
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
		OutStr += doubleToStr(m_ValueNumber);
		break;
	default:
		std::cout << "JsonValue::writeToStr Error!" << std::endl;
	}
}

void JsonString::WriteToStr(std::string& OutStr)
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

std::string JsonString::GetStr()           ///< @brief 获得字符串
{
	return m_Str;
}

void JsonString::SetStr(std::string Value) ///< @brief 设置字符串
{
	m_Str = Value;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * json dictionary * * * * * * * * * * * * * * * * * * * * * * * * * * * *
JsonDict::JsonDict()
	:JsonValue(JSONVALUETYPE_DICT)
{}

JsonDict::~JsonDict()
{

}

void JsonDict::WriteToStr(std::string& OutStr)
{
	OutStr += "{";
	for (int i = 0; i < m_ObjList.size(); ++i)
	{
		JsonValue* pObj = m_Cache[m_ObjList[i]];
		OutStr += "\"";
		OutStr += m_ObjList[i];
		OutStr += "\":";
		pObj->WriteToStr(OutStr);

		if (i != m_ObjList.size() - 1)
			OutStr += ", ";
	}
	OutStr += " }";
}


void JsonDict::Clear()
{
	std::unordered_map<std::string, JsonValue*>::iterator j = m_Cache.begin();
	while (j != m_Cache.end())
	{
		delete (j->second);
		j++;
	}
	m_Cache.clear();
	m_ObjList.clear();
}

bool JsonDict::AddValue(std::string strName, JsonValue * pVal)
{
	if (Contain(strName))
	{
		return false;
	}
	m_ObjList.push_back(strName);
	m_Cache[strName] = pVal;
	return true;
}

bool JsonDict::Contain(std::string Name)
{
	return m_Cache.find(Name) != m_Cache.end();
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * json Parse * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Json::Json() 
	:m_Root(NULL)
{
}

Json::Json(const std::string& Str)
	: m_Root(NULL)
{
	Reader tReader(Str);
	m_Root = ParseValue(tReader);
}

JsonValue* Json::ParseValue(Reader& Context)    ///< @brief 解析一个值
{
	JsonValue* tRet = NULL;

	Context.ignoreSpace();
	char tChar = Context.PeekChar();
	switch (tChar)
	{
	case '{':
		tRet = ParseDict(Context);
		break;
	case '[':
		//tRet = ParseList(Context);
		break;
	case '"':
		tRet = ParseString(Context);
		break;
	case 't':
	{
		Context.match("true", false);
		tRet = new JsonValue(true);
	}
	break;
	case 'f':
	{
		Context.match("false", false);
		tRet = new JsonValue(false);
	}
	break;
	case 'n':
	{
		Context.match("null", false);
		tRet = new JsonValue();
	}
	break;
	default:
		if (tChar == '-' || iswdigit(tChar))
		{
			tRet = ParseNumber(Context);
		}
		else
			cout << "Json::ParseValue", "Unexpected char.";
		break;
	}
	return tRet;
}

JsonValue* Json::ParseNumber(Reader& Context)   ///< @brief 解析一个数值
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
	return new JsonValue(atof(tRet.c_str()));
}

JsonString* Json::ParseString(Reader& Context)  ///< @brief 解析一个字符串
{
	std::string tRet;
	Context.match('"', true);
	char tChar;
	while ((tChar = Context.ReadChar()) != '"')
	{
		if (iscntrl(tChar))
		{
			cout << "Json::ParseString", "Unexpected char.";
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
				std::cout << "Json::ParseString", "Unexpected char.", Context.getLine(), Context.getRow();
				break;
			default:
			{
				std::cout << "Json::ParseString", "Unexpected char.", Context.getLine(), Context.getRow();
				break;
			}
			}
		}
		else
		{
			tRet += tChar;
		}
	}
	return new JsonString(tRet.c_str());
}

JsonDict* Json::ParseDict(Reader& Context)      ///< @brief 解析一个字典
{
	JsonDict* tRet = new JsonDict();

	Context.match('{', true);

	while (1)
	{
		if (Context.tryMatch('}', true, true))
			break;

		//解析键名
		JsonString* tKey = NULL;

		tKey = ParseString(Context);

		Context.match(':', true);

		//解析键值
		JsonValue* tValue = NULL;

		tValue = ParseValue(Context);
		// 追加
		tRet->AddValue(tKey->GetStr(), tValue);

		delete tKey;

		if (Context.tryMatch(',', true, true))
			continue;
	}
	return tRet;
}