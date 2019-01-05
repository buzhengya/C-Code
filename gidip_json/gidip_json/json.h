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
	JSONVALUETYPE_NULL,   ///< @brief ������
	JSONVALUETYPE_BOOL,   ///< @brief �߼�����
	JSONVALUETYPE_NUMBER, ///< @brief ��������
	JSONVALUETYPE_STRING, ///< @brief �ַ�������
	JSONVALUETYPE_LIST,   ///< @brief ������
	JSONVALUETYPE_DICT,   ///< @brief �ֵ�����
};

////////////////////////////////////////////////////////////////////////////////
/// @brief JSONֵ
////////////////////////////////////////////////////////////////////////////////
class JsonValue
{
protected:
	JSONVALUETYPE m_Type;  ///< @brief JSON��������
	/// @brief ������
	union
	{
		bool m_ValueBool;     ///< @brief �߼���
		double m_ValueNumber; ///< @brief ������
	};
public:
	virtual void WriteToStr(std::string& OutStr);
public: // �����ֶ�����
	JsonValue();                       ///< @brief ����Ϊnullֵ����
	JsonValue(bool Value);             ///< @brief ����Ϊboolֵ����
	JsonValue(double Value);           ///< @brief ����Ϊ��ֵ����
	JsonValue(JSONVALUETYPE Type);     ///< @brief ����ΪString/List/Dict
	virtual ~JsonValue();
};

////////////////////////////////////////////////////////////////////////////////
/// @brief JSON�ַ���
////////////////////////////////////////////////////////////////////////////////
class JsonString : public JsonValue
{
protected:
	std::string m_Str;   ///< @brief �ַ�������ֵ
public:
	void WriteToStr(std::string& OutStr);
public:
	std::string GetStr();           ///< @brief ����ַ���
	void SetStr(std::string Value); ///< @brief �����ַ���
public:
	/// @brief     ���캯��
	/// @param[in] Value �ַ���ֵ
	JsonString(std::string Value);
	~JsonString();
};

////////////////////////////////////////////////////////////////////////////////
/// @brief JSON����
/// @note  JsonList���Ḻ�������Ӷ���
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
/// @brief JSON�ֵ�
////////////////////////////////////////////////////////////////////////////////
class JsonDict : public JsonValue
{
protected:
	std::vector<std::string> m_ObjList;                      
	std::unordered_map<std::string, JsonValue*> m_Cache;
public:
	void WriteToStr(std::string& OutStr);

	/// @brief ���
	void Clear();;

	bool AddValue(std::string strName, JsonValue* pVal);

	bool Contain(std::string Name);
public:
	/// @brief ���캯��
	JsonDict();
	~JsonDict();
};

////////////////////////////////////////////////////////////////////////////////
/// @brief JSON������
/// @note  ע��ý�����ֻ�������һ��Ԫ�ء�
////////////////////////////////////////////////////////////////////////////////
class Json
{
private:
	JsonValue* m_Root;  ///< @brief ���ڵ�
private: // ��������
	JsonValue* ParseValue(Reader& Context);    ///< @brief ����һ��ֵ
	JsonValue* ParseNumber(Reader& Context);   ///< @brief ����һ����ֵ
	JsonString* ParseString(Reader& Context);  ///< @brief ����һ���ַ���
	//JsonList* ParseList(Reader& Context);      ///< @brief ����һ����
	JsonDict* ParseDict(Reader& Context);      ///< @brief ����һ���ֵ�

public:
	Json();                           ///< @brief ���캯��
	Json(const std::string& Str);    ///< @brief ���ı�����JSON
};