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

bool IsAlphalAndNum(char c)
{
	if (isalpha1(c) || (c >= '0'&&c <= '9')) return true;
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
		while (st < n && !IsAlphalAndNum(str[st])) st++;
		end = st;
		while (end < n&&IsAlphalAndNum(str[end])) end++;//end找到第一个非字母
		if (st < end) vecStr.push_back(str.substr(st, end - st));
	}
	return vecStr;
}
ifstream read;
ofstream write;
void Init(string strSourcePath, string strDestPath)
{
	read.open(strSourcePath);
	write.open(strDestPath,ios::app);
}
void Close()
{
	write << endl;
	read.close();
	write.close();
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
			write << "type S" + strName + " struct {\n";
			write << "\n";
			write << "}\n";
			write << "func (p *S" + strName + ")SendMsg(pClient *SNetClient, strContent *string) {\n";
			write << "\tpProto := new(SProtoSpace." << strName << ")\n\n";
			write << "\terr := JSON2PB(*strContent, pProto)\n";
			write << "\tif err != nil {\n";
			write << "\t\tgolog.Flog.Warningf(\"unmarshaling "<< strName <<" error : \", err)\n";
			write << "\t\treturn\n\t}\n";
			write << "\tdata, err := proto.Marshal(pProto)\n";
			write << "\tif err != nil {\n";
			write << "\t\tgolog.Flog.Warningf(\"" << strName << " marshaling  error : %s\", err)\n";
			write << "\t\treturn\n";
			write << "\t}\n";
			write << "\tpClient.SendMsg(data, int(pProto.GetProtoid()))\n";
			write << "}\n" << endl;
		}
	}
	Close();
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
			write << "type S" + strName + " struct {\n";
			write << "\n";
			write << "}\n";
			write << "func (p *S" + strName + ")HandleMsg(pMsgpMsgConten []byte, pClient *SNetClient) int {\n";
			write << "\tpProto := new(SProtoSpace." << strName << ")\n";
			write << "\terr := proto.Unmarshal(pMsgpMsgConten, pProto)\n";
			write << "\tif err != nil {\n";
			write << "\t\tgolog.Flog.Warningf(\"unmarshaling " << strName << " error : %s\", err)\n";
			write << "\t\treturn 1\n";
			write << "\t}\n";
			write << "\t";
			write << "if pProto.GetRet() != 0{\n";
			write << "\t\tgolog.Flog.Warningf(\"" << strName << " ret is %d\",pProto.GetRet())\n";
			write << "\t}\n";
			write << "\treturn 0\n";
			write << "}\n" << endl;
		}
	}
	Close();
}
void CountFunc(string strSource, string strDest)
{
	Init(strSource, strDest);
	while (!read.eof())
	{
		string str;
		getline(read, str);
		vector<string> vecStr = GetWord(str);
		if (vecStr.size() > 0 && vecStr[0] == "message")
		{
			//if (vecStr[vecStr.size() - 1] == "req") continue;
			string str_Name = "";
			for (int i = 1; i < vecStr.size(); i++)
			{
				str_Name += "_" + vecStr[i];
				vecStr[i][0] = toupper(vecStr[i][0]);
			}
			write << "p.SliStatProtoID = append(p.SliStatProtoID, SProtoSpace.ECSProtoID" << str_Name << "_id)\n";
		}
	}
	Close();
}

void DelayFunc(string strSource, string strDest)
{
	Init(strSource, strDest);
	vector<string> vecStrLast;
	vector<string> vecReqAndAck;
	vector<string> vecCountReqAndAck;
	vecReqAndAck.push_back("//注册ack对应的req\n\n");
	vector<string> vecProtoDelay;
	vecProtoDelay.push_back("\n//注册需要打印的协议\n");
	while (!read.eof())
	{
		string str;
		getline(read, str);
		vector<string> vecStr = GetWord(str);
		if (vecStr.size() > 0 && vecStr[0] == "message")
		{
			string strAck = "";
			string strReq = "";
			if (vecStr[vecStr.size()-1] == "ack"&&vecStrLast[vecStrLast.size()-1] == "req")
			{
				for (int i = 1; i < vecStr.size(); i++)
				{
					strAck += "_" + vecStr[i];
				}
				for (int i = 1; i < vecStrLast.size(); i++)
				{
					strReq += "_" + vecStrLast[i];
				}
				vecCountReqAndAck.push_back("p.ArrReqAndAck[SProtoSpace.ECSProtoID" + strReq + "_id]  = SProtoSpace.ECSProtoID" + strAck + "_id\n");

				vecReqAndAck.push_back("p.ArrReqAndAck[SProtoSpace.ECSProtoID" + strAck + "_id]  = SProtoSpace.ECSProtoID" + strReq + "_id\n");

				vecProtoDelay.push_back("p.SliValidProtoID = append(p.SliValidProtoID,SProtoSpace.ECSProtoID" + strAck + "_id)\n");
			}
			vecStrLast = vecStr;
		}
	}

	for (int i = 0; i < vecCountReqAndAck.size(); i++)
	{
		write << vecCountReqAndAck[i];
	}

	for (int i = 0; i < vecReqAndAck.size(); i++)
	{
		write << vecReqAndAck[i];
	}
	for (int i = 0; i < vecProtoDelay.size(); i++)
	{
		write << vecProtoDelay[i];
	}
	Close();
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
			write << "p.MapDealFun[int(SProtoSpace.ECSProtoID" << str_Name << "_id)]=new(S" << strName << ")\n";
		}
	}
	Close();
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
			write << "p.MapProtoMsg[SProtoSpace.ECSProtoID" << str_Name << "_id]=new(SProtoSpace." << strName << ")\n";
		}
	}
	Close();
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
			write << "p.MapSpecialProtoMsg[SProtoSpace.ECSProtoID" << str_Name << "_id]=new(S" << strName << ")\n";
		}
	}
	Close();
}

void FrameworkGenerate(string strSource, string strDest, string strNameUp,string strNameLow)
{
	Init(strSource, strDest);
	write << "//普通发送协议注册\n";
	write << "func " << strNameUp << "Register(p *SSender){\n";
	write << "Special" << strNameUp << "Register(p)\n\n";
	write << "}\n\n";

	write << "//特殊发送协议注册\n";
	write << "func Special" << strNameUp << "Register(p *SSender){\n\n";
	write << "}\n\n";

	write << "//注册接收到信息时的处理函数\n";
	write << "func Init" << strNameUp << "DealFun(p *SHandler){\n\n";
	write << "}\n\n";


	write << "func (p *SStatProtoCount)" << strNameUp << "ProtoCount(){\n\n";
	write << "}\n\n";

	write << "func (p *SStatProtoDelay)" << strNameUp << "ProtoDelay(){\n\n";
	write << "}\n\n";

	write << "// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *特殊发送协议结构体 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\n\n";
	write << "// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *处理函数结构体 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\n";
	Close();
}

void DStruct2SStruct(string strSource, string strDest, string strNameUp, string strNameLow)
{
	Init(strSource, strDest);
	while (!read.eof())
	{
		string strLine;
		getline(read, strLine);
		for (int i = 0; i < strLine.size(); i++)
		{
			if (strLine[i] == 'D' && i > 0 && !IsAlphalAndNum(strLine[i - 1]))
			{
				int j = i;
				for (; j < strLine.size(); j++)
				{
					if (!IsAlphalAndNum(strLine[j]))
					{
						break;
					}
				}
				if (j > 3)
				{
					string strSuf = strLine.substr(j - 3, 3);
					if (strSuf == "Req" || strSuf == "Ack" || strSuf == "Ntf")
					{
						strLine[i] = 'S';
					}
				}
			}
		}
		write << strLine << endl;
	}
	Close();
}

bool FindHaveSubStr(string strSour, string strSub)
{
	int nSour = strSour.size(), nSub = strSub.size();
	for (int i = 0; i < nSour; i++)
	{
		int k = i, j = 0;
		for (; k < nSour&&j < nSub; j++, k++)
		{
			if (strSour[k] != strSub[j])
			{
				break;
			}
		}
		if (j == nSub)
		{
			return true;
		}
	}
	return false;
}
void Req2Ack(string strSource, string strDest)
{
	Init(strSource, strDest);
	while (!read.eof())
	{
		string strLine;
		getline(read, strLine);
		for (int i = 0; i + 3 < strLine.size(); i++)
		{
			if (strLine.substr(i,3) == "req")
			{
				strLine[i] = 'a';
				strLine[i + 1] = 'c';
				strLine[i + 2] = 'k';
				continue;
			}
			if (strLine.substr(i, 3) == "ack")
			{
				strLine[i] = 'r';
				strLine[i + 1] = 'e';
				strLine[i + 2] = 'q';
			}
		}
		write << strLine << endl;
	}
	Close();
}
int main()
{
	string strSource = "E:\\project\\intermediate\\Proto\\cs_proto.proto";
	string strDest = "E:\\project\\intermediate\\Proto\\cs_proto.go";

	FrameworkGenerate(strSource, strDest, "Plot", "plot");
	//普通发送注册
	SendRegisterFunc(strSource, strDest);
	//特殊发送注册
	SendSpecialRegisterFunc(strSource, strDest);
	//处理函数注册
	DealRegisterFunc(strSource, strDest);
	//协议数量统计注册
	CountFunc(strSource, strDest);
	//协议延迟统计注册
	DelayFunc(strSource, strDest);
	//特殊发送结构体
	Send2Func(strSource, strDest);
	//处理函数结构体
	Deal2Func(strSource, strDest);
	//string strSource = "E:\\project\\y-server\\servers\\pressure_test_go\\src\\client\\task.go";
	//string strDest = "E:\\project\\y-server\\servers\\pressure_test_go\\src\\client\\tmp.go";
	//DStruct2SStruct(strSource, strDest, "", "");
	//BattleScript(strSource, strDest);
	//Req2Ack(strSource, strDest);
}