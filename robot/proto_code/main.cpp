#include "proto_read.h"
#include "proto_parse.h"
#include "proto_go.h"
#include <algorithm>
#include "proto_cpp.h"
#include <direct.h>
using namespace std;

int main()
{
	string strSource = "E:\\project_master\\intermediate\\Proto\\cs_proto.proto";
	string strDest = "E:\\project_master\\intermediate\\Proto\\cs_proto.go";

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

	CProtoGolang::Instance()->GenGoCode(vecProtoMsg, strDest, "CityMap");
	CProtoCpp::Instance()->GenGoCode(vecProtoMsg, strDest, "CityMap");
	system("pause");
	return 0;
}
