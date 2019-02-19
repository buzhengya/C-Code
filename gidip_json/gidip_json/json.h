#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "reader.h"

using namespace std;

class CJsonString;
class CJsonList;
class CJsonDict;
class CJsonStruct;

typedef vector<CJsonStruct*> VecJsonOutput;

enum JSONVALUETYPE
{
	JSONVALUETYPE_NULL,   ///< @brief 空类型
	JSONVALUETYPE_BOOL,   ///< @brief 逻辑类型
	JSONVALUETYPE_NUMBER, ///< @brief 数字类型
	JSONVALUETYPE_STRING, ///< @brief 字符串类型
	JSONVALUETYPE_LIST,   ///< @brief 表类型
	JSONVALUETYPE_DICT,   ///< @brief 字典类型
};

enum JSON_VALUE_TYPE
{
	JSON_VALUE_NUM,
	JSON_VALUE_STR,
	JSON_VALUE_OBJ,
	JSON_VALUE_ARRAY_NUM,
	JSON_VALUE_ARRAY_STR,
	JSON_VALUE_ARRAY_OBJ
};

////////////////////////////////////////////////////////////////////////////////
/// @brief JSON值
////////////////////////////////////////////////////////////////////////////////
class CJsonValue
{
protected:
	JSONVALUETYPE m_Type;  ///< @brief JSON对象类型
						   /// @brief 数据域
	union
	{
		bool m_ValueBool;     ///< @brief 逻辑型
		double m_ValueNumber; ///< @brief 数字型
	};
public:
	virtual void WriteToStr(std::string& OutStr);
	JSONVALUETYPE GetType() { return m_Type; }
	CJsonString* ToString();
	CJsonList * ToList();
	CJsonDict * ToDict();
public: // 用于手动创建
	CJsonValue();                       ///< @brief 创建为null值类型
	CJsonValue(bool Value);             ///< @brief 创建为bool值类型
	CJsonValue(double Value);           ///< @brief 创建为数值类型
	CJsonValue(JSONVALUETYPE Type);     ///< @brief 创建为String/List/Dict
	virtual ~CJsonValue();
};

////////////////////////////////////////////////////////////////////////////////
/// @brief JSON字符串
////////////////////////////////////////////////////////////////////////////////
class CJsonString : public CJsonValue
{
protected:
	std::string m_Str;   ///< @brief 字符串字面值
public:
	void WriteToStr(std::string& OutStr);
public:
	std::string GetStr();           ///< @brief 获得字符串
	void SetStr(std::string Value); ///< @brief 设置字符串
public:
	/// @brief     构造函数
	/// @param[in] Value 字符串值
	CJsonString(std::string Value);
	~CJsonString();
};

////////////////////////////////////////////////////////////////////////////////
/// @brief JSON数组
/// @note  CJsonList将会负责销毁子对象
////////////////////////////////////////////////////////////////////////////////
class CJsonList : public CJsonValue
{
protected:
	std::vector<CJsonValue*> m_vecObj;   ///< @brief 内部数组
public:
	void WriteToStr(std::string& OutStr);
public:
	CJsonValue* GetValue(int Index);

	void Append(CJsonValue* pNew);

	JSON_VALUE_TYPE GetValueType();

	void Clear();

	int GetCount();
public:
	CJsonList();
	~CJsonList();
};

////////////////////////////////////////////////////////////////////////////////
/// @brief JSON字典
////////////////////////////////////////////////////////////////////////////////
class CJsonDict : public CJsonValue
{
protected:
	std::vector<std::string> m_vecObj;
	std::unordered_map<std::string, CJsonValue*> m_Cache;
public:
	void WriteToStr(std::string& OutStr);

	void Output(VecJsonOutput& vecOutput, string strSturctName);
	/// @brief 清空
	void Clear();;

	CJsonValue * FindJson(std::string strName);

	bool AddValue(std::string strName, CJsonValue* pVal);

	bool Contain(std::string Name);
public:
	/// @brief 构造函数
	CJsonDict();
	~CJsonDict();
};

////////////////////////////////////////////////////////////////////////////////
/// @brief JSON解析器
/// @note  注意该解析器只会解析第一个元素。
////////////////////////////////////////////////////////////////////////////////
class CJson
{
private:
	CJsonValue* m_Root;  ///< @brief 根节点
private: // 解析函数
	CJsonValue* ParseValue(Reader& Context);    ///< @brief 解析一个值
	CJsonValue* ParseNumber(Reader& Context);   ///< @brief 解析一个数值
	CJsonString* ParseString(Reader& Context);  ///< @brief 解析一个字符串
	CJsonList* ParseList(Reader& Context);      ///< @brief 解析一个表
	CJsonDict* ParseDict(Reader& Context);      ///< @brief 解析一个字典

public:
	VecJsonOutput Output();
	CJson();                           ///< @brief 构造函数
	CJson(const std::string& Str);    ///< @brief 从文本构造JSON
};

struct JsonField {
	string m_strKey;
	JSON_VALUE_TYPE m_eType;
public:
	JsonField(string strKey, JSON_VALUE_TYPE eType)
	{
		m_strKey = strKey;
		m_eType = eType;
	}
};

class CJsonStruct
{
private:
	string m_strSrcName;  //当为嵌套结构体时 名字为key  如果时最外层结构体则无名
	vector<JsonField> m_vecJsonOutput;
public:
	string Type2GoStuctStr(JSON_VALUE_TYPE eType, string strKey);
	void AddField(JsonField field);
	void OutputStruct();
	void SetStructName(string strStructName) { m_strSrcName = strStructName; }
	string GetStructName() { return m_strSrcName; }
	vector<JsonField> GetVecField() { return m_vecJsonOutput; }
};