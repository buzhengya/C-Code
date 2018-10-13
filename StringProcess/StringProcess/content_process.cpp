#include "content_process.h"
#pragma region IContentProcess
//文字处理框架
int IContentProcess::Process(string strSourcePath, string strDestPath)
{
	m_strSource = strSourcePath;
	m_strDest = strDestPath;
	int nRes = Init(strSourcePath, strDestPath);
	if (nRes != SUCESS)
	{
		Close();
		return nRes;
	}

	while (!m_oFileIn.eof())
	{
		string strSource = GetLine();
		string strDest = strSource + "\n";
		if (IsProcess(strSource))
		{
			while(!IsLegalEnd(strSource))
			{
				strSource += GetLine();
			}
			strDest = ProcessLine(strSource);
		}
		WriteLine(strDest);
	}
	Close();
	return SUCESS;
}
//打开源文件 和 目标文件
int IContentProcess::Init(string strSourcePath, string strDestPath)
{
	m_oFileIn.open(strSourcePath);
	if (!m_oFileIn)
	{
		return FILE_OPEN_FAIL;
	}

	m_oFileOut.open(strDestPath);
	if(!m_oFileOut)
	{
		return FILE_OPEN_FAIL;
	}

	string strTemp;
	getline(m_oFileIn, strTemp);
	if (strTemp.size() >= 3 && (strTemp[0] < 0 || strTemp[1] < 0 || strTemp[2] < 0))
	{
		assert(m_oFileIn);
		WriteLine(strTemp.substr(0,3));
		m_oFileIn.seekg(3, ios::beg);
	}
	else
	{
		assert(m_oFileIn);
		m_oFileIn.seekg(0, ios::beg);
	}

	return SUCESS;
}
//关闭这两个文件
void IContentProcess::Close()
{
	m_oFileIn.close();
	m_oFileOut.close();
}

//从源文件中得到一行
string IContentProcess::GetLine()
{
	string strSource = "";
	getline(m_oFileIn, strSource);
	return strSource;
}

void IContentProcess::WriteLine(string strDest)
{
	m_oFileOut << strDest;
}

#pragma endregion

#pragma region CContentProcess
//判断是否要进行处理
bool CContentProcess::IsProcess(string strSource)
{
	int i = 0;
	if (i < strSource.size() && strSource[i] < 0) return false;
	while (i < strSource.size() && strSource[i]>=0 && isspace(strSource[i]))
	{
		i++;
	}
	int j = 0;
	while (i < strSource.size() && j < m_strMark.size()&&strSource[i]==m_strMark[j])
	{
		i++;
		j++;
	}
	return j==m_strMark.size();
}
//自定义处理规则 将源字符串处理为新的字符串
//TODO 没有做错误处理 容错性极低
string CContentProcess::ProcessLine(string strSource)
{
	string strSpace = "";
	string strFunc = "";
	string strOut = "";
	vector<string> vecOut;
	vector<string> vecVarName;

	//提取前面的空白
	int nSt = 0, nEnd = 0;
	while (nEnd < strSource.size() && isspace(strSource[nEnd]))
	{
		nEnd++;
	}
	strSpace = strSource.substr(nSt, nEnd - nSt);

	//提取函数名
	nSt = nEnd;
	while (nEnd < strSource.size() && (isalpha(strSource[nEnd]) || strSource[nEnd] == '_'))
	{
		nEnd++;
	}
	strFunc = strSource.substr(nSt, nEnd - nSt);

	//提取函数名后 跨过多余的字符直到"
	while (nEnd < strSource.size() && strSource[nEnd] != '\"')
	{
		nEnd++;
	}

	//提取原始字符串 原始字符串将不包含"字符
	nSt = ++nEnd;
	while (nEnd < strSource.size() && strSource[nEnd] != '\"')
	{
		nEnd++;
	}
	strOut = strSource.substr(nSt, nEnd - nSt);
	nSt = nEnd;

	//将原始字符串肢解 
	int i = 0, j = 0;
	while(i<strOut.size())
	{
		//遇到%符号停止
		while (j < strOut.size()&&strOut[j]!='%')
		{
			j++;
		}
		string strOutTemp = strOut.substr(i, j - i);
		vecOut.push_back("\"" + strOutTemp + "\"");

		//跳过%字符 并且跳过%字符后的字母字符
		j++;
		while (j < strOut.size()&&isalpha(strOut[j]))
		{
			j++;
		}

		i = j;
	}

	//提取变量名
	while (nSt < strSource.size())
	{
		//跳过无用的字符直到首个字母字符
		while (nSt < strSource.size() && !isalpha(strSource[nSt]) && strSource[nSt] != '(')
		{
			nSt++;
		}
		nEnd = nSt;

		while (nEnd < strSource.size() && !(strSource[nEnd] == ',' || (strSource[nEnd] == ')'&&strSource[nEnd + 1] == ';')))
		{
			nEnd++;
		}
		string strVar = strSource.substr(nSt, nEnd - nSt);
		if (!strVar.empty()) vecVarName.push_back(strVar);
		nSt = ++nEnd;
	}

	//字符串肢解成功后 重新组合
	string strDest = strSpace;
	strDest += m_strReplace + strFunc.substr(m_strMark.size());

	if (vecOut.size() != vecVarName.size() && vecOut.size() != vecVarName.size() + 1)
	{
		cout << m_strSource << strSource << "分解后出现问题!\n";
		return "";
	}

	//默认是一个原始字符串一个变量 原始字符串可能会多出来
	int k = 0;
	for(;k<vecVarName.size();k++)
	{
		strDest += " << " + vecOut[k] + " << " + vecVarName[k];
	}

	if (k < vecOut.size()) strDest += " << " + vecOut[k];

	strDest += ";\n";
	return strDest;
}
bool CContentProcess::IsLegalEnd(string strSource)
{
	int i = strSource.size() - 1;
	while(i>=0&&isspace(strSource[i]))
	{
		i--;
	}
	return i >= 0 && strSource[i] == ';';
}
#pragma endregion