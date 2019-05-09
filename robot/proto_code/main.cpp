#include "proto_read.h"
#include "proto_parse.h"
#include "proto_go.h"
using namespace std;

int main()
{
	string strSource = "E:\\project\\intermediate\\Proto\\cs_proto.proto";
	string strDest = "E:\\project\\intermediate\\Proto\\cs_proto.go";

	vector<string> vecStrMsg;
	CProtoRead::Instance()->GetProto(strSource, vecStrMsg);
	for (auto & it : vecStrMsg)
	{
		cout << it << endl;
	}

	vector<CProtoMsg> vecProtoMsg;
	CProtoParse::Instance()->GetProtoMsg(vecStrMsg, vecProtoMsg);
	
	for (auto & it : vecProtoMsg)
	{
		it.PrintMsg();
	}

	CProtoGolang::Instance()->GenGoCode(vecProtoMsg, strDest, "RoyalTest");
	system("pause");
	return 0;
}