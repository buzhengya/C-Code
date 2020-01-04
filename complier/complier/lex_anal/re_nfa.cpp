#include "re_nfa.h"
#include "log.h"
#include "wrap_assert.h"

#include <stack>
#include <string>

using namespace std;

//const char pszWildcard[] = "*|+";
const char pszInvalidInput[] = "()*|+";
const char szExpand = '~';

bool IsInputInvalid(char szC)
{
	for (size_t i = 0; i < strlen(pszInvalidInput); ++i)
	{
		if (pszInvalidInput[i] == szC)
		{
			return true;
		}
	}

	return false;
}

bool CRe2NFA::Init(const std::string& strFile, const std::string& strReg)
{
	LOG_INFO << "[RE2NFA] file : " << strFile << " reg string : " 
		<< strReg;

	if (!m_oReader.Init(strFile))
	{
		LOG_ERROR << "[RE2NFA] init file failed. file : " << strFile;
		return false;
	}

	m_strSrcReg = strReg;

	return true;
}

bool CRe2NFA::Re2NFA()
{
	if (!_PreProcessReg(m_strSrcReg))
	{
		LOG_ERROR << "[RE2NFA] process reg failed. reg : " << m_strSrcReg;
		return false;
	}

	if (!_GetAllInput(m_strStdReg))
	{
		LOG_ERROR << "[RE2NFA] get all input failed. std reg : " << m_strStdReg;
		return false;
	}

	if (!_AddConnSymbol())
	{
		LOG_ERROR << "[RE2NFA] add conn symbol failed. std reg : " << m_strStdReg;
		return false;
	}

	if (!_Infix2Suffix())
	{
		LOG_ERROR << "[RE2NFA] infix to suffix failed. infix string : " << m_strInfix;
		return false;
	}

	if (!_Re2NFA())
	{
		LOG_ERROR << "[RE2NFA] suffix to NFA failed. suffix string : " << m_strSuffix;
		return false;
	}

	return true;
}

bool CRe2NFA::_PreProcessInput(const std::string& strInput)
{
	LOG_INFO << "pre process input. input string : " << strInput;
	m_strInput = "";

	for (size_t i = 0; i < strInput.size(); ++i)
	{
		if (IsInputInvalid(strInput[i]))
		{
			LOG_ERROR << strInput[i] << " is invalid. index : " << i << " all invalid : " << pszInvalidInput;
			return false;
		}

		if (strInput[i] == szExpand)
		{
			if (i == 0 || i == strInput.size() - 1)
			{
				LOG_ERROR << "expand char : " << szExpand << " is in wrong pos : " << i << " input : " << strInput;
				return false;
			}

			string strTmp;
			ASSERT_RETVAL_MSG(_ExpandString(strInput.substr(i - 1, 3), strTmp), false,
				"expand string failed. pos : " << i);
			m_strInput += strTmp;
		}
		else
		{
			m_strInput += strInput[i];
		}
	}

	LOG_INFO << "process input string is : " << m_strInput;
	return true;
}

bool CRe2NFA::_ExpandString(const std::string& strSrc, std::string& strDst)
{
	ASSERT_RETVAL_MSG(strSrc.size() == 3 && strSrc[1] == szExpand, false, 
		" expand string is illegal : " << strSrc);


	strDst = "";
	if (strSrc[0] > strSrc[2])
	{
		LOG_ERROR << "expand char error. left char bigger than right. string : " << strSrc;
		return false;
	}

	for (char szC = strSrc[0] + 1; szC < strSrc[2]; ++szC)
	{
		if (!isprint(szC))
		{
			LOG_ERROR << "expand char error. contain not print char. string : " << strSrc;
			return false;
		}
		strDst += szC;
	}

	return true;
}

const string strRegInput = "()|*";
const size_t OTHER_INDEX = strRegInput.size();
size_t CRe2NFA::_Char2Index(char szC)
{
	for (size_t i = 0; i < strRegInput.size(); ++i)
	{
		if (szC == strRegInput[i])
		{
			return i;
		}
	}

	return strRegInput.size();
}

bool CRe2NFA::_IsFirstValid(char szC)
{
	return szC != ')' && szC != '|' && szC != '*';
}

bool CRe2NFA::_IsEndValid(char szC)
{
	return szC == '(' || szC == '|';
}

/*
   (  )  |  *  O
(  1  0  0  0  1
)  1  1  1  1  1
|  1  0  0  0  1
*  1  1  1  0  1
O  1  1  1  1  1
*/
const uint32 nRegTypeNum = 5;
bool pStateTransfer[nRegTypeNum][nRegTypeNum] =
{
	{true , false, false, false, true},
	{true , true , true , true , true},
	{true , false, false, false, true},
	{true , true , true , false, true},
	{true , true , true , true , true},
};

bool CRe2NFA::_IsAdjoinValid(char szLeft, char szRight)
{
	size_t nLeft = _Char2Index(szLeft);
	ASSERT_RETVAL_MSG(nLeft < nRegTypeNum, false, " left char : " << szLeft << " index : " << nLeft);

	size_t nRight = _Char2Index(szRight);
	ASSERT_RETVAL_MSG(nRight < nRegTypeNum, false, " right char : " << szRight << " index : " << nRight);

	return pStateTransfer[nLeft][nRight];
}

bool CRe2NFA::_IsBracketMatch(const std::string& strInput)
{
	uint32 nLeft = 0;
	for (size_t i = 0; i < strInput.size(); ++i)
	{
		if (strInput[i] == '(')
		{
			++nLeft;
			continue;
		}

		if (strInput[i] == ')')
		{
			if (nLeft == 0)
			{
				LOG_ERROR << "bracket not match. input : " << strInput << " pos : " << i;
				return false;
			}
			--nLeft;
		}
	}

	return nLeft == 0;
}

bool CRe2NFA::_GetAllInput(const std::string& strReg)
{
	LOG_INFO << "start get all input input strReg : " << strReg;
	m_setInput.clear();

	for (size_t i = 0; i < strReg.size(); ++i)
	{
		if (_Char2Index(strReg[i]) == OTHER_INDEX)
		{
			m_setInput.insert(strReg[i]);
		}
	}

	m_strInput = "";
	for (auto it : m_setInput)
	{
		m_strInput += it;
	}
	LOG_INFO << "get all input string success. input : " << m_strInput;
	return true;
}

bool CRe2NFA::_PreProcessReg(const std::string& strReg)
{
	LOG_INFO << "pre process reg. strReg : " << strReg;
	if (strReg.empty())
	{
		LOG_ERROR << "reg string is empty.";
		return false;
	}

	if (!_IsFirstValid(strReg[0]))
	{
		LOG_ERROR << "first string is invalid. strReg : " << strReg;
		return false;
	}

	if (!_IsBracketMatch(strReg))
	{
		LOG_ERROR << "reg string bracket not match. strReg : " << strReg;
		return false;
	}

	if (!strReg.empty())
	{
		m_strStdReg = strReg[0];
	}
	for (size_t i = 1; i < strReg.size(); ++i)
	{
		if (!_IsAdjoinValid(strReg[i - 1], strReg[i]))
		{
			LOG_ERROR << "adjoin char is invalid. strReg : " << strReg << " index : " << i;
			return false;
		}

		if (strReg[i] == szExpand)
		{
			if (i == 0 || i == strReg.size() - 1)
			{
				LOG_ERROR << "reg string expand failed. reg string : " << strReg << " pos : " << i;
				return false;
			}

			if (strReg[i - 1] == szExpand || _Char2Index(strReg[i - 1]) != OTHER_INDEX
				|| strReg[i + 1] == szExpand || _Char2Index(strReg[i + 1]) != OTHER_INDEX)
			{
				
				LOG_ERROR << "reg string expand failed. reg string : " << strReg << " pos : " << i;
				return false;
			}

			string strTmp;
			if (!_ExpandString(strReg.substr(i - 1, 3), strTmp))
			{
				LOG_ERROR << "reg string expand failed. reg string : " << strReg << " pos : " << i;
				return false;
			}

			// 将 a~b 扩展为 (a|b)
			strTmp = strReg[i - 1] + strTmp + strReg[i + 1];

			//m_strStdReg 丢弃最后一个字符
			m_strStdReg = m_strStdReg.substr(0, m_strStdReg.size() - 1);
			m_strStdReg += "(";
			for (size_t j = 0; j < strTmp.size(); ++j)
			{
				m_strStdReg += strTmp[j];
				if (j != strTmp.size() - 1)
				{
					m_strStdReg += "|";
				}
			}
			m_strStdReg += ")";
			
			++i;
		}
		else
		{
			m_strStdReg += strReg[i];
		}
	}

	LOG_INFO << "process reg successful. m_strStdReg : " << m_strStdReg;
	return true;
}

// left ==")*O" && right == "(O"
bool CRe2NFA::_AddConnSymbol()
{
	LOG_INFO << "add connect symbol... input std reg : " << m_strStdReg;

	if (!m_strStdReg.empty())
	{
		m_strInfix = m_strStdReg[0];
	}

	for (size_t i = 1; i < m_strStdReg.size(); ++i)
	{
		if ((m_strStdReg[i - 1] == ')' || m_strStdReg[i - 1] == '*' || _Char2Index(m_strStdReg[i - 1]) == OTHER_INDEX) &&
			(m_strStdReg[i] == '(' || _Char2Index(m_strStdReg[i]) == OTHER_INDEX))
		{
			m_strInfix += "+";
		}
		m_strInfix += m_strStdReg[i];
	}

	LOG_INFO << "add connect symbol success. output infix string : " << m_strInfix;
	return true;
}

bool CRe2NFA::_Infix2Suffix()
{
	LOG_INFO << "infix to suffix... input infix string : " << m_strInfix;

	string strInfix = "(" + m_strInfix + ")";
	stack<char> stConnSymbol;
	m_strSuffix = "";

	for (size_t i = 0; i < strInfix.size(); ++i)
	{
		switch (strInfix[i])
		{
		case '(':
			stConnSymbol.push(strInfix[i]);
			break;

		case ')':
			while (!stConnSymbol.empty())
			{
				char szTop = stConnSymbol.top();
				stConnSymbol.pop();
				if (szTop == '(')
				{
					break;
				}
				m_strSuffix += szTop;
			}
			break;
			
		case '+':
			while (!stConnSymbol.empty())
			{
				char szTop = stConnSymbol.top();
				if (szTop == '+')
				{
					m_strSuffix += szTop;
					stConnSymbol.pop();
					continue;
				}
				break;
			}
			stConnSymbol.push(strInfix[i]);
			break;

		case '|':
			while (!stConnSymbol.empty())
			{
				char szTop = stConnSymbol.top();
				if (szTop == '+' || szTop == '|')
				{
					m_strSuffix += szTop;
					stConnSymbol.pop();
					continue;
				}
				break;
			}
			stConnSymbol.push(strInfix[i]);
			break;

		// other
		default:
			m_strSuffix += strInfix[i];
		}
	}

	LOG_INFO << "infix to suffix success. suffix string : " << m_strSuffix;
	return true;
}

bool CRe2NFA::_Re2NFA()
{
	LOG_INFO << "begin _Re2NFA... suffix : " << m_strSuffix;
	stack<CCell> stCell;
	
	CCell oLeft;
	CCell oRight;
	CCell oOld;
	for (size_t i = 0; i < m_strSuffix.size(); ++i)
	{
		switch (m_strSuffix[i])
		{
		case '+':
			ASSERT_CONTINUE_MSG(stCell.size() >= 2, "stCell size : " << stCell.size() << " suffix : " << m_strSuffix);
			oRight = stCell.top();
			stCell.pop();
			oLeft = stCell.top();
			stCell.pop();
			stCell.emplace(_DoAnd(oLeft, oRight));
			break;

		case '|':
			ASSERT_CONTINUE_MSG(stCell.size() >= 2, "stCell size : " << stCell.size() << " suffix : " << m_strSuffix);
			oRight = stCell.top();
			stCell.pop();
			oLeft = stCell.top();
			stCell.pop();
			stCell.emplace(_DoOr(oLeft, oRight));
			break;

		case '*':
			ASSERT_CONTINUE_MSG(stCell.size() >= 1, "stCell size : " << stCell.size() << " suffix : " << m_strSuffix);
			oOld = stCell.top();
			stCell.pop();
			_DoStar(oOld);
			stCell.push(oOld);
			break;

		default:
			stCell.emplace(_DoChar(m_strSuffix[i]));
		}
	}

	ASSERT_RETVAL_MSG(stCell.size() == 1, false, "stCell size not 1. size : " << stCell.size());
	m_oCell = stCell.top();

	LOG_INFO << "_Re2NFA success!";
	m_oCell.Print();

	return true;
}

CCell CRe2NFA::_DoOr(CCell& oLeft, CCell& oRight)
{
	int32 nStart = m_nState++;
	int32 nEnd = m_nState++;

	CCell oCell(nStart, nEnd);
	oCell.PushEdge(nStart, oLeft.GetStart());
	oCell.PushEdge(nStart, oRight.GetStart());
	oCell.PushEdge(oLeft.GetEnd(), nEnd);
	oCell.PushEdge(oRight.GetEnd(), nEnd);

	oCell.MergeEdge(oLeft);
	oCell.MergeEdge(oRight);
	return oCell;
}

CCell CRe2NFA::_DoAnd(CCell& oLeft, CCell& oRight)
{
	int32 nStart = oLeft.GetStart();
	int32 nEnd = oRight.GetEnd();
	int32 nLeftEnd = oLeft.GetEnd();
	int32 nRightStart = oRight.GetStart();

	CCell oCell(nStart, nEnd);
	oCell.PushEdge(nLeftEnd, nRightStart);
	oCell.MergeEdge(oLeft);
	oCell.MergeEdge(oRight);
	return oCell;
}

void CRe2NFA::_DoStar(CCell& oCell)
{
	int32 nOldStart = oCell.GetStart();
	int32 nOldEnd = oCell.GetEnd();
	int32 nStart = m_nState++;
	int32 nEnd = m_nState++;
	oCell.SetStart(nStart);
	oCell.SetEnd(nEnd);

	oCell.PushEdge(nStart, nOldStart);
	oCell.PushEdge(nStart, nEnd);

	oCell.PushEdge(nOldEnd, nOldStart);
	oCell.PushEdge(nOldEnd, nEnd);
}

CCell CRe2NFA::_DoChar(char szInput)
{
	int32 nStart = m_nState++;
	int32 nEnd = m_nState++;
	CCell oCell(nStart, nEnd);
	oCell.PushEdge(nStart, nEnd, szInput);
	return oCell;
}
