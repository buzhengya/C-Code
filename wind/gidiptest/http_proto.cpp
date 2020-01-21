#include "http_proto.h"
#include "http_net.h"
#include "md5.h"
#include<iostream>
#define BUF_SIZE 4096

bool CHttpProto::Init(const string strHostIp)
{
	m_mapErrMsg[CHECK_SIGN_FAILED] = "check sign failed";

	m_mapErrMsg[SUCCESS] = "success";
	m_mapErrMsg[PLAT_NOT_EXIST] = "plat is not exist";
	m_mapErrMsg[PARTITION_NOT_EXIST] = "partition not exist";
	m_mapErrMsg[OPENID_NOT_EXIST] = "role or openid not exist";
	m_mapErrMsg[ACTIVE_USER_AREA_ERROR] = "active user area error";
	m_mapErrMsg[ACTIVE_USER_TYPE_ERROR] = "active user type error";
	m_mapErrMsg[DEL_WHITE_LIST_AREA_ERROR] = "del white list area error";
	m_mapErrMsg[DEL_WHITE_LIST_TYPE_ERROR] = "del white list type error";
	m_mapErrMsg[ITEM_NUM_IS_INVALID] = "item num is invalid";
	m_mapErrMsg[GIDIP_ITEM_ID_ERROR] = "item id error";
	m_mapErrMsg[BEGIN_EFFECT_TIME_INVALID] = "begin effect time is less than now time";
	m_mapErrMsg[END_EFFECT_TIME_INVALID] = "end effect time is less than begin effect time";
	m_mapErrMsg[BAN_TIME_IS_INVALID] = "ban time is invalid. begin time must less than now and end time contrarily.";
	m_mapErrMsg[GUIDE_ID_ERROR] = "guide id error";
	m_mapErrMsg[PVE_CHPATER_TYPE_ERROR] = "pve chapter type error";
	m_mapErrMsg[PVE_CHAPTER_ID_ERROR] = "pve chapter id error";
	m_mapErrMsg[PVE_STAGE_ID_ERROR] = "pve stage id error";
	m_mapErrMsg[PVE_CHAPTER_STAR_ERROR] = "pve chapter star error";

	m_strHostIp = strHostIp;
	return true;
}

string CHttpProto::GetErrMsg(ErrCode eErrCode)
{
	if (m_mapErrMsg.find(eErrCode) != m_mapErrMsg.end())
	{
		return m_mapErrMsg[eErrCode];
	}
	return "";
}

std::string CHttpProto::GetProtoHeader(string strSign, int32 nLen)
{
	string strHeader = "POST /v1?idip_sign=" + strSign + " HTTP/1.1\r\n";
	strHeader += "Host: " + m_strHostIp + "\r\n";
	strHeader += "Accept: */*\r\n";
	strHeader += "Content-Type: text/html\r\n";
	strHeader += "Content-Length: " + to_string(nLen) + "\r\n";
	strHeader += "\r\n";
	return strHeader;
}

std::string CHttpProto::GetMd5Sign(string strBody)
{
	return MD5(strBody + "553a877573611891341549352a464343").toStr();
}

RspResult CHttpProto::GetRspResult(string strRsp)
{
	//如果返回结果中含中文字符 成功但无返回信息
	for (const auto & it : strRsp)
	{
		if (it < 0)
		{
			return RspResult{ SUCCESS,"" };
		}
	}
	int nBody = strRsp.find("\"body\"");
	string strBody = "";
	if (nBody != string::npos)
	{
		strBody = strRsp.substr(nBody);
	}

	RspResult oHead;
	oHead.eErrCode = ErrCode(GetNum(strRsp, "\"Result\""));
	oHead.strRetMsg = GetStr(strRsp, "\"RetErrMsg\"");

	if (oHead.eErrCode != SUCCESS)
	{
		return oHead;
	}

	RspResult oBody;
	oBody.eErrCode = ErrCode(GetNum(strBody, "\"Result\""));
	oBody.strRetMsg = GetStr(strBody, "\"RetMsg\"");
	return oBody;
}



RspResult CHttpProto::DealReq(string strReq)
{
	strReq = "data_packet={" + strReq + "}";
	//cout << strReq << endl;
	string strSend = GetProtoHeader(GetMd5Sign(strReq), strReq.size()) + strReq;
	//cout << strSend << endl;
	int nSendSize = strSend.size();
	char szRecv[BUF_SIZE], szSend[BUF_SIZE];
	szRecv[0] = '\0';

	strcpy(szSend, strSend.c_str());
	szSend[nSendSize] = '\0';

	CHttpNet::Instance()->SendMsg(szSend, nSendSize, szRecv, BUF_SIZE - 1);
	szRecv[BUF_SIZE - 1] = '\0';
	//cout << szSend << endl;
	//cout << szRecv << endl;

	return GetRspResult(szRecv);
}

// "Result" : 10,  or "Result" : -10,
int32 CHttpProto::GetNum(string strSrc, string strKey)
{
	int nPos = strSrc.find(strKey);
	if (nPos == string::npos)
	{
		return 0;
	}

	int nSt = 0, nEnd = 0;
	for (int i = nPos; i < strSrc.size(); i++)
	{
		if (strSrc[i] ==':')
		{
			nSt = i + 1;
			break;
		}
	}

	for (int i = nPos; i < strSrc.size(); i++)
	{
		if (strSrc[i] == ',')
		{
			nEnd = i;
			break;
		}
	}

	while (nSt <= strSrc.size() && (strSrc[nSt]<'0' || strSrc[nSt] >'9') && strSrc[nSt] != '-') nSt++;

	while (nEnd >= 0 && (strSrc[nEnd]<'0' || strSrc[nEnd] >'9')) nEnd--;
	nEnd++;


	if (nSt >= nEnd)
	{
		return 0;
	}

	return std::stoi(strSrc.substr(nSt, nEnd - nSt));
}


// "RetErrMsg" : "success", or  "RetErrMsg" : "success"}
string CHttpProto::GetStr(string strSrc, string strKey)
{
	int nPos = strSrc.find(strKey);
	if (nPos == string::npos)
	{
		return "";
	}

	int nSt = 0, nEnd = 0;
	for (int i = nPos; i < strSrc.size(); i++)
	{
		if (strSrc[i] == ':')
		{
			nSt = i + 1;
			break;
		}
	}

	for (int i = nPos; i < strSrc.size(); i++)
	{
		if (strSrc[i] == ',' || strSrc[i] == '}')
		{
			nEnd = i;
			break;
		}
	}

	while (nSt <= strSrc.size() && strSrc[nSt] != '"') nSt++;
	nSt++;

	while (nEnd >= 0 && strSrc[nEnd] != '"') nEnd--;


	if (nSt >= nEnd)
	{
		return "";
	}

	return strSrc.substr(nSt, nEnd - nSt);
}
