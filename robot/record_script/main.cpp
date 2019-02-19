#include <sys/timeb.h> 
#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include<assert.h>

using namespace std;

fstream read;
ofstream write;
void Init(string strSrccePath, string strDestPath)
{
	read.open(strSrccePath);


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

string RemoveSpaceAndNote(string strSrc)
{
	int nFirst = 0, nSecond = 0;
	for (; nSecond < strSrc.size(); nSecond++)
	{
		if (strSrc[nSecond] != ' '&& strSrc[nSecond] != '\t' && strSrc[nSecond] != '\n')
		{
			strSrc[nFirst++] = strSrc[nSecond];
		}
	}
	string strDest = strSrc.substr(0, nFirst);
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

int Str2Int(string strSrc)
{
	int nRet = 0;
	for (int i = 0; i < strSrc.size(); i++)
	{
		if (strSrc[i] < '0' || strSrc[i] > '9')
		{
			return 0;
		}
		nRet = nRet * 10 + strSrc[i] - '0';
	}
	return nRet;
}

string GenMsgId(string strSrc)
{
	int nSt = 0, nEnd = 0;
	for (int i = strSrc.size() - 1; i >= 0; i--)
	{
		if (strSrc[i] == ']')
		{
			nEnd = i;
		}
		if (strSrc[i] == '=')
		{
			nSt = i + 1;
			break;
		}
	}
	return strSrc.substr(nSt, nEnd - nSt);
}

void GenerateMap(string strSrc, string strDest)
{
	Init(strSrc, strDest);
	string strMsgName = "", strMsgId = "";
	while (!read.eof())
	{
		string strSrc;
		getline(read, strSrc);
		{
			strSrc = RemoveSpaceAndNote(strSrc);
			if (strSrc.find("message") != string::npos && 
				(strSrc.find("_req") != string::npos || strSrc.find("_ntf") != string::npos))
			{
				strMsgName = strSrc.substr(7);
			}
			if (strSrc.find("default=") != string::npos &&
				(strSrc.find("_req") != string::npos || strSrc.find("_ntf") != string::npos))
			{
				strMsgId = GenMsgId(strSrc);
				write << "\tgpArrMsg[" << strMsgId << "] = new(" << strMsgName << ");" << endl;
			}
		}
	}
	Close();
}


int main()
{
	string strDest = "E:\\project\\intermediate\\Proto\\cs_proto.go";

	vector<string> vecstrSrc;
	vecstrSrc.push_back("E:\\project\\intermediate\\Proto\\cs_activity.proto");
	vecstrSrc.push_back("E:\\project\\intermediate\\Proto\\cs_battle.proto");
	vecstrSrc.push_back("E:\\project\\intermediate\\Proto\\cs_chat.proto");
	vecstrSrc.push_back("E:\\project\\intermediate\\Proto\\cs_gamesystem.proto");
	vecstrSrc.push_back("E:\\project\\intermediate\\Proto\\cs_mail.proto");
	vecstrSrc.push_back("E:\\project\\intermediate\\Proto\\cs_role.proto");
	vecstrSrc.push_back("E:\\project\\intermediate\\Proto\\cs_social.proto");

	write << "void RegisterProto()\n{\n\tRegisterBlackList();" << endl;

	for (int i=0; i < vecstrSrc.size(); i++)
	{
		GenerateMap(vecstrSrc[i], strDest);
	}

	write << "}" << endl;
}