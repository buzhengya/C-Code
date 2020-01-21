#pragma once
#include "basic_data.h"
#include "read_file.h"

#include <set>

/*
1.正则表达式和输入字符串都支持 ~ 字符扩展
2.正则表达式中的分割符 ( ) | * + 这5个字符不能作为输入
*/
class CRe2NFA
{
public:
	CRe2NFA() {};
	~CRe2NFA() {};
	
	// 初始化
	bool Init(const std::string& strFile, const std::string& strReg);

	bool Re2NFA();
	const CCell& GetCell() { return m_oCell; };
	//const std::string& GetInputStr() { return m_strInput; };
	//const std::set<char>& GetInputSet() { return m_setInput; }

private:
	// 预处理输入 将输入变成一个无通配符的字符串
	bool _PreProcessInput(const std::string& strInput);

	// 将 a~c == > abc
	bool _ExpandString(const std::string& strSrc, std::string& strDst);

	// 将字符串"()|*o"转为 下标
	size_t _Char2Index(char szC);

	// 判断正则表达式首字符是否合法
	bool _IsFirstValid(char szC);

	// 判断正则表达式首字符是否合法
	bool _IsEndValid(char szC);

	// 判断正则表达式相邻两字符是否合理
	bool _IsAdjoinValid(char szLeft, char szRight);

	// 判断括号是否匹配
	bool _IsBracketMatch(const std::string& strInput);

	bool _GetAllInput(const std::string& strReg);

	// 预处理正则表达式 去除 "~" 之类的特殊字符
	bool _PreProcessReg(const std::string& strReg);

	// 增加连接符 "+" 将 ab 变成 a+b
	bool _AddConnSymbol();

	// 中缀转后缀 (a+b) ==> (ab+)  (a|b) ==> (ab|) a*不变
	bool _Infix2Suffix();

	// 完成正则表达式的转换
	bool _Re2NFA();

	// 处理 (ab|)
	CCell _DoOr(CCell& oLeft, CCell& oRight);

	// 处理 (ab+)
	CCell _DoAnd(CCell& oLeft, CCell& oRight);

	// 处理 (a*)
	void _DoStar(CCell& oCell);

	// 处理 非连接符的情况
	CCell _DoChar(char szInput);

	// 连接边

	CReadFile	m_oReader;
	CCell		m_oCell;
	std::string m_strSrcReg;  // 原始的正则表达式
	std::string m_strInput; //格式化后的输入字符串
	std::set<char>	m_setInput; //所有的可能的输入
	std::string m_strStdReg; // 预处理后 标准的正则表达式 
	std::string m_strInfix; // 中缀表达式 增加+符号后的
	std::string m_strSuffix; // 后缀表达式
	int32		m_nState; // 当前的状态值
};
