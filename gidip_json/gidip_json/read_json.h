#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include "json.h"

using namespace std;

typedef void(Output)();

enum LINE_TYPE
{
	LINE_INVALID,
	LINE_REQ,
	LINE_CMD,
	LINE_RSP,
	LINE_BODY
};

enum GEN_TYPE
{
	GEN_CONST_CMD,
	GEN_GO_STRUCT,
	GEN_PROTO,
	GEN_PROTO_ID,
	GEN_REQ_HANDLE,
	GEN_REQ_REGISTER,
	GEN_ACK_HANDLE,
	GEN_ACK_REGISTER,
	GEN_PUB_REGISTER,
	GEN_PUB_HANDLE,
	GEN_PUB_DECLARE,
	GEN_GAME_REGISTER,
	GEN_GAME_HANDLE,
	GEN_GAME_DECLARE,
	GEN_GAME_TO_GIDIP
};
class CReadJson
{
public:
	void Init(string strSrcPath, string strDestPath);
	void Finish();
	void Deal(string strSrcPath, string strDestPath, GEN_TYPE eGenType);
	void Output(GEN_TYPE eType);
	LINE_TYPE  GetStrType(string& strSrc);
	void DealStructName(string& strSrc);
	void DealCmdId(string & strSrc);
	void DealBody(string &strSrc);
	void GenConstCmd();
	void GenGoStruct();
	void GenProto();
	void GenProtoId();
	void GenReqHandle();
	void GenReqRegister();
	void GenAckHandle();
	void GenAckRegister();
	void GenPubRegister();
	void GenPubHandle();
	void GenPubDeclare();
	void GenGameDeclare();
	void GenGameHandle();
	void GenGameRegister();
	void GenGame2Gidip();
	void GenLogRecord();
	//IDIP_DO_UPDATE_MONEY_REQ to IdipDoUpdateMoneyReq
	string Src2Stand(string strSrc);
	//IDIP_DO_UPDATE_MONEY_REQ to gmcmd_do_update_money_req (rsp to ack)
	string Src2Proto(string strSrc);
	//MenKe to men_ke... AreaId to area_id
	string Stand2Proto(string strSrc);
	//gmcmd_do_update_money_req to GmcmdDoUpdateMoneyReq
	string Proto2Stand(string strSrc);
	//gmcmd_do_update_money_req to gmcmd_do_update_money_ack
	string ProtoReq2Ack(string strSrc);
	//gmcmd_do_update_money_ack to gmcmd_do_update_money_req
	string ProtoAck2Req(string strSrc);

	void Start(int nProtoIdStart);
	CReadJson();
private:
	bool IsValidName(char szSrc);
	bool IsNum(char szSrc);
	bool IsSpace(char szSrc);
	int  Str2Int(string strSrc);
	string DelComment(string & strSrc);
	string DelSpace(string & strSrc);
	char Up2Low(char szSrc);
	char Low2Up(char szSrc);
	bool IsInt64(string strKey);
	bool ReqBodyNotInProto(string strKey);
	bool ProtoAckNotSet(string strKey);
	bool HasRoleId(CJsonStruct *pJson);
	bool HasOpenId(CJsonStruct *pJson);
	bool HasPartition(CJsonStruct *pJson);
	string StrReplace(string strSrc, string strSubSrc, string strSubDest);

	//AreaId:, to AreaId:null,
	string AddNum(string & strSrc);

	ifstream m_oRead;
	ofstream m_oWrite;
	int m_nState;  // 0 req  1 rsp
	string m_strSrcName;  // IDIP_DO_UPDATE_MONEY_REQ
	string m_strSrcID; //IDIP_DO_UPDATE_MONEY_REQ_ID
	string m_strStandName; //IdipDoUpdateMoneyReq
	string m_strProtoName; // gmcmd_do_update_money_req
	string m_strProtoId;  //  gmcmd_do_update_money_req_id
	int m_nCmdId;
	string m_strJsonBody;
	vector<string> m_vecSrcBody;
	int m_nProtoIdStart;

	set<string> m_setInt64;
	set<string> m_setReqBodyNotProto;
	set<string> m_setProtoAckNotSet;
};