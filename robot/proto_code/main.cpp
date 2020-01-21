#include "proto_read.h"
#include "proto_parse.h"
#include "proto_go.h"
#include <algorithm>
#include "proto_cpp.h"
#include <direct.h>
using namespace std;

template<typename T>
uint32 GetRandom(std::vector<T*>& vecKey)
{
	for (auto it : vecKey)
	{
		cout << *it << endl;
	}
	return 0;
}


int main()
{
	string strSource = "D:\\project_dw\\config\\Proto\\cs_proto.proto";
	string strDest = "D:\\project_dw\\config\\Proto\\cs_proto.go";

	vector<string> vecStrMsg;
	CProtoRead::Instance()->GetProto(strSource, vecStrMsg);
	for (auto & it : vecStrMsg)
	{
		//cout << it << endl;
	}

	vector<CProtoMsg> vecProtoMsg;
	CProtoParse::Instance()->GetProtoMsg(vecStrMsg, vecProtoMsg);
	
	for (auto & it : vecProtoMsg)
	{
		//it.PrintMsg();
	}

	//CProtoGolang::Instance()->GenGoCode(vecProtoMsg, strDest, "Person");
	CProtoCpp::Instance()->SetHandler("");
	CProtoCpp::Instance()->SetSession("");
	CProtoCpp::Instance()->SetModule("CUgcPlotMgr");
	CProtoCpp::Instance()->GenCppCode(vecProtoMsg, strDest);
	system("pause");
	return 0;
}
