#pragma once
#include "basic_data.h"
#include "read_file.h"

#include <set>

/*
1.������ʽ�������ַ�����֧�� ~ �ַ���չ
2.������ʽ�еķָ�� ( ) | * + ��5���ַ�������Ϊ����
*/
class CRe2NFA
{
public:
	CRe2NFA() {};
	~CRe2NFA() {};
	
	// ��ʼ��
	bool Init(const std::string& strFile, const std::string& strReg);

	bool Re2NFA();
	const CCell& GetCell() { return m_oCell; };
	//const std::string& GetInputStr() { return m_strInput; };
	//const std::set<char>& GetInputSet() { return m_setInput; }

private:
	// Ԥ�������� ��������һ����ͨ������ַ���
	bool _PreProcessInput(const std::string& strInput);

	// �� a~c == > abc
	bool _ExpandString(const std::string& strSrc, std::string& strDst);

	// ���ַ���"()|*o"תΪ �±�
	size_t _Char2Index(char szC);

	// �ж�������ʽ���ַ��Ƿ�Ϸ�
	bool _IsFirstValid(char szC);

	// �ж�������ʽ���ַ��Ƿ�Ϸ�
	bool _IsEndValid(char szC);

	// �ж�������ʽ�������ַ��Ƿ����
	bool _IsAdjoinValid(char szLeft, char szRight);

	// �ж������Ƿ�ƥ��
	bool _IsBracketMatch(const std::string& strInput);

	bool _GetAllInput(const std::string& strReg);

	// Ԥ����������ʽ ȥ�� "~" ֮��������ַ�
	bool _PreProcessReg(const std::string& strReg);

	// �������ӷ� "+" �� ab ��� a+b
	bool _AddConnSymbol();

	// ��׺ת��׺ (a+b) ==> (ab+)  (a|b) ==> (ab|) a*����
	bool _Infix2Suffix();

	// ���������ʽ��ת��
	bool _Re2NFA();

	// ���� (ab|)
	CCell _DoOr(CCell& oLeft, CCell& oRight);

	// ���� (ab+)
	CCell _DoAnd(CCell& oLeft, CCell& oRight);

	// ���� (a*)
	void _DoStar(CCell& oCell);

	// ���� �����ӷ������
	CCell _DoChar(char szInput);

	// ���ӱ�

	CReadFile	m_oReader;
	CCell		m_oCell;
	std::string m_strSrcReg;  // ԭʼ��������ʽ
	std::string m_strInput; //��ʽ����������ַ���
	std::set<char>	m_setInput; //���еĿ��ܵ�����
	std::string m_strStdReg; // Ԥ����� ��׼��������ʽ 
	std::string m_strInfix; // ��׺���ʽ ����+���ź��
	std::string m_strSuffix; // ��׺���ʽ
	int32		m_nState; // ��ǰ��״ֵ̬
};
