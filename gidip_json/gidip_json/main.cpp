#include "read_json.h"
#include "gtest/gtest.h"

//int main()
//{
//	CReadJson oReadJson;
//	oReadJson.Start(3008);
//	return 0;
//}

int getAbs(int i)
{
	// 添加了以部分逻辑处理导致进入了以下逻辑
	/* add some code here */

	return abs(i);
}

enum ErrCode
{
	CHECK_SIGN_FAILED = -10,
	SERVER_BUSY = -11,
	PARSE_BODY_ERROR = -12,
	REQUEST_IS_UNKNOWN = -13,
	DEAL_TIME_OUT = -14,
	PARTITION_NOT_EXIST = -15,
	ROLE_NOT_EXIST = 1,
	SUCCESS = 0,
	PLAT_NOT_EXIST = 11,
	SEND_MAIL_FAILED = 12,
	UPDATE_SQL_FAILED = 13,
	INSERT_SQL_FAILED = 14,
	OPENID_NOT_EXIST = 1,
	ACTIVE_USER_TYPE_ERROR = 16,
	DEL_WHITE_LIST_TYPE_ERROR = 17,
	ACTIVE_USER_AREA_ERROR = 18,
	DEL_WHITE_LIST_AREA_ERROR = 19,
	ITEM_NUM_IS_INVALID = 20,
	COUNT_NOT_MATCH = 21,
	PVE_CHPATER_TYPE_ERROR = 22,
	PVE_CHAPTER_ID_ERROR = 23,
	PVE_STAGE_ID_ERROR = 24,
	PVE_CHAPTER_STAR_ERROR = 25,
	GUIDE_ID_ERROR = 26,
	ACTIVE_USER_OPEN_ID_ERROR = 27,
	GIDIP_ITEM_ID_ERROR = 28,
	GIDIP_MAIL_TIME_ERROR = 29
};

struct RspResult
{
	ErrCode eErrCode;
	string strRetMsg;

	bool operator==(RspResult & oRspRet)
	{
		return eErrCode == oRspRet.eErrCode && strRetMsg == oRspRet.strRetMsg;
	}
};
TEST(abswapper, case1)
{
	EXPECT_TRUE(RspResult{}==RspResult{});
	EXPECT_EQ(1, getAbs(1));
	EXPECT_EQ(1, getAbs(-1));
}

int main(int argc, char* argv[])
{
	testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();

	return 0;
}