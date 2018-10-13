#include "content_process.h"
#pragma region IContentProcess
//���ִ�����
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
//��Դ�ļ� �� Ŀ���ļ�
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
//�ر��������ļ�
void IContentProcess::Close()
{
	m_oFileIn.close();
	m_oFileOut.close();
}

//��Դ�ļ��еõ�һ��
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
//�ж��Ƿ�Ҫ���д���
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
//�Զ��崦����� ��Դ�ַ�������Ϊ�µ��ַ���
//TODO û���������� �ݴ��Լ���
string CContentProcess::ProcessLine(string strSource)
{
	string strSpace = "";
	string strFunc = "";
	string strOut = "";
	vector<string> vecOut;
	vector<string> vecVarName;

	//��ȡǰ��Ŀհ�
	int nSt = 0, nEnd = 0;
	while (nEnd < strSource.size() && isspace(strSource[nEnd]))
	{
		nEnd++;
	}
	strSpace = strSource.substr(nSt, nEnd - nSt);

	//��ȡ������
	nSt = nEnd;
	while (nEnd < strSource.size() && (isalpha(strSource[nEnd]) || strSource[nEnd] == '_'))
	{
		nEnd++;
	}
	strFunc = strSource.substr(nSt, nEnd - nSt);

	//��ȡ�������� ���������ַ�ֱ��"
	while (nEnd < strSource.size() && strSource[nEnd] != '\"')
	{
		nEnd++;
	}

	//��ȡԭʼ�ַ��� ԭʼ�ַ�����������"�ַ�
	nSt = ++nEnd;
	while (nEnd < strSource.size() && strSource[nEnd] != '\"')
	{
		nEnd++;
	}
	strOut = strSource.substr(nSt, nEnd - nSt);
	nSt = nEnd;

	//��ԭʼ�ַ���֫�� 
	int i = 0, j = 0;
	while(i<strOut.size())
	{
		//����%����ֹͣ
		while (j < strOut.size()&&strOut[j]!='%')
		{
			j++;
		}
		string strOutTemp = strOut.substr(i, j - i);
		vecOut.push_back("\"" + strOutTemp + "\"");

		//����%�ַ� ��������%�ַ������ĸ�ַ�
		j++;
		while (j < strOut.size()&&isalpha(strOut[j]))
		{
			j++;
		}

		i = j;
	}

	//��ȡ������
	while (nSt < strSource.size())
	{
		//�������õ��ַ�ֱ���׸���ĸ�ַ�
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

	//�ַ���֫��ɹ��� �������
	string strDest = strSpace;
	strDest += m_strReplace + strFunc.substr(m_strMark.size());

	if (vecOut.size() != vecVarName.size() && vecOut.size() != vecVarName.size() + 1)
	{
		cout << m_strSource << strSource << "�ֽ���������!\n";
		return "";
	}

	//Ĭ����һ��ԭʼ�ַ���һ������ ԭʼ�ַ������ܻ�����
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