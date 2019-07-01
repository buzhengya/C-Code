#include "proto_read.h"
#include "proto_parse.h"
#include "proto_go.h"
using namespace std;

void Test();

int main()
{
	string strSource = "E:\\project_pet\\intermediate\\Proto\\cs_proto.proto";
	string strDest = "E:\\project_pet\\intermediate\\Proto\\cs_proto.go";

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

	CProtoGolang::Instance()->GenGoCode(vecProtoMsg, strDest, "Pet");
	system("pause");
	return 0;
}

void Test()
{
	std::map<int32, std::string> mapErase;
	mapErase[1] = "1";
	mapErase[2] = "2";
	mapErase[3] = "3";
	mapErase[4] = "4";

	auto itErase = mapErase.begin();
	for (; itErase != mapErase.end(); )
	{
		auto & strVal = itErase->second;
		itErase++;
		if (strVal == "3")
		{
			mapErase.erase(3);
		}
	}

	for (auto & itErase : mapErase)
	{
		std::cout << "first : " << itErase.first << " second : " << itErase.second << std::endl;
	}
}