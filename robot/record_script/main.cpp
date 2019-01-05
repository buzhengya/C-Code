#include <sys/timeb.h> 
#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include<assert.h>

using namespace std;

fstream read;
ofstream write;
void Init(string strSourcePath, string strDestPath)
{
	read.open(strSourcePath);


	write.open(strDestPath, ios::app);


	string strTemp;
	getline(read, strTemp);
	if (strTemp.size() >= 3 && (strTemp[0] < 0 || strTemp[1] < 0 || strTemp[2] < 0))
	{
		assert(read);
		read.seekg(3, ios::beg);
	}
	else
	{
		assert(read);
		read.seekg(0, ios::beg);
	}

}
void Close()
{
	write << endl;
	read.close();
	write.close();
}

string RemoveSpaceAndNote(string strSour)
{
	int nFirst = 0, nSecond = 0;
	for (; nSecond < strSour.size(); nSecond++)
	{
		if (strSour[nSecond] != ' '&& strSour[nSecond] != '\t' && strSour[nSecond] != '\n')
		{
			strSour[nFirst++] = strSour[nSecond];
		}
	}
	string strDest = strSour.substr(0, nFirst);
	int nEnd = strDest.size();
	for (int i = 0; i < strDest.size(); i++)
	{
		if (strDest[i] == '/'&&i > 0 && strDest[i - 1] == '/')
		{
			nEnd = i - 1;
			break;
		}
	}
	return strDest.substr(0, nEnd);
}
bool IsNameChar(char szCh)
{
	if ((szCh >= 'a'&&szCh <= 'z') || (szCh >= 'A'&&szCh <= 'Z') || (szCh >= '0'&&szCh <= '9') || szCh == '_')
	{
		return true;
	}
	return false;
}

string ParseMessageName(string strSour)
{
	return strSour.substr(7, strSour.size());
}

string ParseMessageId(string strSour)
{
	int nLeft = 0, nRight = 0;
	for(int i=0; i<strSour.size(); i++)
	{
		if (strSour[i] == '[')
		{
			nLeft = i;
		}
		else if (strSour[i] == ']')
		{
			nRight = i;
		}
	}
	int nSt = nLeft + 9, nEnd = nRight;
	return strSour.substr(nSt, nEnd - nSt);
}

bool FindHaveSubStr(string strSour,string strSub)
{
	int nSour = strSour.size(), nSub = strSub.size();
	for(int i=0;i<nSour;i++)
	{
		int k = i, j = 0;
		for (; k < nSour&&j < nSub; j++, k++)
		{
			if(strSour[k]!=strSub[j])
			{
				break;
			}
		}
		if(j==nSub)
		{
			return true;
		}
	}
	return false;
}

int Str2Int(string strSour)
{
	int nRet = 0;
	for (int i = 0; i < strSour.size(); i++)
	{
		if (strSour[i] < '0' || strSour[i] > '9')
		{
			return 0;
		}
		nRet = nRet * 10 + strSour[i] - '0';
	}
	return nRet;
}

void GenerateMap(string strSource, string strDest)
{
	Init(strSource, strDest);
	string strMsgName = "", strMsgId = "";
	while (!read.eof())
	{
		string strSour;
		getline(read, strSour);
		{
			strSour = RemoveSpaceAndNote(strSour);
			if (!FindHaveSubStr(strSour, "req"))
			{
				continue;
			}
			if (FindHaveSubStr(strSour,"message"))
			{
				strMsgName = ParseMessageName(strSour);
				continue;
			}
			if (FindHaveSubStr(strSour, "default"))
			{
				strMsgId = ParseMessageId(strSour);
				write << "gpArrMsg[" << strMsgId << "] = new(" << strMsgName << ");" << endl;
			}
		}
	}
	Close();
}

void WriteData(string strPath,string strData)
{
	write.open(strPath, ios::app);
	write << strData << endl;
	write.close();
}

int main()
{
	string strDest = "E:\\project\\intermediate\\Proto\\cs_proto.go";

	vector<string> vecStrSour;
	vecStrSour.push_back("E:\\project\\intermediate\\Proto\\cs_activity.proto");
	vecStrSour.push_back("E:\\project\\intermediate\\Proto\\cs_battle.proto");
	vecStrSour.push_back("E:\\project\\intermediate\\Proto\\cs_chat.proto");
	vecStrSour.push_back("E:\\project\\intermediate\\Proto\\cs_gamesystem.proto");
	vecStrSour.push_back("E:\\project\\intermediate\\Proto\\cs_mail.proto");
	vecStrSour.push_back("E:\\project\\intermediate\\Proto\\cs_role.proto");
	vecStrSour.push_back("E:\\project\\intermediate\\Proto\\cs_social.proto");

	WriteData(strDest, "void RegisterProto()\n{\nRegisterBlackList();");

	for (int i=0; i < vecStrSour.size(); i++)
	{
		GenerateMap(vecStrSour[i], strDest);
	}

	WriteData(strDest, "}");
}