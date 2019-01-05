#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "reader.h"

class JsonString;
class JsonList;
class JsonDict;

enum JSONVALUETYPE
{
	JSONVALUETYPE_NULL,   ///< @brief 空类型
	JSONVALUETYPE_BOOL,   ///< @brief 逻辑类型
	JSONVALUETYPE_NUMBER, ///< @brief 数字类型
	JSONVALUETYPE_STRING, ///< @brief 字符串类型
	JSONVALUETYPE_LIST,   ///< @brief 表类型
	JSONVALUETYPE_DICT,   ///< @brief 字典类型
};

////////////////////////////////////////////////////////////////////////////////
/// @brief JSON值
////////////////////////////////////////////////////////////////////////////////
class JsonValue
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
public: // 用于手动创建
	JsonValue();                       ///< @brief 创建为null值类型
	JsonValue(bool Value);             ///< @brief 创建为bool值类型
	JsonValue(double Value);           ///< @brief 创建为数值类型
	JsonValue(JSONVALUETYPE Type);     ///< @brief 创建为String/List/Dict
	virtual ~JsonValue();
};

////////////////////////////////////////////////////////////////////////////////
/// @brief JSON字符串
////////////////////////////////////////////////////////////////////////////////
class JsonString : public JsonValue
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
	JsonString(std::string Value);
	~JsonString();
};

////////////////////////////////////////////////////////////////////////////////
/// @brief JSON数组
/// @note  JsonList将会负责销毁子对象
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
/// @brief JSON字典
////////////////////////////////////////////////////////////////////////////////
class JsonDict : public JsonValue
{
protected:
	std::vector<std::string> m_ObjList;                      
	std::unordered_map<std::string, JsonValue*> m_Cache;
public:
	void WriteToStr(std::string& OutStr);

	/// @brief 清空
	void Clear();;

	bool AddValue(std::string strName, JsonValue* pVal);

	bool Contain(std::string Name);
public:
	/// @brief 构造函数
	JsonDict();
	~JsonDict();
};

////////////////////////////////////////////////////////////////////////////////
/// @brief JSON解析器
/// @note  注意该解析器只会解析第一个元素。
////////////////////////////////////////////////////////////////////////////////
class Json
{
private:
	JsonValue* m_Root;  ///< @brief 根节点
private: // 解析函数
	JsonValue* ParseValue(Reader& Context);    ///< @brief 解析一个值
	JsonValue* ParseNumber(Reader& Context);   ///< @brief 解析一个数值
	JsonString* ParseString(Reader& Context);  ///< @brief 解析一个字符串
	//JsonList* ParseList(Reader& Context);      ///< @brief 解析一个表
	JsonDict* ParseDict(Reader& Context);      ///< @brief 解析一个字典

public:
	Json();                           ///< @brief 构造函数
	Json(const std::string& Str);    ///< @brief 从文本构造JSON
};