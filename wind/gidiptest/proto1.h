#pragma once
#include "base_define.h"
#include<string>
#include "http_proto.h"
#include "gtest/gtest.h"
#include "common.h"
#pragma warning(disable : 4996)
using namespace std;

#define RIGHT_AREA 1
#define WRONG_AREA 2
#define RIGHT_PLAT 1
#define WRONG_PLAT 2
#define RIGHT_PARTITION 888
#define WRONG_PARTITION 99
//#define RIGHT_OPENID "o-u2I05RE5WuvD5HVRMZWrLy1yTw"
#define RIGHT_OPENID "dalao"
#define WRONG_OPENID "illegal"
#define ROLEID 0
#define POSITIVE_VAL 12
#define ZERO_VAL 0
#define NEGATIVE_VAL -10

string GetHeadStr(uint32 nCmdId)
{
	char szHead[1024];
	string strFormat = "\
		\"head\" :\
		{\
			\"PacketLen\" : 0,\
				\"Cmdid\" : %d, \
				\"Seqid\" : 0,\
				\"ServiceName\" : \"\",\
				\"SendTime\" : 0,\
				\"Version\" : 0,\
				\"Authenticate\" : \"\",\
				\"Result\" : 0, \
				\"RetErrMsg\" : \"\"\
		},";
	sprintf(szHead, strFormat.c_str(), nCmdId);
	return string(szHead);
}

uint32 GetNowTime()
{
	return time(0);
}

//修改铜钱
bool TestDoUpdateMoneyReq(ErrCode eErrCode, uint32 nAreaId, uint32 nPartition, uint32 nPlatId, char * szOpenId, uint64 nRoleId, int32 nValue)
{
	string strFormat = "\"body\" :{";

	strFormat += "\"AreaId\" : %d,";
	strFormat += "\"Partition\" : %d,";
	strFormat += "\"PlatId\" : %d,";
	strFormat += "\"OpenId\" : \"%s\",";
	strFormat += "\"RoleId\" : %lld,";
	strFormat += "\"Value\" : %d,";

	strFormat += "\"Source\" : 0, ";
	strFormat += "\"Serial\" : \"\"}";

	char szBody[2048];
	sprintf(szBody, strFormat.c_str(), nAreaId, nPartition, nPlatId, szOpenId, nRoleId, nValue);

	string strRetMsg = CHttpProto::Instance()->GetErrMsg(eErrCode);
	
	//RspResult oRet = CHttpProto::Instance()->DealReq(GetHeadStr(4097) + string(szBody));
	//bool bRet = RspResult{ eErrCode,strRetMsg } == oRet;
	//return bRet;
	return RspResult{ eErrCode,strRetMsg } == CHttpProto::Instance()->DealReq(GetHeadStr(4097) + string(szBody));
}


TEST(TestDoUpdateMoneyReq, SUCCESS)
{
	EXPECT_TRUE(TestDoUpdateMoneyReq(SUCCESS, RIGHT_AREA, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, ROLEID, POSITIVE_VAL));
	EXPECT_TRUE(TestDoUpdateMoneyReq(SUCCESS, RIGHT_AREA, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, ROLEID, ZERO_VAL));
	EXPECT_TRUE(TestDoUpdateMoneyReq(SUCCESS, RIGHT_AREA, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, ROLEID, NEGATIVE_VAL));
}

TEST(TestDoUpdateMoneyReq, PLAT_NOT_EXIST)
{
	EXPECT_TRUE(TestDoUpdateMoneyReq(PLAT_NOT_EXIST, WRONG_AREA, RIGHT_PARTITION, WRONG_PLAT, RIGHT_OPENID, ROLEID, POSITIVE_VAL));
	EXPECT_TRUE(TestDoUpdateMoneyReq(PLAT_NOT_EXIST, WRONG_AREA, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, ROLEID, ZERO_VAL));
	EXPECT_TRUE(TestDoUpdateMoneyReq(PLAT_NOT_EXIST, RIGHT_AREA, RIGHT_PARTITION, WRONG_PLAT, RIGHT_OPENID, ROLEID, NEGATIVE_VAL));
}

TEST(TestDoUpdateMoneyReq, PARTITION_NOT_EXIST)
{
	EXPECT_TRUE(TestDoUpdateMoneyReq(PARTITION_NOT_EXIST, RIGHT_AREA, WRONG_PARTITION, RIGHT_PLAT, RIGHT_OPENID, ROLEID, POSITIVE_VAL));
}

TEST(TestDoUpdateMoneyReq, OPENID_NOT_EXIST)
{
	EXPECT_TRUE(TestDoUpdateMoneyReq(OPENID_NOT_EXIST, RIGHT_AREA, RIGHT_PARTITION, RIGHT_PLAT, WRONG_OPENID, ROLEID, POSITIVE_VAL));
}

//修改元宝
bool TestDoUpdateCashReq(ErrCode eErrCode, uint32 nAreaId, uint32 nPartition, uint32 nPlatId, char * szOpenId, uint64 nRoleId, int32 nValue)
{
	string strFormat = "\"body\" :{";

	strFormat += "\"AreaId\" : %d,";
	strFormat += "\"Partition\" : %d,";
	strFormat += "\"PlatId\" : %d,";
	strFormat += "\"OpenId\" : \"%s\",";
	strFormat += "\"RoleId\" : %lld,";
	strFormat += "\"Value\" : %d,";

	strFormat += "\"Source\" : 0, ";
	strFormat += "\"Serial\" : \"\"}";

	char szBody[2048];
	sprintf(szBody, strFormat.c_str(), nAreaId, nPartition, nPlatId, szOpenId, nRoleId, nValue);

	string strRetMsg = CHttpProto::Instance()->GetErrMsg(eErrCode);

	//RspResult oRet = CHttpProto::Instance()->DealReq(GetHeadStr(4097) + string(szBody));
	//bool bRet = RspResult{ eErrCode,strRetMsg } == oRet;
	//return bRet;
	return RspResult{ eErrCode,strRetMsg } == CHttpProto::Instance()->DealReq(GetHeadStr(4099) + string(szBody));
}


TEST(TestDoUpdateCashReq, SUCCESS)
{
	EXPECT_TRUE(TestDoUpdateCashReq(SUCCESS, RIGHT_AREA, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, ROLEID, POSITIVE_VAL));
	EXPECT_TRUE(TestDoUpdateCashReq(SUCCESS, RIGHT_AREA, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, ROLEID, ZERO_VAL));
	EXPECT_TRUE(TestDoUpdateCashReq(SUCCESS, RIGHT_AREA, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, ROLEID, NEGATIVE_VAL));
}

TEST(TestDoUpdateCashReq, PLAT_NOT_EXIST)
{
	EXPECT_TRUE(TestDoUpdateCashReq(PLAT_NOT_EXIST, WRONG_AREA, RIGHT_PARTITION, WRONG_PLAT, RIGHT_OPENID, ROLEID, POSITIVE_VAL));
	EXPECT_TRUE(TestDoUpdateCashReq(PLAT_NOT_EXIST, WRONG_AREA, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, ROLEID, ZERO_VAL));
	EXPECT_TRUE(TestDoUpdateCashReq(PLAT_NOT_EXIST, RIGHT_AREA, RIGHT_PARTITION, WRONG_PLAT, RIGHT_OPENID, ROLEID, NEGATIVE_VAL));
}

TEST(TestDoUpdateCashReq, PARTITION_NOT_EXIST)
{
	EXPECT_TRUE(TestDoUpdateCashReq(PARTITION_NOT_EXIST, RIGHT_AREA, WRONG_PARTITION, RIGHT_PLAT, RIGHT_OPENID, ROLEID, POSITIVE_VAL));
}

TEST(TestDoUpdateCashReq, OPENID_NOT_EXIST)
{
	EXPECT_TRUE(TestDoUpdateCashReq(OPENID_NOT_EXIST, RIGHT_AREA, RIGHT_PARTITION, RIGHT_PLAT, WRONG_OPENID, ROLEID, POSITIVE_VAL));
}

//修改体力
bool TestDoUpdatePowerReq(ErrCode eErrCode, uint32 nAreaId, uint32 nPartition, uint32 nPlatId, char * szOpenId, uint64 nRoleId, int32 nValue)
{
	string strFormat = "\"body\" :{";

	strFormat += "\"AreaId\" : %d,";
	strFormat += "\"Partition\" : %d,";
	strFormat += "\"PlatId\" : %d,";
	strFormat += "\"OpenId\" : \"%s\",";
	strFormat += "\"RoleId\" : %lld,";
	strFormat += "\"Value\" : %d,";

	strFormat += "\"Source\" : 0, ";
	strFormat += "\"Serial\" : \"\"}";

	char szBody[2048];
	sprintf(szBody, strFormat.c_str(), nAreaId, nPartition, nPlatId, szOpenId, nRoleId, nValue);

	string strRetMsg = CHttpProto::Instance()->GetErrMsg(eErrCode);

	//RspResult oRet = CHttpProto::Instance()->DealReq(GetHeadStr(4097) + string(szBody));
	//bool bRet = RspResult{ eErrCode,strRetMsg } == oRet;
	//return bRet;
	return RspResult{ eErrCode,strRetMsg } == CHttpProto::Instance()->DealReq(GetHeadStr(4101) + string(szBody));
}


TEST(TestDoUpdatePowerReq, SUCCESS)
{
	EXPECT_TRUE(TestDoUpdatePowerReq(SUCCESS, RIGHT_AREA, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, ROLEID, POSITIVE_VAL));
	EXPECT_TRUE(TestDoUpdatePowerReq(SUCCESS, RIGHT_AREA, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, ROLEID, ZERO_VAL));
	EXPECT_TRUE(TestDoUpdatePowerReq(SUCCESS, RIGHT_AREA, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, ROLEID, NEGATIVE_VAL));
}

TEST(TestDoUpdatePowerReq, PLAT_NOT_EXIST)
{
	EXPECT_TRUE(TestDoUpdatePowerReq(PLAT_NOT_EXIST, WRONG_AREA, RIGHT_PARTITION, WRONG_PLAT, RIGHT_OPENID, ROLEID, POSITIVE_VAL));
	EXPECT_TRUE(TestDoUpdatePowerReq(PLAT_NOT_EXIST, WRONG_AREA, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, ROLEID, ZERO_VAL));
	EXPECT_TRUE(TestDoUpdatePowerReq(PLAT_NOT_EXIST, RIGHT_AREA, RIGHT_PARTITION, WRONG_PLAT, RIGHT_OPENID, ROLEID, NEGATIVE_VAL));
}

TEST(TestDoUpdatePowerReq, PARTITION_NOT_EXIST)
{
	EXPECT_TRUE(TestDoUpdatePowerReq(PARTITION_NOT_EXIST, RIGHT_AREA, WRONG_PARTITION, RIGHT_PLAT, RIGHT_OPENID, ROLEID, POSITIVE_VAL));
}

TEST(TestDoUpdatePowerReq, OPENID_NOT_EXIST)
{
	EXPECT_TRUE(TestDoUpdatePowerReq(OPENID_NOT_EXIST, RIGHT_AREA, RIGHT_PARTITION, RIGHT_PLAT, WRONG_OPENID, ROLEID, POSITIVE_VAL));
}

#define VALID_ITEM_ID 40002
#define INVALID_ITEM_ID 99
#define VALID_ITEM_NUM 1
#define GOLD_ID 101
#define JADE_ID 102
#define POWER_ID 103

#define MAIL_TITLE_1 "%E5%A4%9A%E9%81%93%E5%85%B7%E8%B5%A0%E9%80%81%E6%B5%8B%E8%AF%95%E6%A0%87%E9%A2%98"
#define MAIL_TITLE_2 "test send many item title"
#define MAIL_CONTENT_1 "%E5%A4%9A%E9%81%93%E5%85%B7%E8%B5%A0%E9%80%81%E6%B5%8B%E8%AF%95%E5%86%85%E5%AE%B9"
#define MAIL_CONTENT_2 "test send many item content"
#define RIGHT_SEND_ITEM_COUNT 2
//赠送道具
bool TestDoSendItemReq(ErrCode eErrCode, uint32 nAreaId, uint32 nPartition, uint32 nPlatId, char * szOpenId, uint64 nRoleId,
	char * szMailTitle, char * szMailContent, int32 nCount, int32 nItemId, int32 nItemNum)
{
	string strFormat = "\"body\" :{";

	strFormat += "\"AreaId\" : %d,";
	strFormat += "\"Partition\" : %d,";
	strFormat += "\"PlatId\" : %d,";
	strFormat += "\"OpenId\" : \"%s\",";
	strFormat += "\"RoleId\" : %lld,";
	strFormat += "\"MailTitle\" : \"%s\",";
	strFormat += "\"MailContent\" : \"%s\",";
	strFormat += "\"ItemList_count\" : %d,";

	strFormat += "\"ItemList\" :[";

	strFormat += "{\"ItemId\" : 101,";
	strFormat += "\"ItemNum\" : 1,";
	strFormat += "\"ItemLevel\" : 0},";

	strFormat += "{\"ItemId\" : %d,";
	strFormat += "\"ItemNum\" : %d,";
	strFormat += "\"ItemLevel\" : 0}";

	strFormat += "],";

	strFormat += "\"Source\" : 0, ";
	strFormat += "\"Serial\" : \"\"}";

	char szBody[2048];
	sprintf(szBody, strFormat.c_str(), nAreaId, nPartition, nPlatId, szOpenId, nRoleId,szMailTitle,szMailContent,nCount, nItemId, nItemNum);

	string strRetMsg = CHttpProto::Instance()->GetErrMsg(eErrCode);

	//RspResult oRet = CHttpProto::Instance()->DealReq(GetHeadStr(4097) + string(szBody));
	//bool bRet = RspResult{ eErrCode,strRetMsg } == oRet;
	//return bRet;
	return RspResult{ eErrCode,strRetMsg } == CHttpProto::Instance()->DealReq(GetHeadStr(4103) + string(szBody));
}


TEST(TestDoSendItemReq, SUCCESS)
{
	EXPECT_TRUE(TestDoSendItemReq(SUCCESS, RIGHT_AREA, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, ROLEID, MAIL_TITLE_1,MAIL_CONTENT_1,
		RIGHT_SEND_ITEM_COUNT, GOLD_ID, VALID_ITEM_NUM));
	EXPECT_TRUE(TestDoSendItemReq(SUCCESS, RIGHT_AREA, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, ROLEID, MAIL_TITLE_1, MAIL_CONTENT_1,
		RIGHT_SEND_ITEM_COUNT, JADE_ID, VALID_ITEM_NUM));
	EXPECT_TRUE(TestDoSendItemReq(SUCCESS, RIGHT_AREA, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, ROLEID, MAIL_TITLE_2, MAIL_CONTENT_2,
		RIGHT_SEND_ITEM_COUNT, VALID_ITEM_ID, VALID_ITEM_NUM));
	EXPECT_TRUE(TestDoSendItemReq(SUCCESS, RIGHT_AREA, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, ROLEID, MAIL_TITLE_2, MAIL_CONTENT_2,
		RIGHT_SEND_ITEM_COUNT, POWER_ID, VALID_ITEM_NUM));
}

TEST(TestDoSendItemReq, PLAT_NOT_EXIST)
{
	EXPECT_TRUE(TestDoSendItemReq(PLAT_NOT_EXIST, RIGHT_AREA, RIGHT_PARTITION, WRONG_PLAT, RIGHT_OPENID, ROLEID, MAIL_TITLE_1, MAIL_CONTENT_1,
		RIGHT_SEND_ITEM_COUNT, VALID_ITEM_ID, VALID_ITEM_NUM));
	EXPECT_TRUE(TestDoSendItemReq(PLAT_NOT_EXIST, WRONG_AREA, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, ROLEID, MAIL_TITLE_1, MAIL_CONTENT_1,
		RIGHT_SEND_ITEM_COUNT, VALID_ITEM_ID, VALID_ITEM_NUM));
	EXPECT_TRUE(TestDoSendItemReq(PLAT_NOT_EXIST, WRONG_AREA, RIGHT_PARTITION, WRONG_PLAT, RIGHT_OPENID, ROLEID, MAIL_TITLE_1, MAIL_CONTENT_1,
		RIGHT_SEND_ITEM_COUNT, VALID_ITEM_ID, VALID_ITEM_NUM));
	EXPECT_TRUE(TestDoSendItemReq(PLAT_NOT_EXIST, RIGHT_AREA, RIGHT_PARTITION, WRONG_PLAT, RIGHT_OPENID, ROLEID, MAIL_TITLE_2, MAIL_CONTENT_2,
		RIGHT_SEND_ITEM_COUNT, VALID_ITEM_ID, VALID_ITEM_NUM));
	EXPECT_TRUE(TestDoSendItemReq(PLAT_NOT_EXIST, WRONG_AREA, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, ROLEID, MAIL_TITLE_2, MAIL_CONTENT_2,
		RIGHT_SEND_ITEM_COUNT, VALID_ITEM_ID, VALID_ITEM_NUM));
	EXPECT_TRUE(TestDoSendItemReq(PLAT_NOT_EXIST, WRONG_AREA, RIGHT_PARTITION, WRONG_PLAT, RIGHT_OPENID, ROLEID, MAIL_TITLE_2, MAIL_CONTENT_2,
		RIGHT_SEND_ITEM_COUNT, VALID_ITEM_ID, VALID_ITEM_NUM));
}

TEST(TestDoSendItemReq, PARTITION_NOT_EXIST)
{
	EXPECT_TRUE(TestDoSendItemReq(PARTITION_NOT_EXIST, RIGHT_AREA, WRONG_PARTITION, RIGHT_PLAT, RIGHT_OPENID, ROLEID, MAIL_TITLE_1, MAIL_CONTENT_1,
		RIGHT_SEND_ITEM_COUNT, VALID_ITEM_ID, VALID_ITEM_NUM));
	EXPECT_TRUE(TestDoSendItemReq(PARTITION_NOT_EXIST, RIGHT_AREA, WRONG_PARTITION, RIGHT_PLAT, RIGHT_OPENID, ROLEID, MAIL_TITLE_2, MAIL_CONTENT_2,
		RIGHT_SEND_ITEM_COUNT, VALID_ITEM_ID, VALID_ITEM_NUM));
}

TEST(TestDoSendItemReq, OPENID_NOT_EXIST)
{
	EXPECT_TRUE(TestDoSendItemReq(OPENID_NOT_EXIST, RIGHT_AREA, RIGHT_PARTITION, RIGHT_PLAT, WRONG_OPENID, ROLEID, MAIL_TITLE_1, MAIL_CONTENT_1,
		RIGHT_SEND_ITEM_COUNT, VALID_ITEM_ID, VALID_ITEM_NUM));
	EXPECT_TRUE(TestDoSendItemReq(OPENID_NOT_EXIST, RIGHT_AREA, RIGHT_PARTITION, RIGHT_PLAT, WRONG_OPENID, ROLEID, MAIL_TITLE_2, MAIL_CONTENT_2,
		RIGHT_SEND_ITEM_COUNT, VALID_ITEM_ID, VALID_ITEM_NUM));
}

TEST(TestDoSendItemReq, ITEM_NUM_IS_INVALID)
{
	EXPECT_TRUE(TestDoSendItemReq(ITEM_NUM_IS_INVALID, RIGHT_AREA, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, ROLEID, MAIL_TITLE_1, MAIL_CONTENT_1,
		RIGHT_SEND_ITEM_COUNT, VALID_ITEM_ID, 0));
	EXPECT_TRUE(TestDoSendItemReq(ITEM_NUM_IS_INVALID, RIGHT_AREA, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, ROLEID, MAIL_TITLE_2, MAIL_CONTENT_2,
		RIGHT_SEND_ITEM_COUNT, VALID_ITEM_ID, 0));
	EXPECT_TRUE(TestDoSendItemReq(ITEM_NUM_IS_INVALID, RIGHT_AREA, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, ROLEID, MAIL_TITLE_1, MAIL_CONTENT_1,
		RIGHT_SEND_ITEM_COUNT, VALID_ITEM_ID, -2));
	EXPECT_TRUE(TestDoSendItemReq(ITEM_NUM_IS_INVALID, RIGHT_AREA, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, ROLEID, MAIL_TITLE_2, MAIL_CONTENT_2,
		RIGHT_SEND_ITEM_COUNT, VALID_ITEM_ID, -2));
}

TEST(TestDoSendItemReq, GIDIP_ITEM_ID_ERROR)
{
	EXPECT_TRUE(TestDoSendItemReq(GIDIP_ITEM_ID_ERROR, RIGHT_AREA, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, ROLEID, MAIL_TITLE_1, MAIL_CONTENT_1,
		RIGHT_SEND_ITEM_COUNT, INVALID_ITEM_ID, VALID_ITEM_NUM));
	EXPECT_TRUE(TestDoSendItemReq(GIDIP_ITEM_ID_ERROR, RIGHT_AREA, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, ROLEID, MAIL_TITLE_2, MAIL_CONTENT_2,
		RIGHT_SEND_ITEM_COUNT, INVALID_ITEM_ID, VALID_ITEM_NUM));
	EXPECT_TRUE(TestDoSendItemReq(GIDIP_ITEM_ID_ERROR, RIGHT_AREA, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, ROLEID, MAIL_TITLE_1, MAIL_CONTENT_1,
		RIGHT_SEND_ITEM_COUNT, -10, VALID_ITEM_NUM));
	EXPECT_TRUE(TestDoSendItemReq(GIDIP_ITEM_ID_ERROR, RIGHT_AREA, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, ROLEID, MAIL_TITLE_2, MAIL_CONTENT_2,
		RIGHT_SEND_ITEM_COUNT, -10, VALID_ITEM_NUM));
}

#define VALID_BEGIN_TIME GetNowTime()+10
#define VALID_END_TIME 1580490061 // 2020
#define VALID_MONEY_NUM 3
#define VALID_COIN_NUM 4
#define INVALID_BEGIN_TIME GetNowTime() - 10
#define INVALID_END_TIME GetNowTime() - 5
//发送全服邮件
bool TestDoSendMailAllServerReq(ErrCode eErrCode, uint32 nAreaId, uint32 nPartition, uint32 nPlatId,int64 nBeginTm, int64 nEndTm, 
	char * szMailTitle, char * szMailContent, int32 nMoneyNum, int32 nCoinNum, int32 nCount, int32 nItemId, int32 nItemNum)
{
	string strFormat = "\"body\" :{";

	strFormat += "\"AreaId\" : %d,";
	strFormat += "\"Partition\" : %d,";
	strFormat += "\"PlatId\" : %d,";
	strFormat += "\"BeginEffectTime\" : %lld,";
	strFormat += "\"EndEffectTime\" : %lld,";
	strFormat += "\"MailTitle\" : \"%s\",";
	strFormat += "\"MailContent\" : \"%s\",";
	strFormat += "\"MoneyNum\" : %d,";
	strFormat += "\"CoinNum\" : %d,";
	strFormat += "\"Accessory_count\" : %d,";

	strFormat += "\"Accessory\" :[";

	strFormat += "{\"ItemId\" : 101,";
	strFormat += "\"ItemNum\" : 1},";

	strFormat += "{\"ItemId\" : %d,";
	strFormat += "\"ItemNum\" : %d}";

	strFormat += "],";

	strFormat += "\"Source\" : 0, ";
	strFormat += "\"Serial\" : \"\"}";

	char szBody[2048];
	sprintf(szBody, strFormat.c_str(), nAreaId, nPartition, nPlatId, nBeginTm, nEndTm, szMailTitle, szMailContent,
		nMoneyNum, nCoinNum, nCount, nItemId, nItemNum);

	string strRetMsg = CHttpProto::Instance()->GetErrMsg(eErrCode);

	//RspResult oRet = CHttpProto::Instance()->DealReq(GetHeadStr(4097) + string(szBody));
	//bool bRet = RspResult{ eErrCode,strRetMsg } == oRet;
	//return bRet;
	return RspResult{ eErrCode,strRetMsg } == CHttpProto::Instance()->DealReq(GetHeadStr(4123) + string(szBody));
}

#define MAIL_ALL_SERVER_TITLE_1 "%E5%85%A8%E6%9C%8D%E9%82%AE%E4%BB%B6%E6%B5%8B%E8%AF%95%E6%A0%87%E9%A2%98"
#define MAIL_ALL_SERVER_TITLE_2 "test all server mail title"
#define MAIL_ALL_SERVER_CONTENT_1 "%E5%85%A8%E6%9C%8D%E9%82%AE%E4%BB%B6%E6%B5%8B%E8%AF%95%E5%86%85%E5%AE%B9"
#define MAIL_ALL_SERVER_CONTENT_2 "test all server mail content"

TEST(TestDoSendMailAllServerReq, SUCCESS)
{
	EXPECT_TRUE(TestDoSendMailAllServerReq(SUCCESS, RIGHT_AREA, RIGHT_PARTITION, RIGHT_PLAT, VALID_BEGIN_TIME, VALID_END_TIME, MAIL_ALL_SERVER_TITLE_1, MAIL_ALL_SERVER_CONTENT_1,
		VALID_MONEY_NUM, VALID_COIN_NUM, RIGHT_SEND_ITEM_COUNT, VALID_ITEM_ID, VALID_ITEM_NUM));
	EXPECT_TRUE(TestDoSendMailAllServerReq(SUCCESS, RIGHT_AREA, RIGHT_PARTITION, RIGHT_PLAT, VALID_BEGIN_TIME, VALID_END_TIME, MAIL_ALL_SERVER_TITLE_1, MAIL_ALL_SERVER_CONTENT_1,
		0, VALID_COIN_NUM, RIGHT_SEND_ITEM_COUNT, VALID_ITEM_ID, VALID_ITEM_NUM));
	EXPECT_TRUE(TestDoSendMailAllServerReq(SUCCESS, RIGHT_AREA, RIGHT_PARTITION, RIGHT_PLAT, VALID_BEGIN_TIME, VALID_END_TIME, MAIL_ALL_SERVER_TITLE_2, MAIL_ALL_SERVER_CONTENT_2,
		VALID_MONEY_NUM, 0, RIGHT_SEND_ITEM_COUNT, VALID_ITEM_ID, VALID_ITEM_NUM));
	EXPECT_TRUE(TestDoSendMailAllServerReq(SUCCESS, RIGHT_AREA, RIGHT_PARTITION, RIGHT_PLAT, VALID_BEGIN_TIME, VALID_END_TIME, MAIL_ALL_SERVER_TITLE_2, MAIL_ALL_SERVER_CONTENT_2,
		0, 0, RIGHT_SEND_ITEM_COUNT, VALID_ITEM_ID, VALID_ITEM_NUM));
}

TEST(TestDoSendMailAllServerReq, PLAT_NOT_EXIST)
{
	EXPECT_TRUE(TestDoSendMailAllServerReq(PLAT_NOT_EXIST, RIGHT_AREA, RIGHT_PARTITION, WRONG_PLAT, VALID_BEGIN_TIME, VALID_END_TIME, MAIL_ALL_SERVER_TITLE_1, MAIL_ALL_SERVER_CONTENT_1,
		VALID_MONEY_NUM, VALID_COIN_NUM, RIGHT_SEND_ITEM_COUNT, VALID_ITEM_ID, VALID_ITEM_NUM));
	EXPECT_TRUE(TestDoSendMailAllServerReq(PLAT_NOT_EXIST, WRONG_AREA, RIGHT_PARTITION, RIGHT_PLAT, VALID_BEGIN_TIME, VALID_END_TIME, MAIL_ALL_SERVER_TITLE_1, MAIL_ALL_SERVER_CONTENT_1,
		VALID_MONEY_NUM, VALID_COIN_NUM, RIGHT_SEND_ITEM_COUNT, VALID_ITEM_ID, VALID_ITEM_NUM));
	EXPECT_TRUE(TestDoSendMailAllServerReq(PLAT_NOT_EXIST, WRONG_AREA, RIGHT_PARTITION, WRONG_PLAT, VALID_BEGIN_TIME, VALID_END_TIME, MAIL_ALL_SERVER_TITLE_1, MAIL_ALL_SERVER_CONTENT_1,
		VALID_MONEY_NUM, VALID_COIN_NUM, RIGHT_SEND_ITEM_COUNT, VALID_ITEM_ID, VALID_ITEM_NUM));
	EXPECT_TRUE(TestDoSendMailAllServerReq(PLAT_NOT_EXIST, RIGHT_AREA, RIGHT_PARTITION, WRONG_PLAT, VALID_BEGIN_TIME, VALID_END_TIME, MAIL_ALL_SERVER_TITLE_2, MAIL_ALL_SERVER_CONTENT_2,
		VALID_MONEY_NUM, VALID_COIN_NUM, RIGHT_SEND_ITEM_COUNT, VALID_ITEM_ID, VALID_ITEM_NUM));
	EXPECT_TRUE(TestDoSendMailAllServerReq(PLAT_NOT_EXIST, WRONG_AREA, RIGHT_PARTITION, RIGHT_PLAT, VALID_BEGIN_TIME, VALID_END_TIME, MAIL_ALL_SERVER_TITLE_2, MAIL_ALL_SERVER_CONTENT_2,
		VALID_MONEY_NUM, VALID_COIN_NUM, RIGHT_SEND_ITEM_COUNT, VALID_ITEM_ID, VALID_ITEM_NUM));
	EXPECT_TRUE(TestDoSendMailAllServerReq(PLAT_NOT_EXIST, WRONG_AREA, RIGHT_PARTITION, WRONG_PLAT, VALID_BEGIN_TIME, VALID_END_TIME, MAIL_ALL_SERVER_TITLE_2, MAIL_ALL_SERVER_CONTENT_2,
		VALID_MONEY_NUM, VALID_COIN_NUM, RIGHT_SEND_ITEM_COUNT, VALID_ITEM_ID, VALID_ITEM_NUM));
}

TEST(TestDoSendMailAllServerReq, PARTITION_NOT_EXIST)
{
	EXPECT_TRUE(TestDoSendMailAllServerReq(PARTITION_NOT_EXIST, RIGHT_AREA, WRONG_PARTITION, RIGHT_PLAT, VALID_BEGIN_TIME, VALID_END_TIME, MAIL_ALL_SERVER_TITLE_1, MAIL_ALL_SERVER_CONTENT_1,
		VALID_MONEY_NUM, VALID_COIN_NUM, RIGHT_SEND_ITEM_COUNT, VALID_ITEM_ID, VALID_ITEM_NUM));
	EXPECT_TRUE(TestDoSendMailAllServerReq(PARTITION_NOT_EXIST, RIGHT_AREA, WRONG_PARTITION, RIGHT_PLAT, VALID_BEGIN_TIME, VALID_END_TIME, MAIL_ALL_SERVER_TITLE_2, MAIL_ALL_SERVER_CONTENT_2,
		VALID_MONEY_NUM, VALID_COIN_NUM, RIGHT_SEND_ITEM_COUNT, VALID_ITEM_ID, VALID_ITEM_NUM));
}

TEST(TestDoSendMailAllServerReq, ITEM_NUM_IS_INVALID)
{
	EXPECT_TRUE(TestDoSendMailAllServerReq(ITEM_NUM_IS_INVALID, RIGHT_AREA, RIGHT_PARTITION, RIGHT_PLAT, VALID_BEGIN_TIME, VALID_END_TIME, MAIL_ALL_SERVER_TITLE_1, MAIL_ALL_SERVER_CONTENT_1,
		VALID_MONEY_NUM, VALID_COIN_NUM, RIGHT_SEND_ITEM_COUNT, VALID_ITEM_ID, 0));
	EXPECT_TRUE(TestDoSendMailAllServerReq(ITEM_NUM_IS_INVALID, RIGHT_AREA, RIGHT_PARTITION, RIGHT_PLAT, VALID_BEGIN_TIME, VALID_END_TIME, MAIL_ALL_SERVER_TITLE_2, MAIL_ALL_SERVER_CONTENT_2,
		VALID_MONEY_NUM, VALID_COIN_NUM, RIGHT_SEND_ITEM_COUNT, VALID_ITEM_ID, 0));
	EXPECT_TRUE(TestDoSendMailAllServerReq(ITEM_NUM_IS_INVALID, RIGHT_AREA, RIGHT_PARTITION, RIGHT_PLAT, VALID_BEGIN_TIME, VALID_END_TIME, MAIL_ALL_SERVER_TITLE_1, MAIL_ALL_SERVER_CONTENT_1,
		VALID_MONEY_NUM, VALID_COIN_NUM, RIGHT_SEND_ITEM_COUNT, VALID_ITEM_ID, -2));
	EXPECT_TRUE(TestDoSendMailAllServerReq(ITEM_NUM_IS_INVALID, RIGHT_AREA, RIGHT_PARTITION, RIGHT_PLAT, VALID_BEGIN_TIME, VALID_END_TIME, MAIL_ALL_SERVER_TITLE_2, MAIL_ALL_SERVER_CONTENT_2,
		VALID_MONEY_NUM, VALID_COIN_NUM, RIGHT_SEND_ITEM_COUNT, VALID_ITEM_ID, -2));
}

TEST(TestDoSendMailAllServerReq, GIDIP_ITEM_ID_ERROR)
{
	EXPECT_TRUE(TestDoSendMailAllServerReq(GIDIP_ITEM_ID_ERROR, RIGHT_AREA, RIGHT_PARTITION, RIGHT_PLAT, VALID_BEGIN_TIME, VALID_END_TIME, MAIL_ALL_SERVER_TITLE_1, MAIL_ALL_SERVER_CONTENT_1,
		VALID_MONEY_NUM, VALID_COIN_NUM, RIGHT_SEND_ITEM_COUNT, INVALID_ITEM_ID, VALID_ITEM_NUM));
	EXPECT_TRUE(TestDoSendMailAllServerReq(GIDIP_ITEM_ID_ERROR, RIGHT_AREA, RIGHT_PARTITION, RIGHT_PLAT, VALID_BEGIN_TIME, VALID_END_TIME, MAIL_ALL_SERVER_TITLE_2, MAIL_ALL_SERVER_CONTENT_2,
		VALID_MONEY_NUM, VALID_COIN_NUM, RIGHT_SEND_ITEM_COUNT, INVALID_ITEM_ID, VALID_ITEM_NUM));
	EXPECT_TRUE(TestDoSendMailAllServerReq(GIDIP_ITEM_ID_ERROR, RIGHT_AREA, RIGHT_PARTITION, RIGHT_PLAT, VALID_BEGIN_TIME, VALID_END_TIME, MAIL_ALL_SERVER_TITLE_1, MAIL_ALL_SERVER_CONTENT_1,
		VALID_MONEY_NUM, VALID_COIN_NUM, RIGHT_SEND_ITEM_COUNT, -10, VALID_ITEM_NUM));
	EXPECT_TRUE(TestDoSendMailAllServerReq(GIDIP_ITEM_ID_ERROR, RIGHT_AREA, RIGHT_PARTITION, RIGHT_PLAT, VALID_BEGIN_TIME, VALID_END_TIME, MAIL_ALL_SERVER_TITLE_2, MAIL_ALL_SERVER_CONTENT_2,
		VALID_MONEY_NUM, VALID_COIN_NUM, RIGHT_SEND_ITEM_COUNT, -10, VALID_ITEM_NUM));
}

TEST(TestDoSendMailAllServerReq, INVALID_TIME)
{
	EXPECT_TRUE(TestDoSendMailAllServerReq(BEGIN_EFFECT_TIME_INVALID, RIGHT_AREA, RIGHT_PARTITION, RIGHT_PLAT, INVALID_BEGIN_TIME, VALID_END_TIME, MAIL_ALL_SERVER_TITLE_1, MAIL_ALL_SERVER_CONTENT_1,
		VALID_MONEY_NUM, VALID_COIN_NUM, RIGHT_SEND_ITEM_COUNT, VALID_ITEM_ID, VALID_ITEM_NUM));

	EXPECT_TRUE(TestDoSendMailAllServerReq(END_EFFECT_TIME_INVALID, RIGHT_AREA, RIGHT_PARTITION, RIGHT_PLAT, VALID_BEGIN_TIME, INVALID_END_TIME, MAIL_ALL_SERVER_TITLE_1, MAIL_ALL_SERVER_CONTENT_1,
		VALID_MONEY_NUM, VALID_COIN_NUM, RIGHT_SEND_ITEM_COUNT, VALID_ITEM_ID, VALID_ITEM_NUM));

	EXPECT_TRUE(TestDoSendMailAllServerReq(BEGIN_EFFECT_TIME_INVALID, RIGHT_AREA, RIGHT_PARTITION, RIGHT_PLAT, INVALID_BEGIN_TIME, VALID_END_TIME, MAIL_ALL_SERVER_TITLE_2, MAIL_ALL_SERVER_CONTENT_2,
		VALID_MONEY_NUM, VALID_COIN_NUM, RIGHT_SEND_ITEM_COUNT, VALID_ITEM_ID, VALID_ITEM_NUM));

	EXPECT_TRUE(TestDoSendMailAllServerReq(END_EFFECT_TIME_INVALID, RIGHT_AREA, RIGHT_PARTITION, RIGHT_PLAT, VALID_BEGIN_TIME, INVALID_END_TIME, MAIL_ALL_SERVER_TITLE_2, MAIL_ALL_SERVER_CONTENT_2,
		VALID_MONEY_NUM, VALID_COIN_NUM, RIGHT_SEND_ITEM_COUNT, VALID_ITEM_ID, VALID_ITEM_NUM));
}

//删除道具
bool TestDoDelItemReq(ErrCode eErrCode, uint32 nAreaId, uint32 nPartition, uint32 nPlatId, char * szOpenId, uint64 nRoleId, 
	int32 nItemId, int32 nItemNum)
{
	string strFormat = "\"body\" :{";

	strFormat += "\"AreaId\" : %d,";
	strFormat += "\"Partition\" : %d,";
	strFormat += "\"PlatId\" : %d,";
	strFormat += "\"OpenId\" : \"%s\",";
	strFormat += "\"RoleId\" : %lld,";
	strFormat += "\"ItemId\" : %d,";
	strFormat += "\"ItemNum\" : %d,";
	strFormat += "\"ItemLevel\" : 0,";

	strFormat += "\"Source\" : 0, ";
	strFormat += "\"Serial\" : \"\"}";

	char szBody[2048];
	sprintf(szBody, strFormat.c_str(), nAreaId, nPartition, nPlatId, szOpenId, nRoleId, nItemId, nItemNum);

	string strRetMsg = CHttpProto::Instance()->GetErrMsg(eErrCode);

	//RspResult oRet = CHttpProto::Instance()->DealReq(GetHeadStr(4097) + string(szBody));
	//bool bRet = RspResult{ eErrCode,strRetMsg } == oRet;
	//return bRet;
	return RspResult{ eErrCode,strRetMsg } == CHttpProto::Instance()->DealReq(GetHeadStr(4105) + string(szBody));
}


TEST(TestDoDelItemReq, SUCCESS)
{
	EXPECT_TRUE(TestDoDelItemReq(SUCCESS, RIGHT_AREA, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, ROLEID, VALID_ITEM_ID, VALID_ITEM_NUM));
	EXPECT_TRUE(TestDoDelItemReq(SUCCESS, RIGHT_AREA, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, ROLEID, GOLD_ID, VALID_ITEM_NUM));
	EXPECT_TRUE(TestDoDelItemReq(SUCCESS, RIGHT_AREA, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, ROLEID, JADE_ID, VALID_ITEM_NUM));
	EXPECT_TRUE(TestDoDelItemReq(SUCCESS, RIGHT_AREA, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, ROLEID, POWER_ID, VALID_ITEM_NUM));
}

TEST(TestDoDelItemReq, PLAT_NOT_EXIST)
{
	EXPECT_TRUE(TestDoDelItemReq(PLAT_NOT_EXIST, WRONG_AREA, RIGHT_PARTITION, WRONG_PLAT, RIGHT_OPENID, ROLEID, VALID_ITEM_ID, VALID_ITEM_NUM));
	EXPECT_TRUE(TestDoDelItemReq(PLAT_NOT_EXIST, WRONG_AREA, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, ROLEID, VALID_ITEM_ID, VALID_ITEM_NUM));
	EXPECT_TRUE(TestDoDelItemReq(PLAT_NOT_EXIST, RIGHT_AREA, RIGHT_PARTITION, WRONG_PLAT, RIGHT_OPENID, ROLEID, VALID_ITEM_ID, VALID_ITEM_NUM));
}

TEST(TestDoDelItemReq, PARTITION_NOT_EXIST)
{
	EXPECT_TRUE(TestDoDelItemReq(PARTITION_NOT_EXIST, RIGHT_AREA, WRONG_PARTITION, RIGHT_PLAT, RIGHT_OPENID, ROLEID, VALID_ITEM_ID, VALID_ITEM_NUM));
}

TEST(TestDoDelItemReq, OPENID_NOT_EXIST)
{
	EXPECT_TRUE(TestDoDelItemReq(OPENID_NOT_EXIST, RIGHT_AREA, RIGHT_PARTITION, RIGHT_PLAT, WRONG_OPENID, ROLEID, VALID_ITEM_ID, VALID_ITEM_NUM));
}

TEST(TestDoDelItemReq, ITEM_NUM_IS_INVALID)
{
	EXPECT_TRUE(TestDoDelItemReq(ITEM_NUM_IS_INVALID, RIGHT_AREA, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, ROLEID, VALID_ITEM_ID, 0));
	EXPECT_TRUE(TestDoDelItemReq(ITEM_NUM_IS_INVALID, RIGHT_AREA, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, ROLEID, VALID_ITEM_ID, -10));
}

TEST(TestDoDelItemReq, GIDIP_ITEM_ID_ERROR)
{
	EXPECT_TRUE(TestDoDelItemReq(GIDIP_ITEM_ID_ERROR, RIGHT_AREA, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, ROLEID, INVALID_ITEM_ID, VALID_ITEM_NUM));
	EXPECT_TRUE(TestDoDelItemReq(GIDIP_ITEM_ID_ERROR, RIGHT_AREA, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, ROLEID, -10, VALID_ITEM_NUM));
}

//增加白名单
bool TestDoActiveUserReq(ErrCode eErrCode, uint32 nAreaId, uint32 nPartition, uint32 nPlatId, char * szOpenId, int32 nType)
{
	string strFormat = "\"body\" :{";

	strFormat += "\"AreaId\" : %d,";
	strFormat += "\"Partition\" : %d,";
	strFormat += "\"PlatId\" : %d,";
	strFormat += "\"OpenId\" : \"%s\",";
	strFormat += "\"Type\" : %d,";

	strFormat += "\"Source\" : 0, ";
	strFormat += "\"Serial\" : \"\"}";

	char szBody[2048];
	sprintf(szBody, strFormat.c_str(), nAreaId, nPartition, nPlatId, szOpenId, nType);

	string strRetMsg = CHttpProto::Instance()->GetErrMsg(eErrCode);

	//RspResult oRet = CHttpProto::Instance()->DealReq(GetHeadStr(4097) + string(szBody));
	//bool bRet = RspResult{ eErrCode,strRetMsg } == oRet;
	//return bRet;
	return RspResult{ eErrCode,strRetMsg } == CHttpProto::Instance()->DealReq(GetHeadStr(4107) + string(szBody));
}

//area 1-WX  2-QQ 3-Account   type  0-outer  1-inner
TEST(TestDoActiveUserReq, SUCCESS)
{
	EXPECT_TRUE(TestDoActiveUserReq(SUCCESS, 1, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, 0));
	EXPECT_TRUE(TestDoActiveUserReq(SUCCESS, 2, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, 0));
	EXPECT_TRUE(TestDoActiveUserReq(SUCCESS, 3, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, 0));
	EXPECT_TRUE(TestDoActiveUserReq(SUCCESS, 1, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, 1));
	EXPECT_TRUE(TestDoActiveUserReq(SUCCESS, 2, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, 1));
	EXPECT_TRUE(TestDoActiveUserReq(SUCCESS, 3, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, 1));

	EXPECT_TRUE(TestDoActiveUserReq(SUCCESS, 1, RIGHT_PARTITION, RIGHT_PLAT, WRONG_OPENID, 0));
	EXPECT_TRUE(TestDoActiveUserReq(SUCCESS, 2, RIGHT_PARTITION, RIGHT_PLAT, WRONG_OPENID, 0));
	EXPECT_TRUE(TestDoActiveUserReq(SUCCESS, 3, RIGHT_PARTITION, RIGHT_PLAT, WRONG_OPENID, 0));
	EXPECT_TRUE(TestDoActiveUserReq(SUCCESS, 1, RIGHT_PARTITION, RIGHT_PLAT, WRONG_OPENID, 1));
	EXPECT_TRUE(TestDoActiveUserReq(SUCCESS, 2, RIGHT_PARTITION, RIGHT_PLAT, WRONG_OPENID, 1));
	EXPECT_TRUE(TestDoActiveUserReq(SUCCESS, 3, RIGHT_PARTITION, RIGHT_PLAT, WRONG_OPENID, 1));
}

TEST(TestDoActiveUserReq, ACTIVE_USER_AREA_ERROR)
{
	EXPECT_TRUE(TestDoActiveUserReq(ACTIVE_USER_AREA_ERROR, -1, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, 0));
	EXPECT_TRUE(TestDoActiveUserReq(ACTIVE_USER_AREA_ERROR, 0, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, 0));
	EXPECT_TRUE(TestDoActiveUserReq(ACTIVE_USER_AREA_ERROR, 4, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, 0));
	EXPECT_TRUE(TestDoActiveUserReq(ACTIVE_USER_AREA_ERROR, -1, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, 1));
	EXPECT_TRUE(TestDoActiveUserReq(ACTIVE_USER_AREA_ERROR, 0, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, 1));
	EXPECT_TRUE(TestDoActiveUserReq(ACTIVE_USER_AREA_ERROR, 4, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, 1));
}

TEST(TestDoActiveUserReq, ACTIVE_USER_TYPE_ERROR)
{
	EXPECT_TRUE(TestDoActiveUserReq(ACTIVE_USER_TYPE_ERROR, 1, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, -1));
	EXPECT_TRUE(TestDoActiveUserReq(ACTIVE_USER_TYPE_ERROR, 2, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, -1));
	EXPECT_TRUE(TestDoActiveUserReq(ACTIVE_USER_TYPE_ERROR, 3, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, -1));
	EXPECT_TRUE(TestDoActiveUserReq(ACTIVE_USER_TYPE_ERROR, 1, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, 2));
	EXPECT_TRUE(TestDoActiveUserReq(ACTIVE_USER_TYPE_ERROR, 2, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, 2));
	EXPECT_TRUE(TestDoActiveUserReq(ACTIVE_USER_TYPE_ERROR, 3, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, 2));
}

//删除白名单
bool TestDoDelWhiteListReq(ErrCode eErrCode, uint32 nAreaId, uint32 nPartition, uint32 nPlatId, char * szOpenId, int32 nType)
{
	string strFormat = "\"body\" :{";

	strFormat += "\"AreaId\" : %d,";
	strFormat += "\"Partition\" : %d,";
	strFormat += "\"PlatId\" : %d,";
	strFormat += "\"OpenId\" : \"%s\",";
	strFormat += "\"Type\" : %d,";

	strFormat += "\"Source\" : 0, ";
	strFormat += "\"Serial\" : \"\"}";

	char szBody[2048];
	sprintf(szBody, strFormat.c_str(), nAreaId, nPartition, nPlatId, szOpenId, nType);

	string strRetMsg = CHttpProto::Instance()->GetErrMsg(eErrCode);

	//RspResult oRet = CHttpProto::Instance()->DealReq(GetHeadStr(4097) + string(szBody));
	//bool bRet = RspResult{ eErrCode,strRetMsg } == oRet;
	//return bRet;
	return RspResult{ eErrCode,strRetMsg } == CHttpProto::Instance()->DealReq(GetHeadStr(4129) + string(szBody));
}

//area 1-WX  2-QQ 3-Account   type  0-outer  1-inner
TEST(TestDoDelWhiteListReq, SUCCESS)
{
	EXPECT_TRUE(TestDoDelWhiteListReq(SUCCESS, 1, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, 0));
	EXPECT_TRUE(TestDoDelWhiteListReq(SUCCESS, 2, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, 0));
	EXPECT_TRUE(TestDoDelWhiteListReq(SUCCESS, 3, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, 0));
	EXPECT_TRUE(TestDoDelWhiteListReq(SUCCESS, 1, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, 1));
	EXPECT_TRUE(TestDoDelWhiteListReq(SUCCESS, 2, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, 1));
	EXPECT_TRUE(TestDoDelWhiteListReq(SUCCESS, 3, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, 1));
}

TEST(TestDoDelWhiteListReq, DEL_WHITE_LIST_AREA_ERROR)
{
	EXPECT_TRUE(TestDoDelWhiteListReq(DEL_WHITE_LIST_AREA_ERROR, -1, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, 0));
	EXPECT_TRUE(TestDoDelWhiteListReq(DEL_WHITE_LIST_AREA_ERROR, 0, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, 0));
	EXPECT_TRUE(TestDoDelWhiteListReq(DEL_WHITE_LIST_AREA_ERROR, 4, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, 0));
	EXPECT_TRUE(TestDoDelWhiteListReq(DEL_WHITE_LIST_AREA_ERROR, -1, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, 1));
	EXPECT_TRUE(TestDoDelWhiteListReq(DEL_WHITE_LIST_AREA_ERROR, 0, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, 1));
	EXPECT_TRUE(TestDoDelWhiteListReq(DEL_WHITE_LIST_AREA_ERROR, 4, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, 1));
}

TEST(TestDoDelWhiteListReq, DEL_WHITE_LIST_TYPE_ERROR)
{
	EXPECT_TRUE(TestDoDelWhiteListReq(DEL_WHITE_LIST_TYPE_ERROR, 1, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, -1));
	EXPECT_TRUE(TestDoDelWhiteListReq(DEL_WHITE_LIST_TYPE_ERROR, 2, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, -1));
	EXPECT_TRUE(TestDoDelWhiteListReq(DEL_WHITE_LIST_TYPE_ERROR, 3, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, -1));
	EXPECT_TRUE(TestDoDelWhiteListReq(DEL_WHITE_LIST_TYPE_ERROR, 1, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, 2));
	EXPECT_TRUE(TestDoDelWhiteListReq(DEL_WHITE_LIST_TYPE_ERROR, 2, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, 2));
	EXPECT_TRUE(TestDoDelWhiteListReq(DEL_WHITE_LIST_TYPE_ERROR, 3, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, 2));
}

//保证 RIGHT_OPENID 在白名单
TEST(TestAddActiveUserReq, ADD_WHITE_LIST)
{
	EXPECT_TRUE(TestDoActiveUserReq(SUCCESS, 3, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, 0));
	EXPECT_TRUE(TestDoActiveUserReq(SUCCESS, 3, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, 1));
}

//小区查询
bool TestQueryPartitionReq(ErrCode eErrCode, uint32 nAreaId, uint32 nPlatId)
{
	string strFormat = "\"body\" :{";

	strFormat += "\"AreaId\" : %d,";
	strFormat += "\"PlatId\" : %d,";

	strFormat += "\"Source\" : 0, ";
	strFormat += "\"Serial\" : \"\"}";

	char szBody[2048];
	sprintf(szBody, strFormat.c_str(), nAreaId, nPlatId );

	string strRetMsg = CHttpProto::Instance()->GetErrMsg(eErrCode);

	//RspResult oRet = CHttpProto::Instance()->DealReq(GetHeadStr(4097) + string(szBody));
	//bool bRet = RspResult{ eErrCode,strRetMsg } == oRet;
	//return bRet;
	return RspResult{ eErrCode,strRetMsg } == CHttpProto::Instance()->DealReq(GetHeadStr(4121) + string(szBody));
}

TEST(TestQueryPartitionReq, SUCCESS)
{
	EXPECT_TRUE(TestQueryPartitionReq(SUCCESS, RIGHT_AREA,RIGHT_PLAT));
}

TEST(TestQueryPartitionReq, PLAT_NOT_EXIST)
{
	EXPECT_TRUE(TestQueryPartitionReq(PLAT_NOT_EXIST, WRONG_AREA, WRONG_PLAT));
	EXPECT_TRUE(TestQueryPartitionReq(PLAT_NOT_EXIST, WRONG_AREA, RIGHT_PLAT));
	EXPECT_TRUE(TestQueryPartitionReq(PLAT_NOT_EXIST, RIGHT_AREA, WRONG_PLAT));
}

//个人信息查询
bool TestQueryInfoCurPersonReq(ErrCode eErrCode, uint32 nAreaId, uint32 nPartition, uint32 nPlatId, char * szOpenId)
{
	string strFormat = "\"body\" :{";

	strFormat += "\"AreaId\" : %d,";
	strFormat += "\"Partition\" : %d,";
	strFormat += "\"PlatId\" : %d,";
	strFormat += "\"OpenId\" : \"%s\",";

	strFormat += "\"Source\" : 0, ";
	strFormat += "\"Serial\" : \"\"}";

	char szBody[2048];
	sprintf(szBody, strFormat.c_str(), nAreaId, nPartition, nPlatId, szOpenId);

	string strRetMsg = CHttpProto::Instance()->GetErrMsg(eErrCode);

	//RspResult oRet = CHttpProto::Instance()->DealReq(GetHeadStr(4097) + string(szBody));
	//bool bRet = RspResult{ eErrCode,strRetMsg } == oRet;
	//return bRet;
	return RspResult{ eErrCode,strRetMsg } == CHttpProto::Instance()->DealReq(GetHeadStr(4113) + string(szBody));
}


TEST(TestQueryInfoCurPersonReq, SUCCESS)
{
	EXPECT_TRUE(TestQueryInfoCurPersonReq(SUCCESS, RIGHT_AREA, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID));
}

TEST(TestQueryInfoCurPersonReq, PLAT_NOT_EXIST)
{
	EXPECT_TRUE(TestQueryInfoCurPersonReq(PLAT_NOT_EXIST, WRONG_AREA, RIGHT_PARTITION, WRONG_PLAT, RIGHT_OPENID));
	EXPECT_TRUE(TestQueryInfoCurPersonReq(PLAT_NOT_EXIST, WRONG_AREA, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID));
	EXPECT_TRUE(TestQueryInfoCurPersonReq(PLAT_NOT_EXIST, RIGHT_AREA, RIGHT_PARTITION, WRONG_PLAT, RIGHT_OPENID));
}

TEST(TestQueryInfoCurPersonReq, PARTITION_NOT_EXIST)
{
	EXPECT_TRUE(TestQueryInfoCurPersonReq(PARTITION_NOT_EXIST, RIGHT_AREA, WRONG_PARTITION, RIGHT_PLAT, RIGHT_OPENID));
}

TEST(TestQueryInfoCurPersonReq, OPENID_NOT_EXIST)
{
	EXPECT_TRUE(TestQueryInfoCurPersonReq(OPENID_NOT_EXIST, RIGHT_AREA, RIGHT_PARTITION, RIGHT_PLAT, WRONG_OPENID));
}
//门客查询
bool TestQueryInfoCurMenkeReq(ErrCode eErrCode, uint32 nAreaId, uint32 nPartition, uint32 nPlatId, char * szOpenId, uint64 nRoleId)
{
	string strFormat = "\"body\" :{";

	strFormat += "\"AreaId\" : %d,";
	strFormat += "\"Partition\" : %d,";
	strFormat += "\"PlatId\" : %d,";
	strFormat += "\"OpenId\" : \"%s\",";
	strFormat += "\"RoleId\" : %lld,";

	strFormat += "\"Source\" : 0, ";
	strFormat += "\"Serial\" : \"\"}";

	char szBody[2048];
	sprintf(szBody, strFormat.c_str(), nAreaId, nPartition, nPlatId, szOpenId, nRoleId);

	string strRetMsg = CHttpProto::Instance()->GetErrMsg(eErrCode);

	//RspResult oRet = CHttpProto::Instance()->DealReq(GetHeadStr(4097) + string(szBody));
	//bool bRet = RspResult{ eErrCode,strRetMsg } == oRet;
	//return bRet;
	return RspResult{ eErrCode,strRetMsg } == CHttpProto::Instance()->DealReq(GetHeadStr(4117) + string(szBody));
}


TEST(TestQueryInfoCurMenkeReq, SUCCESS)
{
	EXPECT_TRUE(TestQueryInfoCurMenkeReq(SUCCESS, RIGHT_AREA, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, ROLEID));
}

TEST(TestQueryInfoCurMenkeReq, PLAT_NOT_EXIST)
{
	EXPECT_TRUE(TestQueryInfoCurMenkeReq(PLAT_NOT_EXIST, WRONG_AREA, RIGHT_PARTITION, WRONG_PLAT, RIGHT_OPENID, ROLEID));
	EXPECT_TRUE(TestQueryInfoCurMenkeReq(PLAT_NOT_EXIST, WRONG_AREA, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, ROLEID));
	EXPECT_TRUE(TestQueryInfoCurMenkeReq(PLAT_NOT_EXIST, RIGHT_AREA, RIGHT_PARTITION, WRONG_PLAT, RIGHT_OPENID, ROLEID));
}

TEST(TestQueryInfoCurMenkeReq, PARTITION_NOT_EXIST)
{
	EXPECT_TRUE(TestQueryInfoCurMenkeReq(PARTITION_NOT_EXIST, RIGHT_AREA, WRONG_PARTITION, RIGHT_PLAT, RIGHT_OPENID, ROLEID));
}

TEST(TestQueryInfoCurMenkeReq, OPENID_NOT_EXIST)
{
	EXPECT_TRUE(TestQueryInfoCurMenkeReq(OPENID_NOT_EXIST, RIGHT_AREA, RIGHT_PARTITION, RIGHT_PLAT, WRONG_OPENID, ROLEID));
}
//邮件查询
bool TestQueryMailReq(ErrCode eErrCode, uint32 nAreaId, uint32 nPartition, uint32 nPlatId, 
	char * szOpenId, uint64 nRoleId, uint64 nBeginTm, uint64 nEndTm)
{
	string strFormat = "\"body\" :{";

	strFormat += "\"AreaId\" : %d,";
	strFormat += "\"Partition\" : %d,";
	strFormat += "\"PlatId\" : %d,";
	strFormat += "\"OpenId\" : \"%s\",";
	strFormat += "\"RoleId\" : %lld,";
	strFormat += "\"BeginTime\" : %lld,";
	strFormat += "\"EndTime\" : %lld,";

	strFormat += "\"Source\" : 0, ";
	strFormat += "\"Serial\" : \"\"}";

	char szBody[2048];
	sprintf(szBody, strFormat.c_str(), nAreaId, nPartition, nPlatId, szOpenId, nRoleId, nBeginTm, nEndTm);

	string strRetMsg = CHttpProto::Instance()->GetErrMsg(eErrCode);

	//RspResult oRet = CHttpProto::Instance()->DealReq(GetHeadStr(4097) + string(szBody));
	//bool bRet = RspResult{ eErrCode,strRetMsg } == oRet;
	//return bRet;
	return RspResult{ eErrCode,strRetMsg } == CHttpProto::Instance()->DealReq(GetHeadStr(4119) + string(szBody));
}

#define QUERY_MAIL_BEGIN_TM 0
#define QUERY_MAIL_END_TM 1580490061  //2020 2-1-1

TEST(TestQueryMailReq, SUCCESS)
{
	EXPECT_TRUE(TestQueryMailReq(SUCCESS, RIGHT_AREA, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, ROLEID, QUERY_MAIL_BEGIN_TM, QUERY_MAIL_END_TM));
}

TEST(TestQueryMailReq, PLAT_NOT_EXIST)
{
	EXPECT_TRUE(TestQueryMailReq(PLAT_NOT_EXIST, WRONG_AREA, RIGHT_PARTITION, WRONG_PLAT, RIGHT_OPENID, ROLEID, QUERY_MAIL_BEGIN_TM, QUERY_MAIL_END_TM));
	EXPECT_TRUE(TestQueryMailReq(PLAT_NOT_EXIST, WRONG_AREA, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, ROLEID, QUERY_MAIL_BEGIN_TM, QUERY_MAIL_END_TM));
	EXPECT_TRUE(TestQueryMailReq(PLAT_NOT_EXIST, RIGHT_AREA, RIGHT_PARTITION, WRONG_PLAT, RIGHT_OPENID, ROLEID, QUERY_MAIL_BEGIN_TM, QUERY_MAIL_END_TM));
}

TEST(TestQueryMailReq, PARTITION_NOT_EXIST)
{
	EXPECT_TRUE(TestQueryMailReq(PARTITION_NOT_EXIST, RIGHT_AREA, WRONG_PARTITION, RIGHT_PLAT, RIGHT_OPENID, ROLEID, QUERY_MAIL_BEGIN_TM, QUERY_MAIL_END_TM));
}

TEST(TestQueryMailReq, OPENID_NOT_EXIST)
{
	EXPECT_TRUE(TestQueryMailReq(OPENID_NOT_EXIST, RIGHT_AREA, RIGHT_PARTITION, RIGHT_PLAT, WRONG_OPENID, ROLEID, QUERY_MAIL_BEGIN_TM, QUERY_MAIL_END_TM));
}
//修改新手引导
#define VALID_GUIDE_ID 501

bool TestDoUpdateStageGuideReq(ErrCode eErrCode, uint32 nAreaId, uint32 nPartition, uint32 nPlatId, char * szOpenId, uint64 nRoleId, int32 nValue)
{
	string strFormat = "\"body\" :{";

	strFormat += "\"AreaId\" : %d,";
	strFormat += "\"Partition\" : %d,";
	strFormat += "\"PlatId\" : %d,";
	strFormat += "\"OpenId\" : \"%s\",";
	strFormat += "\"RoleId\" : %lld,";
	strFormat += "\"Value\" : %d,";

	strFormat += "\"Source\" : 0, ";
	strFormat += "\"Serial\" : \"\"}";

	char szBody[2048];
	sprintf(szBody, strFormat.c_str(), nAreaId, nPartition, nPlatId, szOpenId, nRoleId, nValue);

	string strRetMsg = CHttpProto::Instance()->GetErrMsg(eErrCode);

	//RspResult oRet = CHttpProto::Instance()->DealReq(GetHeadStr(4097) + string(szBody));
	//bool bRet = RspResult{ eErrCode,strRetMsg } == oRet;
	//return bRet;
	return RspResult{ eErrCode,strRetMsg } == CHttpProto::Instance()->DealReq(GetHeadStr(4125) + string(szBody));
}


TEST(TestDoUpdateStageGuideReq, SUCCESS)
{
	EXPECT_TRUE(TestDoUpdateStageGuideReq(SUCCESS, RIGHT_AREA, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, ROLEID, VALID_GUIDE_ID));
}

TEST(TestDoUpdateStageGuideReq, PLAT_NOT_EXIST)
{
	EXPECT_TRUE(TestDoUpdateStageGuideReq(PLAT_NOT_EXIST, WRONG_AREA, RIGHT_PARTITION, WRONG_PLAT, RIGHT_OPENID, ROLEID, POSITIVE_VAL));
	EXPECT_TRUE(TestDoUpdateStageGuideReq(PLAT_NOT_EXIST, WRONG_AREA, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, ROLEID, ZERO_VAL));
	EXPECT_TRUE(TestDoUpdateStageGuideReq(PLAT_NOT_EXIST, RIGHT_AREA, RIGHT_PARTITION, WRONG_PLAT, RIGHT_OPENID, ROLEID, NEGATIVE_VAL));
}

TEST(TestDoUpdateStageGuideReq, PARTITION_NOT_EXIST)
{
	EXPECT_TRUE(TestDoUpdateStageGuideReq(PARTITION_NOT_EXIST, RIGHT_AREA, WRONG_PARTITION, RIGHT_PLAT, RIGHT_OPENID, ROLEID, VALID_GUIDE_ID));
}

TEST(TestDoUpdateStageGuideReq, OPENID_NOT_EXIST)
{
	EXPECT_TRUE(TestDoUpdateStageGuideReq(OPENID_NOT_EXIST, RIGHT_AREA, RIGHT_PARTITION, RIGHT_PLAT, WRONG_OPENID, ROLEID, VALID_GUIDE_ID));
}

TEST(TestDoUpdateStageGuideReq, GUIDE_ID_ERROR)
{
	EXPECT_TRUE(TestDoUpdateStageGuideReq(GUIDE_ID_ERROR, RIGHT_AREA, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, ROLEID, 0));
	EXPECT_TRUE(TestDoUpdateStageGuideReq(GUIDE_ID_ERROR, RIGHT_AREA, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, ROLEID, -10));
	EXPECT_TRUE(TestDoUpdateStageGuideReq(GUIDE_ID_ERROR, RIGHT_AREA, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, ROLEID, 500));
}
//修改PVE进度
#define VALID_CHAPTER_TYPE_0 0
#define VALID_CHAPTER_TYPE_1 1
#define VALID_CHAPTER_ID 5
#define VALID_STAGE_ID 5
#define VALID_CHAPTER_STAR 1

bool TestDoUpdatePveProgressReq(ErrCode eErrCode, uint32 nAreaId, uint32 nPartition, uint32 nPlatId, char * szOpenId, uint64 nRoleId,
	int32 nChapterType, int32 nChapterId, int32 nStageId, int32 nChapterStar)
{
	string strFormat = "\"body\" :{";

	strFormat += "\"AreaId\" : %d,";
	strFormat += "\"Partition\" : %d,";
	strFormat += "\"PlatId\" : %d,";
	strFormat += "\"OpenId\" : \"%s\",";
	strFormat += "\"RoleId\" : %lld,";
	strFormat += "\"ChapterType\" : %d,";
	strFormat += "\"ChapterId\" : %d,";
	strFormat += "\"StageId\" : %d,";
	strFormat += "\"ChapterStar\" : %d,";

	strFormat += "\"Source\" : 0, ";
	strFormat += "\"Serial\" : \"\"}";

	char szBody[2048];
	sprintf(szBody, strFormat.c_str(), nAreaId, nPartition, nPlatId, szOpenId, nRoleId, nChapterType, nChapterId, nStageId, nChapterStar);

	string strRetMsg = CHttpProto::Instance()->GetErrMsg(eErrCode);

	//RspResult oRet = CHttpProto::Instance()->DealReq(GetHeadStr(4097) + string(szBody));
	//bool bRet = RspResult{ eErrCode,strRetMsg } == oRet;
	//return bRet;
	return RspResult{ eErrCode,strRetMsg } == CHttpProto::Instance()->DealReq(GetHeadStr(4127) + string(szBody));
}


TEST(TestDoUpdatePveProgressReq, SUCCESS)
{
	EXPECT_TRUE(TestDoUpdatePveProgressReq(SUCCESS, RIGHT_AREA, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, ROLEID,
		VALID_CHAPTER_TYPE_0, VALID_CHAPTER_ID, VALID_STAGE_ID, VALID_CHAPTER_STAR));
	EXPECT_TRUE(TestDoUpdatePveProgressReq(SUCCESS, RIGHT_AREA, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, ROLEID,
		VALID_CHAPTER_TYPE_1, VALID_CHAPTER_ID, VALID_STAGE_ID, VALID_CHAPTER_STAR));
}

TEST(TestDoUpdatePveProgressReq, PLAT_NOT_EXIST)
{
	EXPECT_TRUE(TestDoUpdatePveProgressReq(PLAT_NOT_EXIST, WRONG_AREA, RIGHT_PARTITION, WRONG_PLAT, RIGHT_OPENID, ROLEID,
		VALID_CHAPTER_TYPE_0, VALID_CHAPTER_ID, VALID_STAGE_ID, VALID_CHAPTER_STAR));
	EXPECT_TRUE(TestDoUpdatePveProgressReq(PLAT_NOT_EXIST, WRONG_AREA, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, ROLEID,
		VALID_CHAPTER_TYPE_0, VALID_CHAPTER_ID, VALID_STAGE_ID, VALID_CHAPTER_STAR));
	EXPECT_TRUE(TestDoUpdatePveProgressReq(PLAT_NOT_EXIST, RIGHT_AREA, RIGHT_PARTITION, WRONG_PLAT, RIGHT_OPENID, ROLEID,
		VALID_CHAPTER_TYPE_0, VALID_CHAPTER_ID, VALID_STAGE_ID, VALID_CHAPTER_STAR));
	EXPECT_TRUE(TestDoUpdatePveProgressReq(PLAT_NOT_EXIST, WRONG_AREA, RIGHT_PARTITION, WRONG_PLAT, RIGHT_OPENID, ROLEID,
		VALID_CHAPTER_TYPE_1, VALID_CHAPTER_ID, VALID_STAGE_ID, VALID_CHAPTER_STAR));
	EXPECT_TRUE(TestDoUpdatePveProgressReq(PLAT_NOT_EXIST, WRONG_AREA, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, ROLEID,
		VALID_CHAPTER_TYPE_1, VALID_CHAPTER_ID, VALID_STAGE_ID, VALID_CHAPTER_STAR));
	EXPECT_TRUE(TestDoUpdatePveProgressReq(PLAT_NOT_EXIST, RIGHT_AREA, RIGHT_PARTITION, WRONG_PLAT, RIGHT_OPENID, ROLEID,
		VALID_CHAPTER_TYPE_1, VALID_CHAPTER_ID, VALID_STAGE_ID, VALID_CHAPTER_STAR));
}

TEST(TestDoUpdatePveProgressReq, PARTITION_NOT_EXIST)
{
	EXPECT_TRUE(TestDoUpdatePveProgressReq(PARTITION_NOT_EXIST, RIGHT_AREA, WRONG_PARTITION, RIGHT_PLAT, RIGHT_OPENID, ROLEID,
		VALID_CHAPTER_TYPE_0, VALID_CHAPTER_ID, VALID_STAGE_ID, VALID_CHAPTER_STAR));
	EXPECT_TRUE(TestDoUpdatePveProgressReq(PARTITION_NOT_EXIST, RIGHT_AREA, WRONG_PARTITION, RIGHT_PLAT, RIGHT_OPENID, ROLEID,
		VALID_CHAPTER_TYPE_1, VALID_CHAPTER_ID, VALID_STAGE_ID, VALID_CHAPTER_STAR));
}

TEST(TestDoUpdatePveProgressReq, OPENID_NOT_EXIST)
{
	EXPECT_TRUE(TestDoUpdatePveProgressReq(OPENID_NOT_EXIST, RIGHT_AREA, RIGHT_PARTITION, RIGHT_PLAT, WRONG_OPENID, ROLEID,
		VALID_CHAPTER_TYPE_0, VALID_CHAPTER_ID, VALID_STAGE_ID, VALID_CHAPTER_STAR));
	EXPECT_TRUE(TestDoUpdatePveProgressReq(OPENID_NOT_EXIST, RIGHT_AREA, RIGHT_PARTITION, RIGHT_PLAT, WRONG_OPENID, ROLEID,
		VALID_CHAPTER_TYPE_1, VALID_CHAPTER_ID, VALID_STAGE_ID, VALID_CHAPTER_STAR));
}

TEST(TestDoUpdatePveProgressReq, PVE_CHPATER_TYPE_ERROR)
{
	EXPECT_TRUE(TestDoUpdatePveProgressReq(PVE_CHPATER_TYPE_ERROR, RIGHT_AREA, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, ROLEID,
		-1, VALID_CHAPTER_ID, VALID_STAGE_ID, VALID_CHAPTER_STAR));
	EXPECT_TRUE(TestDoUpdatePveProgressReq(PVE_CHPATER_TYPE_ERROR, RIGHT_AREA, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, ROLEID,
		2, VALID_CHAPTER_ID, VALID_STAGE_ID, VALID_CHAPTER_STAR));
}

TEST(TestDoUpdatePveProgressReq, PVE_CHAPTER_ID_ERROR)
{
	EXPECT_TRUE(TestDoUpdatePveProgressReq(PVE_CHAPTER_ID_ERROR, RIGHT_AREA, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, ROLEID,
		VALID_CHAPTER_TYPE_0, 0, VALID_STAGE_ID, VALID_CHAPTER_STAR));
	EXPECT_TRUE(TestDoUpdatePveProgressReq(PVE_CHAPTER_ID_ERROR, RIGHT_AREA, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, ROLEID,
		VALID_CHAPTER_TYPE_1, 0, VALID_STAGE_ID, VALID_CHAPTER_STAR));
	EXPECT_TRUE(TestDoUpdatePveProgressReq(PVE_CHAPTER_ID_ERROR, RIGHT_AREA, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, ROLEID,
		VALID_CHAPTER_TYPE_0, 20, VALID_STAGE_ID, VALID_CHAPTER_STAR));
	EXPECT_TRUE(TestDoUpdatePveProgressReq(PVE_CHAPTER_ID_ERROR, RIGHT_AREA, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, ROLEID,
		VALID_CHAPTER_TYPE_1, 20, VALID_STAGE_ID, VALID_CHAPTER_STAR));
}

TEST(TestDoUpdatePveProgressReq, PVE_STAGE_ID_ERROR)
{
	EXPECT_TRUE(TestDoUpdatePveProgressReq(PVE_STAGE_ID_ERROR, RIGHT_AREA, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, ROLEID,
		VALID_CHAPTER_TYPE_0, VALID_CHAPTER_ID, 0, VALID_CHAPTER_STAR));
	EXPECT_TRUE(TestDoUpdatePveProgressReq(PVE_STAGE_ID_ERROR, RIGHT_AREA, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, ROLEID,
		VALID_CHAPTER_TYPE_1, VALID_CHAPTER_ID, 0, VALID_CHAPTER_STAR));
	EXPECT_TRUE(TestDoUpdatePveProgressReq(PVE_STAGE_ID_ERROR, RIGHT_AREA, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, ROLEID,
		VALID_CHAPTER_TYPE_0, VALID_CHAPTER_ID, 20, VALID_CHAPTER_STAR));
	EXPECT_TRUE(TestDoUpdatePveProgressReq(PVE_STAGE_ID_ERROR, RIGHT_AREA, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, ROLEID,
		VALID_CHAPTER_TYPE_1, VALID_CHAPTER_ID, 20, VALID_CHAPTER_STAR));
}

TEST(TestDoUpdatePveProgressReq, PVE_CHAPTER_STAR_ERROR)
{
	EXPECT_TRUE(TestDoUpdatePveProgressReq(PVE_CHAPTER_STAR_ERROR, RIGHT_AREA, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, ROLEID,
		VALID_CHAPTER_TYPE_0, VALID_CHAPTER_ID, VALID_STAGE_ID, 0));
	EXPECT_TRUE(TestDoUpdatePveProgressReq(PVE_CHAPTER_STAR_ERROR, RIGHT_AREA, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, ROLEID,
		VALID_CHAPTER_TYPE_1, VALID_CHAPTER_ID, VALID_STAGE_ID, 0));
	EXPECT_TRUE(TestDoUpdatePveProgressReq(PVE_CHAPTER_STAR_ERROR, RIGHT_AREA, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, ROLEID,
		VALID_CHAPTER_TYPE_0, VALID_CHAPTER_ID, VALID_STAGE_ID, 5));
	EXPECT_TRUE(TestDoUpdatePveProgressReq(PVE_CHAPTER_STAR_ERROR, RIGHT_AREA, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, ROLEID,
		VALID_CHAPTER_TYPE_1, VALID_CHAPTER_ID, VALID_STAGE_ID, 5));
}
//封号
bool TestDoBanUsrReq(ErrCode eErrCode, uint32 nAreaId, uint32 nPartition, uint32 nPlatId, char * szOpenId,
	int64 nBeginTm, int64 nEndTm, char * szReason)
{
	string strFormat = "\"body\" :{";

	strFormat += "\"AreaId\" : %d,";
	strFormat += "\"Partition\" : %d,";
	strFormat += "\"PlatId\" : %d,";
	strFormat += "\"OpenId\" : \"%s\",";
	strFormat += "\"BanBeginTime\" : %lld,";
	strFormat += "\"BanEndTime\" : %lld,";
	strFormat += "\"BanReason\" : \"%s\",";

	strFormat += "\"Source\" : 0, ";
	strFormat += "\"Serial\" : \"\"}";

	char szBody[2048];
	sprintf(szBody, strFormat.c_str(), nAreaId, nPartition, nPlatId, szOpenId, nBeginTm, nEndTm, szReason);

	string strRetMsg = CHttpProto::Instance()->GetErrMsg(eErrCode);

	//RspResult oRet = CHttpProto::Instance()->DealReq(GetHeadStr(4097) + string(szBody));
	//bool bRet = RspResult{ eErrCode,strRetMsg } == oRet;
	//return bRet;
	return RspResult{ eErrCode,strRetMsg } == CHttpProto::Instance()->DealReq(GetHeadStr(4109) + string(szBody));
}

#define BAN_BEGIN_TIME GetNowTime()-100
#define INVALID_BAN_BEGIN_TIME GetNowTime()+10
#define BAN_END_TIME 1580490061  //2020 2-1-1
#define	INVALID_BAN_END_TIME GetNowTime() - 10
#define BAN_REASON_1 "%E7%9C%8B%E4%BD%A0%E4%B8%8D%E7%88%BD%E3%80%82"  //看你不爽。
#define BAN_REASON_2 "I enjoy it!"

TEST(TestDoBanUsrReq, SUCCESS)
{
	EXPECT_TRUE(TestDoBanUsrReq(SUCCESS, RIGHT_AREA, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, BAN_BEGIN_TIME, BAN_END_TIME, BAN_REASON_2));
	EXPECT_TRUE(TestDoBanUsrReq(SUCCESS, RIGHT_AREA, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, BAN_BEGIN_TIME, BAN_END_TIME, BAN_REASON_1));
}

TEST(TestDoBanUsrReq, PLAT_NOT_EXIST)
{
	EXPECT_TRUE(TestDoBanUsrReq(PLAT_NOT_EXIST, WRONG_AREA, RIGHT_PARTITION, WRONG_PLAT, RIGHT_OPENID, BAN_BEGIN_TIME, BAN_END_TIME, BAN_REASON_1));
	EXPECT_TRUE(TestDoBanUsrReq(PLAT_NOT_EXIST, WRONG_AREA, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, BAN_BEGIN_TIME, BAN_END_TIME, BAN_REASON_1));
	EXPECT_TRUE(TestDoBanUsrReq(PLAT_NOT_EXIST, RIGHT_AREA, RIGHT_PARTITION, WRONG_PLAT, RIGHT_OPENID, BAN_BEGIN_TIME, BAN_END_TIME, BAN_REASON_1));
	EXPECT_TRUE(TestDoBanUsrReq(PLAT_NOT_EXIST, WRONG_AREA, WRONG_PARTITION, WRONG_PLAT, RIGHT_OPENID, BAN_BEGIN_TIME, BAN_END_TIME, BAN_REASON_1));
	EXPECT_TRUE(TestDoBanUsrReq(PLAT_NOT_EXIST, WRONG_AREA, WRONG_PARTITION, RIGHT_PLAT, RIGHT_OPENID, BAN_BEGIN_TIME, BAN_END_TIME, BAN_REASON_1));
	EXPECT_TRUE(TestDoBanUsrReq(PLAT_NOT_EXIST, RIGHT_AREA, WRONG_PARTITION, WRONG_PLAT, RIGHT_OPENID, BAN_BEGIN_TIME, BAN_END_TIME, BAN_REASON_1));
}

TEST(TestDoBanUsrReq, OPENID_NOT_EXIST)
{
	EXPECT_TRUE(TestDoBanUsrReq(OPENID_NOT_EXIST, RIGHT_AREA, RIGHT_PARTITION, RIGHT_PLAT, WRONG_OPENID, BAN_BEGIN_TIME, BAN_END_TIME, BAN_REASON_1));
	EXPECT_TRUE(TestDoBanUsrReq(OPENID_NOT_EXIST, RIGHT_AREA, RIGHT_PARTITION, RIGHT_PLAT, WRONG_OPENID, BAN_BEGIN_TIME, BAN_END_TIME, BAN_REASON_2));
}

TEST(TestDoBanUsrReq, INVALID_TIME)
{
	EXPECT_TRUE(TestDoBanUsrReq(BAN_TIME_IS_INVALID, RIGHT_AREA, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, INVALID_BAN_BEGIN_TIME, BAN_END_TIME, BAN_REASON_2));
	EXPECT_TRUE(TestDoBanUsrReq(BAN_TIME_IS_INVALID, RIGHT_AREA, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, BAN_BEGIN_TIME, INVALID_BAN_END_TIME, BAN_REASON_2));
	EXPECT_TRUE(TestDoBanUsrReq(BAN_TIME_IS_INVALID, RIGHT_AREA, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, INVALID_BAN_BEGIN_TIME, BAN_END_TIME, BAN_REASON_1));
	EXPECT_TRUE(TestDoBanUsrReq(BAN_TIME_IS_INVALID, RIGHT_AREA, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, BAN_BEGIN_TIME, INVALID_BAN_END_TIME, BAN_REASON_1));
}
//解封
bool TestDoUnBanUsrReq(ErrCode eErrCode, uint32 nAreaId, uint32 nPartition, uint32 nPlatId, char * szOpenId, char * szReason)
{
	string strFormat = "\"body\" :{";

	strFormat += "\"AreaId\" : %d,";
	strFormat += "\"Partition\" : %d,";
	strFormat += "\"PlatId\" : %d,";
	strFormat += "\"OpenId\" : \"%s\",";
	strFormat += "\"BanReason\" : \"%s\",";

	strFormat += "\"Source\" : 0, ";
	strFormat += "\"Serial\" : \"\"}";

	char szBody[2048];
	sprintf(szBody, strFormat.c_str(), nAreaId, nPartition, nPlatId, szOpenId, szReason);

	string strRetMsg = CHttpProto::Instance()->GetErrMsg(eErrCode);

	//RspResult oRet = CHttpProto::Instance()->DealReq(GetHeadStr(4097) + string(szBody));
	//bool bRet = RspResult{ eErrCode,strRetMsg } == oRet;
	//return bRet;
	return RspResult{ eErrCode,strRetMsg } == CHttpProto::Instance()->DealReq(GetHeadStr(4111) + string(szBody));
}

TEST(TestDoUnBanUsrReq, SUCCESS)
{
	EXPECT_TRUE(TestDoUnBanUsrReq(SUCCESS, RIGHT_AREA, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, BAN_REASON_1));
}

TEST(TestDoUnBanUsrReq, PLAT_NOT_EXIST)
{
	EXPECT_TRUE(TestDoUnBanUsrReq(PLAT_NOT_EXIST, WRONG_AREA, RIGHT_PARTITION, WRONG_PLAT, RIGHT_OPENID, BAN_REASON_1));
	EXPECT_TRUE(TestDoUnBanUsrReq(PLAT_NOT_EXIST, WRONG_AREA, RIGHT_PARTITION, RIGHT_PLAT, RIGHT_OPENID, BAN_REASON_1));
	EXPECT_TRUE(TestDoUnBanUsrReq(PLAT_NOT_EXIST, RIGHT_AREA, RIGHT_PARTITION, WRONG_PLAT, RIGHT_OPENID, BAN_REASON_1));
	EXPECT_TRUE(TestDoUnBanUsrReq(PLAT_NOT_EXIST, WRONG_AREA, WRONG_PARTITION, WRONG_PLAT, RIGHT_OPENID, BAN_REASON_1));
	EXPECT_TRUE(TestDoUnBanUsrReq(PLAT_NOT_EXIST, WRONG_AREA, WRONG_PARTITION, RIGHT_PLAT, RIGHT_OPENID, BAN_REASON_1));
	EXPECT_TRUE(TestDoUnBanUsrReq(PLAT_NOT_EXIST, RIGHT_AREA, WRONG_PARTITION, WRONG_PLAT, RIGHT_OPENID, BAN_REASON_1));
}

TEST(TestDoUnBanUsrReq, OPENID_NOT_EXIST)
{
	EXPECT_TRUE(TestDoUnBanUsrReq(OPENID_NOT_EXIST, RIGHT_AREA, RIGHT_PARTITION, RIGHT_PLAT, WRONG_OPENID, BAN_REASON_1));
}