#include "red_black_tree.h"
#include "skip_list.h"
#include <sys/timeb.h> 
#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include<assert.h>
using namespace std;

bool isalpha1(char c)
{
	if ((c >= 'a'&&c <= 'z') || (c >= 'A'&&c <= 'Z')) return true;
	return false;
}

vector<string> GetWord(string str)
{
	//删注释
	int i = 0;
	while (i < str.length())
	{
		if(str[i]=='/')
		{
			str = str.substr(0, i);
			break;
		}
		i++;
	}

	int st = 0, end = 0, n = str.length();
	vector<string> vecStr;
	while (end < n)
	{
		st = end;
		while (st < n && !isalpha1(str[st])) st++;
		end = st;
		while (end < n&&isalpha1(str[end])) end++;//end找到第一个非字母
		if (st < end) vecStr.push_back(str.substr(st, end - st));
	}
	return vecStr;
}
ifstream read;
ofstream write;
void Init(string strSourcePath, string strDestPath)
{
	read.open(strSourcePath);


	write.open(strDestPath,ios::out);


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

void Send2Func(string strSource, string strDest)
{
	Init(strSource, strDest);
	while (!read.eof())
	{
		string str;
		getline(read, str);
		vector<string> vecStr = GetWord(str);
		string strName = "";//strName是发送协议struct中 少前缀D 也是proto中对应协议struct的名字
		if (vecStr.size()>0 && vecStr[0] == "message")
		{
			//cout << vecStr[vecStr.size() - 1].length() << endl;
			if (vecStr[vecStr.size() - 1] != "req") continue;
			string str_Name = "";
			for (int i = 1; i<vecStr.size(); i++)
			{
				str_Name += "_" + vecStr[i];
				vecStr[i][0] = toupper(vecStr[i][0]);
				strName += vecStr[i];
			}
			write << "type D" + strName + " struct {\n";
			write << "\n";
			write << "}\n";
			write << "func (p *D" + strName + ")SendMsg(pClient *NetClient, content *string) {\n";
			write << "\tProtoObj := new(SProtoSpace." << strName << ")\n\n";
			write << "\tdata, err := proto.Marshal(ProtoObj)\n";
			write << "\tif err != nil {\n";
			write << "\t\tWarningf(\"" << strName << " marshaling  error : %s\", err)\n";
			write << "\t\treturn\n";
			write << "\t}\n";
			write << "\tpClient.SendMsg(data, int(ProtoObj.GetProtoid()))\n";
			write << "}\n" << endl;
		}
	}
}

void Deal2Func(string strSource, string strDest)
{
	Init(strSource, strDest);
	while (!read.eof())
	{
		string str;
		getline(read, str);
		vector<string> vecStr = GetWord(str);
		string strName = "";
		if (vecStr.size()>0 && vecStr[0] == "message")
		{
			if (vecStr[vecStr.size() - 1] == "req") continue;
			string str_Name = "";
			for (int i = 1; i<vecStr.size(); i++)
			{
				str_Name += "_" + vecStr[i];
				vecStr[i][0] = toupper(vecStr[i][0]);
				strName += vecStr[i];
			}
			write << "pClient.pMapDealFun[int(SProtoSpace.ECSProtoID" << str_Name << "_id)]=new(D" << strName << ")\n";
			write << "type D" + strName + " struct {\n";
			write << "\n";
			write << "}\n";
			write << "func (p *D" + strName + ")handleMsg(pmsgContent []byte, pClient *NetClient) int {\n";
			write << "\tpProto := new(SProtoSpace." << strName << ")\n";
			write << "\terr := proto.Unmarshal(pmsgContent, pProto)\n";
			write << "\tif err != nil {\n";
			write << "\t\tWarningf(\"unmarshaling " << strName << " error : %s\", err)\n";
			write << "\t\treturn 1\n";
			write << "\t}\n\n";
			write << "\treturn 0\n";
			write << "}\n" << endl;
		}
	}
}
void CountFunc(string strSource, string strDest)
{
	Init(strSource, strDest);
	while (!read.eof())
	{
		string str;
		getline(read, str);
		vector<string> vecStr = GetWord(str);
		string strName = "";
		if (vecStr.size() > 0 && vecStr[0] == "message")
		{
			//if (vecStr[vecStr.size() - 1] == "req") continue;
			string str_Name = "";
			for (int i = 1; i < vecStr.size(); i++)
			{
				str_Name += "_" + vecStr[i];
				vecStr[i][0] = toupper(vecStr[i][0]);
				strName += vecStr[i];
			}
			write << "GStatProtoID = append(GStatProtoID, SProtoSpace.ECSProtoID" << str_Name << "_id)\n";
		}
	}
}
void DealRegisterFunc(string strSource, string strDest)
{
	Init(strSource, strDest);
	while (!read.eof())
	{
		string str;
		getline(read, str);
		vector<string> vecStr = GetWord(str);
		string strName = "";
		if (vecStr.size() > 0 && vecStr[0] == "message")
		{
			if (vecStr[vecStr.size() - 1] == "req") continue;
			string str_Name = "";
			for (int i = 1; i < vecStr.size(); i++)
			{
				str_Name += "_" + vecStr[i];
				vecStr[i][0] = toupper(vecStr[i][0]);
				strName += vecStr[i];
			}
			write << "pClient.pMapDealFun[int(SProtoSpace.ECSProtoID" << str_Name << "_id)]=new(D" << strName << ")\n";
		}
	}
}

void SendRegisterFunc(string strSource, string strDest)
{
	Init(strSource, strDest);
	while (!read.eof())
	{
		string str;
		getline(read, str);
		vector<string> vecStr = GetWord(str);
		string strName = "";//strName是发送协议struct中 少前缀D 也是proto中对应协议struct的名字
		if (vecStr.size() > 0 && vecStr[0] == "message")
		{
			//cout << vecStr[vecStr.size() - 1].length() << endl;
			if (vecStr[vecStr.size() - 1] != "req") continue;
			string str_Name = "";
			for (int i = 1; i < vecStr.size(); i++)
			{
				str_Name += "_" + vecStr[i];
				vecStr[i][0] = toupper(vecStr[i][0]);
				strName += vecStr[i];
			}
			write << "p.PMapProtoObj[SProtoSpace.ECSProtoID" << str_Name << "_id]=new(SProtoSpace." << strName << ")\n";
		}
	}
}
void SendSpecialRegisterFunc(string strSource, string strDest)
{
	Init(strSource, strDest);
	while (!read.eof())
	{
		string str;
		getline(read, str);
		vector<string> vecStr = GetWord(str);
		string strName = "";//strName是发送协议struct中 少前缀D 也是proto中对应协议struct的名字
		if (vecStr.size() > 0 && vecStr[0] == "message")
		{
			//cout << vecStr[vecStr.size() - 1].length() << endl;
			if (vecStr[vecStr.size() - 1] != "req") continue;
			string str_Name = "";
			for (int i = 1; i < vecStr.size(); i++)
			{
				str_Name += "_" + vecStr[i];
				vecStr[i][0] = toupper(vecStr[i][0]);
				strName += vecStr[i];
			}
			write << "p.PMapSpecialProtoObj[SProtoSpace.ECSProtoID" << str_Name << "_id]=new(D" << strName << ")\n";
		}
	}
}
int main()
{
	/*CRBTree<int> oRoot;
	for(int i=0;i<20;i++)
	{
		oRoot.Insert(i);
	}
	std::cout << "先序遍历：" << std::endl;
	oRoot.PreOrder(oRoot.GetRoot());
	std::cout << std::endl << "中序遍历：" << std::endl;
	oRoot.MedOrder(oRoot.GetRoot());
	std::cout << std::endl << "后序遍历：" << std::endl;
	oRoot.PostOrder(oRoot.GetRoot());*/

	//SkipList* pSkipList = NewSkipList();
	//srand(time(nullptr));
	//for(int i=0;i<1000;i++)
	//{
	//	//int x = rand() % 2000 + 1;
	//	int x = i;
	//	Insert(pSkipList, x, x*1.32646);
	//}
	//PrintSkipList(pSkipList);

	//Insert(pSkipList, 3, 3 * 1.32646);

	//PrintSkipList(pSkipList);

	/*timeb t;
	ftime(&t);
	std::cout << t.time * 1000 + t.millitm;
	return 0;*/

	string strSource = "E:\\project\\intermediate\\Proto\\cs_proto.proto";
	string strDest = "E:\\project\\intermediate\\Proto\\cs_proto.txt";
	CountFunc(strSource, strDest);
}