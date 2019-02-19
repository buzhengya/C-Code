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
	JSONVALUETYPE_NULL,   ///< @brief ������
	JSONVALUETYPE_BOOL,   ///< @brief �߼�����
	JSONVALUETYPE_NUMBER, ///< @brief ��������
	JSONVALUETYPE_STRING, ///< @brief �ַ�������
	JSONVALUETYPE_LIST,   ///< @brief ������
	JSONVALUETYPE_DICT,   ///< @brief �ֵ�����
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
/// @brief JSONֵ
////////////////////////////////////////////////////////////////////////////////
class CJsonValue
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
	JSONVALUETYPE GetType() { return m_Type; }
	CJsonString* ToString();
	CJsonList * ToList();
	CJsonDict * ToDict();
public: // �����ֶ�����
	CJsonValue();                       ///< @brief ����Ϊnullֵ����
	CJsonValue(bool Value);             ///< @brief ����Ϊboolֵ����
	CJsonValue(double Value);           ///< @brief ����Ϊ��ֵ����
	CJsonValue(JSONVALUETYPE Type);     ///< @brief ����ΪString/List/Dict
	virtual ~CJsonValue();
};

////////////////////////////////////////////////////////////////////////////////
/// @brief JSON�ַ���
////////////////////////////////////////////////////////////////////////////////
class CJsonString : public CJsonValue
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
	CJsonString(std::string Value);
	~CJsonString();
};

////////////////////////////////////////////////////////////////////////////////
/// @brief JSON����
/// @note  CJsonList���Ḻ�������Ӷ���
////////////////////////////////////////////////////////////////////////////////
class CJsonList : public CJsonValue
{
protected:
	std::vector<CJsonValue*> m_vecObj;   ///< @brief �ڲ�����
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
/// @brief JSON�ֵ�
////////////////////////////////////////////////////////////////////////////////
class CJsonDict : public CJsonValue
{
protected:
	std::vector<std::string> m_vecObj;
	std::unordered_map<std::string, CJsonValue*> m_Cache;
public:
	void WriteToStr(std::string& OutStr);

	void Output(VecJsonOutput& vecOutput, string strSturctName);
	/// @brief ���
	void Clear();;

	CJsonValue * FindJson(std::string strName);

	bool AddValue(std::string strName, CJsonValue* pVal);

	bool Contain(std::string Name);
public:
	/// @brief ���캯��
	CJsonDict();
	~CJsonDict();
};

////////////////////////////////////////////////////////////////////////////////
/// @brief JSON������
/// @note  ע��ý�����ֻ�������һ��Ԫ�ء�
////////////////////////////////////////////////////////////////////////////////
class CJson
{
private:
	CJsonValue* m_Root;  ///< @brief ���ڵ�
private: // ��������
	CJsonValue* ParseValue(Reader& Context);    ///< @brief ����һ��ֵ
	CJsonValue* ParseNumber(Reader& Context);   ///< @brief ����һ����ֵ
	CJsonString* ParseString(Reader& Context);  ///< @brief ����һ���ַ���
	CJsonList* ParseList(Reader& Context);      ///< @brief ����һ����
	CJsonDict* ParseDict(Reader& Context);      ///< @brief ����һ���ֵ�

public:
	VecJsonOutput Output();
	CJson();                           ///< @brief ���캯��
	CJson(const std::string& Str);    ///< @brief ���ı�����JSON
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
	string m_strSrcName;  //��ΪǶ�׽ṹ��ʱ ����Ϊkey  ���ʱ�����ṹ��������
	vector<JsonField> m_vecJsonOutput;
public:
	string Type2GoStuctStr(JSON_VALUE_TYPE eType, string strKey);
	void AddField(JsonField field);
	void OutputStruct();
	void SetStructName(string strStructName) { m_strSrcName = strStructName; }
	string GetStructName() { return m_strSrcName; }
	vector<JsonField> GetVecField() { return m_vecJsonOutput; }
};