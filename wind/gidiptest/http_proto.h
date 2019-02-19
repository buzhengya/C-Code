#pragma once
#include "common.h"
#include "singleton.h"
#include<unordered_map>

using namespace wind;
using namespace std;

class CHttpProto : public TSingleton<CHttpProto>
{
	//friend class TSingleton<CHttpProto>;
public:
	CHttpProto() {}

	~CHttpProto() {}

	bool Init(const string strHostIp);

	string GetErrMsg(ErrCode eErrCode);

	string GetProtoHeader(string strSign, int32 nLen);

	string GetMd5Sign(string strBody);

	RspResult GetRspResult(string strRsp);

	RspResult DealReq(string strReq);

private:

	int32 GetNum(string strSrc, string strKey);

	string GetStr(string strSrc, string strKey);

	unordered_map<ErrCode, string> m_mapErrMsg;

	string m_strHostIp;
};