#include "read_json.h"
#include <iomanip>

void CReadJson::Init(string strSrcPath, string strDestPath)
{
	m_oRead.open(strSrcPath);
	m_oWrite.open(strDestPath, ios::app);
	m_oWrite.flags(ios::left);
}

void CReadJson::Finish()
{
	m_oWrite << endl;
	m_oWrite.close();
	m_oRead.close();
}

void CReadJson::Deal(string strSrcPath, string strDestPath, GEN_TYPE eGenType)
{
	Init(strSrcPath, strDestPath);

	while (!m_oRead.eof())
	{
		string strSrc;
		getline(m_oRead, strSrc);

		LINE_TYPE eType = GetStrType(strSrc);

		switch (eType)
		{
		case LINE_INVALID:
			break;

		case LINE_REQ:
			DealStructName(strSrc);
			break;

		case LINE_CMD:
			DealCmdId(strSrc);
			break;

		case LINE_RSP:
			DealStructName(strSrc);
			break;

		case LINE_BODY:
			DealBody(strSrc);
			Output(eGenType);
			break;

		default:
			break;
		}
	}
	Finish();
}

void CReadJson::Output(GEN_TYPE eType)
{
	switch (eType)
	{
	case GEN_CONST_CMD:
		GenConstCmd();
		break;

	case GEN_GO_STRUCT:
		GenGoStruct();
		break;

	case GEN_PROTO:
		GenProto();
		break;

	case GEN_PROTO_ID:
		GenProtoId();
		break;

	case GEN_REQ_HANDLE:
		GenReqHandle();
		break;

	case GEN_REQ_REGISTER:
		GenReqRegister();
		break;

	case GEN_ACK_HANDLE:
		GenAckHandle();
		break;

	case GEN_ACK_REGISTER:
		GenAckRegister();
		break;

	case GEN_PUB_REGISTER:
		GenPubRegister();
		break;

	case GEN_PUB_HANDLE:
		GenPubHandle();
		break;

	case GEN_PUB_DECLARE:
		GenPubDeclare();
		break;

	case GEN_GAME_REGISTER:
		GenGameRegister();
		break;

	case GEN_GAME_HANDLE:
		GenGameHandle();
		break;

	case GEN_GAME_DECLARE:
		GenGameDeclare();
		break;

	case GEN_GAME_TO_GIDIP:
		GenGame2Gidip();
		break;

	default:
		cout << "gen func type is not find." << endl;
		break;
	}
}

LINE_TYPE CReadJson::GetStrType(string& strSrc)
{
	if (strSrc.find("[request]") != string::npos)
	{
		return LINE_REQ;
	}
	
	if (strSrc.find("\"Cmdid\"") != string::npos)
	{
		return LINE_CMD;
	}

	if (strSrc.find("\"body\"") != string::npos)
	{
		return LINE_BODY;
	}

	if (strSrc.find("[rsponse]") != string::npos)
	{
		return LINE_RSP;
	}
	return LINE_INVALID;
}

void CReadJson::DealStructName(string& strSrc)
{
	int nSt = strSrc.find("IDIP_");
	int i = nSt;
	for (; i < strSrc.size(); i++)
	{
		if (!IsValidName(strSrc[i]))
		{
			break;
		}
	}
	int nEnd = i;

	m_strSrcName = strSrc.substr(nSt, nEnd - nSt);
}

void CReadJson::DealCmdId(string & strSrc)
{
	int nSt = 0, i = 0;
	for (; i < strSrc.size(); i++)
	{
		if (IsNum(strSrc[i]))
		{
			break;
		}
	}
	nSt = i;
	for (; i < strSrc.size(); i++)
	{
		if (!IsNum(strSrc[i]))
		{
			break;
		}
	}
	int nEnd = i;
	m_nCmdId = Str2Int(strSrc.substr(nSt, nEnd - nSt));
}

/*
"body" :
{
	"AreaId" : ,        
	"Partition" :,
	"PlatId" : ,      
	"OpenId" : "",
	"RoleId" : ,      
	"BeginTime" : ,  
	"EndTime" : ,     
	"PageNo" :         
}
 {数量不为0且 { }数量匹配时 读取字符串结束
*/
void CReadJson::DealBody(string &strSrc)
{
	int nLeft = 0, nRight = 0;
	while (!m_oRead.eof() && (nLeft == 0 || nLeft != nRight))
	{
		string strSrc;
		getline(m_oRead, strSrc);
		if (strSrc.find('{') != string::npos)
		{ 
			nLeft++;
		}
		if (strSrc.find('}') != string::npos)
		{
			nRight++;
		}
		m_vecSrcBody.push_back(strSrc);
	}
	m_strJsonBody = "";
	for (auto i = 0; i < m_vecSrcBody.size(); i++)
	{
		string strDest = DelComment(m_vecSrcBody[i]);
		strDest = DelSpace(strDest);
		if (i + 1 < m_vecSrcBody.size() && m_vecSrcBody[i + 1].find('[') == string::npos && m_vecSrcBody[i + 1].find('{') == string::npos)
		{
			strDest = AddNum(strDest);
		}
		m_strJsonBody += strDest;
	}
	m_vecSrcBody.clear();
}

void CReadJson::GenConstCmd()
{
	m_strSrcID = m_strSrcName + "_ID";
	m_oWrite << "\t" + m_strSrcName + "_ID" << " = " << m_nCmdId << endl;
}

void CReadJson::GenGoStruct()
{
	cout << m_strJsonBody << endl;
	CJson oJson(m_strJsonBody);
	VecJsonOutput vecJsonOutput = oJson.Output();
	for (const auto & it : vecJsonOutput)
	{
		string strStruct = it->GetStructName();
		if (strStruct == "")
		{
			strStruct = Src2Stand(m_strSrcName);
			m_strSrcName = strStruct;
		}
		m_oWrite << "type S" << strStruct + " struct {" << endl;
		for (const auto & itField : it->GetVecField())
		{
			string strType = "";
			JSON_VALUE_TYPE eType = itField.m_eType;
			switch (eType)
			{
			case JSON_VALUE_NUM:
				if (IsInt64(itField.m_strKey))
				{
					strType = "int64";
				}
				else
				{
					strType = "int32";
				}
				break;

			case JSON_VALUE_STR:
				strType = "string";
				break;

			case JSON_VALUE_OBJ:
				strType = "S" + itField.m_strKey;
				break;

			case JSON_VALUE_ARRAY_NUM:
				if (IsInt64(itField.m_strKey))
				{
					strType = "[]int64";
				}
				else
				{
					strType = "[]int32";
				}
				break;

			case JSON_VALUE_ARRAY_STR:
				strType = "[]string";
				break;

			case JSON_VALUE_ARRAY_OBJ:
				strType = "[]S" + itField.m_strKey;
				break;

			default:
				break;
			}

			m_oWrite << "\t" + itField.m_strKey + "\t\t" + strType << endl;
		}
		m_oWrite << "}" << endl;
	}
}

void CReadJson::GenProto()
{
	cout << m_strJsonBody << endl;
	CJson oJson(m_strJsonBody);
	VecJsonOutput vecJsonOutput = oJson.Output();
	for (const auto & it : vecJsonOutput)
	{
		string strStruct = it->GetStructName();
		string strId = "";
		if (strStruct == "")
		{
			strStruct = Src2Proto(m_strSrcName);
			m_strProtoName = strStruct;
			strId = strStruct + "_id";
		}
		else
		{
			strStruct = Stand2Proto(strStruct);
		}
		m_oWrite << "message " << strStruct << endl;
		m_oWrite << "{" << endl;
		int nIndex = 1;
		if (strId != "")
		{
			m_oWrite << "\toptional ESSProtoID\t\tproto_id\t\t\t= 1 [default = " << strId << " ];" << endl;
			m_oWrite << "\toptional GMCMD_SRC_TYPE\tcommand_src\t\t\t= 2 [default = GMCMD_SRC_GIDIP];" << endl;
			m_oWrite << "\toptional int64\t\t\tmsg_seq\t\t\t\t= 3;" << endl;
			m_oWrite << "\toptional int64\t\t\tgidip_session_id\t= 4;" << endl;
			
			nIndex = 5;
			if (strStruct.find("req") != string::npos)
			{
				m_oWrite << "\toptional int64\t\t\trole_id\t\t\t\t= 5;" << endl;
				m_oWrite << "\toptional int64\t\t\tgame_session_id\t\t= 6;" << endl;
				nIndex = 7;
			}
		}
		for (auto & itField : it->GetVecField())
		{
			if (ReqBodyNotInProto(itField.m_strKey))
			{
				continue;
			}
			JSON_VALUE_TYPE eType = itField.m_eType;
			string strType = "";
			string strFieldName = Stand2Proto(itField.m_strKey);
			switch (eType)
			{
			case JSON_VALUE_NUM:
				if (IsInt64(itField.m_strKey))
				{
					strType = "\toptional int64";
				}
				else
				{
					strType = "\toptional int32";
				}
				break;

			case JSON_VALUE_STR:
				strType = "\toptional string";
				break;

			case JSON_VALUE_OBJ:
				strType = "\toptional " + Stand2Proto(itField.m_strKey);
				strFieldName += "s";
				break;

			case JSON_VALUE_ARRAY_NUM:
				if (IsInt64(itField.m_strKey))
				{
					strType = "\trepeated int64";
				}
				else
				{
					strType = "\trepeated int32";
				}
				break;

			case JSON_VALUE_ARRAY_STR:
				strType = "\trepeated string";
				break;

			case JSON_VALUE_ARRAY_OBJ:
				strType = "\trepeated " + Stand2Proto(itField.m_strKey) + "";
				strFieldName += "s";
				break;

			default:
				cout << "GenProto eType is not find." << endl;
				break;
			}
			m_oWrite << left << setw(25) << strType << setw(20) << strFieldName << "= " << nIndex++ << ";" << endl;
		}
		m_oWrite << "}\n" << endl;
	}
}

void CReadJson::GenProtoId()
{
	CJson oJson(m_strJsonBody);
	VecJsonOutput vecJsonOutput = oJson.Output();
	string strStruct = "\t" + Src2Proto(m_strSrcName);
	m_oWrite << left << setw(41) << strStruct + "_id" << "= " << m_nProtoIdStart++ << ";" << endl;
}

void CReadJson::GenReqHandle()
{
	if (m_strSrcName.find("_RSP") != string::npos)
	{
		return;
	}

	CJson oJson(m_strJsonBody);
	VecJsonOutput vecJsonOutput = oJson.Output();
	CJsonStruct * pJson = nullptr;
	for (auto & it : vecJsonOutput)
	{
		if (it->GetStructName() == "")
		{
			pJson = it;
			break;
		}
	}

	string strStand = Src2Stand(m_strSrcName);
	string strHandle = "Handle" + strStand; //HandleIdipQueryInfoCurPersonReq
	string strStruct = "S" + strStand; //SIdipQueryInfoCurPersonReq
	string strProto = Proto2Stand(Src2Proto(m_strSrcName)); // GmcmdQueryInfoCurPewrsonReq

	m_oWrite << "type " << strHandle << " struct{}\n" << endl;
	m_oWrite << "func (p *" << strHandle + ") Handle(strDataPacket string) (int64, *NetSession) {" << endl;
	m_oWrite << "\tpReq := new(" << strStruct << ")" << endl;
	m_oWrite << "\tif !ParseDataBody(&strDataPacket, pReq) {" << endl;
	m_oWrite << "\t\treturn PARSE_BODY_ERROR, nil" << endl;
	m_oWrite << "\t}\n" << endl;
	m_oWrite << "\tnPlatId := GenPlat(pReq.PlatId, pReq.AreaId)" << endl;
	m_oWrite << "\tnAreaId := pReq.Partition" << endl;
	
	m_oWrite << "\tbIsOnline, nGameSessionId, nRoleId := CheckRoleByOpenId(int32(nPlatId), int32(nAreaId), pReq.OpenId)" << endl;
	m_oWrite << "\tif nRoleId == 0 {" << endl;
	m_oWrite << "\t\tlogger.Errorf(\"open id %s not exist role in area %d.\", pReq.OpenId, nAreaId)" << endl;
	m_oWrite << "\t}\n" << endl;

	m_oWrite << "\tif bIsOnline == false {" << endl;
	m_oWrite << "\t\tnGameSessionId = 0" << endl;
	m_oWrite << "\t}\n" << endl;
	m_oWrite << "\tpSession := GetNetSessionMgr().FindRandPub(int32(nPlatId), int32(nAreaId))" << endl;
	m_oWrite << "\tif pSession == nil {" << endl;
	m_oWrite << "\t\treturn AREA_NOT_EXIST, nil" << endl;
	m_oWrite << "\t}\n" << endl;

	m_oWrite << "\tpMsg := new(" << strProto << ")" << endl;
	m_oWrite << "\tnMsgSeq := pSession.GenMsgId()" << endl;
	m_oWrite << "\tpMsg.MsgSeq = proto.Int64(nMsgSeq)" << endl;
	m_oWrite << "\tpMsg.GameSessionId = proto.Int64(nGameSessionId)" << endl;
	m_oWrite << "\tpMsg.RoleId = proto.Int64(nRoleId)" << endl;

	for (auto & it : pJson->GetVecField())
	{
		if (ReqBodyNotInProto(it.m_strKey))
		{
			continue;
		}
		JSON_VALUE_TYPE eType = it.m_eType;
		string strType = "";
		switch (eType)
		{
		case JSON_VALUE_NUM:
			if (IsInt64(it.m_strKey))
			{
				strType = "Int64";
			}
			else
			{
				strType = "Int32";
			}
			break;

		case JSON_VALUE_STR:
			strType = "String";
			break;
		case JSON_VALUE_OBJ:
			break;
		case JSON_VALUE_ARRAY_NUM:
			break;
		case JSON_VALUE_ARRAY_STR:
			break;
		case JSON_VALUE_ARRAY_OBJ:
			break;
		default:
			break;
		}
		if (strType == "")
		{
			continue;
		}
		m_oWrite << "\tpMsg." << it.m_strKey << " = proto." << strType << "(pReq." << it.m_strKey << ")" << endl;
	}

	m_oWrite << "\n\tnLen := pSession.SendProto(pMsg, pMsg.GetProtoId())" << endl;
	m_oWrite << "\tif nLen == 0 {" << endl;
	m_oWrite << "\t\treturn SERVER_BUSY, nil" << endl;
	m_oWrite << "\t}" << endl;
	m_oWrite << "\treturn nMsgSeq, pSession" << endl;
	m_oWrite << "}\n" << endl;
}

void CReadJson::GenReqRegister()
{
	if (m_strSrcName.find("RSP") != string::npos)
	{
		return;
	}
	string strStand = Src2Stand(m_strSrcName);
	m_oWrite << "\tp.MapHandle[" << m_strSrcName + "_ID" << "] = new(Handle" << strStand + ")" << endl;
}

void CReadJson::GenAckHandle()
{
	if (m_strSrcName.find("_RSP") == string::npos)
	{
		return;
	}
	string strProto = Src2Proto(m_strSrcName); //gmcmd_do_update_money_ack
	string strStand = Proto2Stand(strProto); //GmcmdDoUpdateMoneyAck
	string strHandle = strStand + "Handle";//GmcmdDoUpdateMoneyAckHandle
	string strRsp = "S" + Src2Stand(m_strSrcName); // SIdipDoUpdateMoneyRsp
	
	m_oWrite << "type " << strHandle << " struct{}\n" << endl;
	m_oWrite << "func (p *" << strHandle << ") HandleProto(sliProtoCnt []byte, pNS *NetSession) bool {" << endl;
	m_oWrite << "\tpMsg := new(" << strStand << ")" << endl;
	m_oWrite << "\tif ParseProtoMsg(sliProtoCnt, pMsg) == false {" << endl;
	m_oWrite << "\t\treturn false" << endl;
	m_oWrite << "\t}\n" << endl;
	m_oWrite << "\tpRet := new(" << strRsp << ")" << endl;
	m_oWrite << "\tpRet.Result = pMsg.GetResult()" << endl;
	m_oWrite << "\tpRet.RetMsg = GetErrorMsg(pMsg.GetResult())" << endl;

	CJson oJson(m_strJsonBody);
	VecJsonOutput vecJsonOutput = oJson.Output();
	CJsonStruct * pJson = nullptr;
	for (auto & it : vecJsonOutput)
	{
		if (it->GetStructName() == "")
		{
			pJson = it;
			break;
		}
	}

	for (const auto & it : pJson->GetVecField())
	{
		if(it.m_strKey == "RetMsg" || it.m_strKey == "Result")
		{
			continue;
		}
		m_oWrite << "\tpRet." << it.m_strKey << " = pMsg.Get" << it.m_strKey << "()" << endl;
	}

	m_oWrite << "\n\tlogger.Debug(\"receive " << strProto << " to ChanRetBody\")" << endl;
	m_oWrite << "\tpNS.ChanRetBody <- SRetBody{pMsg.GetMsgSeq(), pRet}" << endl;
	m_oWrite << "\treturn true" << endl;
	m_oWrite << "}\n" << endl;
}

void CReadJson::GenAckRegister()
{
	if (m_strSrcName.find("_RSP") == string::npos)
	{
		return;
	}
	string strProto = Src2Proto(m_strSrcName); //gmcmd_do_update_money_ack
	string strStand = Proto2Stand(strProto); //GmcmdDoUpdateMoneyAck
	string strId = "ESSProtoID_" + strProto + "_id";
	string strHandle = strStand + "Handle";
	m_oWrite << "\tpMgr.pDealerMap[int(" << strId << ")] = new(" << strHandle << ")" << endl;
}

void CReadJson::GenPubRegister()
{
	if (m_strSrcName.find("_REQ") == string::npos)
	{
		return;
	}
	string strProto = Src2Proto(m_strSrcName); // gmcmd_query_info_cur_person_req
	string strHandle = Proto2Stand(strProto); // GmcmdQueryInfoCurPersonReq
	string strProtoId = strProto + "_id";
	m_oWrite << "\t\tRegisterDealer(SProtoSpace::" << strProtoId << ", &CPub2GidipHandler::" << strHandle << ") &&" << endl;
}

void CReadJson::GenPubHandle()
{
	if (m_strSrcName.find("_REQ") == string::npos)
	{
		return;
	}
	string strProtoReq = Src2Proto(m_strSrcName); // gmcmd_query_info_cur_person_req
	string strHandle = Proto2Stand(strProtoReq); // GmcmdQueryInfoCurPersonReq
	string strProtoAck = ProtoReq2Ack(strProtoReq); // gmcmd_query_info_cur_person_ack

	m_oWrite << "bool CPub2GidipHandler::" << strHandle << "(const char* pszData, uint32 nLen, CPub2GidipSession* pSession)" << endl;
	m_oWrite << "{" << endl;
	m_oWrite << "\t" << strProtoReq << " oReqMsg;" << endl;
	m_oWrite << "\tPARSE_PROTO(" << strProtoReq << ", oReqMsg, pszData, nLen);\n" << endl;
	m_oWrite << "\tEXLOG_DEBUG << \"[GMCMD] Receive " << strProtoReq << " successful!\";" << endl;

	m_oWrite <<
		"\tif (oReqMsg.game_session_id() != 0)\n\
\t{\n\
\t\toReqMsg.set_gidip_session_id(pSession->GetPipeID());\n\
\t\tCPub2GameSession *pGameSession = CPipeMgr::Instance()->FindGameSession(oReqMsg.game_session_id());\n\
\t\tif (pGameSession == nullptr)\n\
\t\t{" << endl;
	m_oWrite << "\t\t\t" << strProtoAck << " oAckMsg;" << endl;
	m_oWrite << "\
\t\t\toAckMsg.set_result(INVALID_32BIT_ID);\n\
\t\t\tpSession->Send(oAckMsg.proto_id(), oAckMsg);\n\
\t\t\tEXLOG_ERROR << \"game session can not find.id :\" << oReqMsg.game_session_id() << \" proto id : \" << oReqMsg.proto_id();\n\
\t\t}\n\
\t\telse\n\
\t\t{\n\
\t\t\tpGameSession->Send(oReqMsg.proto_id(), oReqMsg, oReqMsg.role_id());\n\
\t\t}\n\
\t}\n\
\telse\n\
\t{\n\
\t}\n\
\treturn true;\n\
}\n" << endl;
}

void CReadJson::GenPubDeclare()
{
	if (m_strSrcName.find("_REQ") == string::npos)
	{
		return;
	}
	string strHandle = Proto2Stand(Src2Proto(m_strSrcName));
	m_oWrite << "\tbool " << strHandle << "(const char* pszData, uint32 nLen, CPub2GidipSession* pSession);" << endl;
}

void CReadJson::GenGameDeclare()
{
	if (m_strSrcName.find("_REQ") == string::npos)
	{
		return;
	}
	string strHandle = Proto2Stand(Src2Proto(m_strSrcName));
	m_oWrite << "\tbool " << strHandle << "(const char* pszData, uint32 nLen, CGame2PubSession* pSession, uint64 nRoleId);" << endl;
}

void CReadJson::GenGameHandle()
{
	if (m_strSrcName.find("_RSP") == string::npos)
	{
		return;
	}
	CJson oJson(m_strJsonBody);
	VecJsonOutput vecJsonOutput = oJson.Output();
	CJsonStruct * pJson = nullptr;
	for (auto & it : vecJsonOutput)
	{
		if (it->GetStructName() == "")
		{
			pJson = it;
			break;
		}
	}
	string strProtoAck = Src2Proto(m_strSrcName); // gmcmd_query_info_cur_person_ack
	string strProtoReq = ProtoAck2Req(strProtoAck); // gmcmd_query_info_cur_person_req
	string strHandle = Proto2Stand(strProtoReq); // GmcmdQueryInfoCurPersonReq
	string strSet = "oAckMsg.set_result(GIDIP_SUCCESS);\n"; // oAckMsg.set_result....
	strSet += "\toAckMsg.set_msg_seq(oReqMsg.msg_seq());\n";

	for (const auto & it : pJson->GetVecField())
	{
		if (it.m_strKey == "RetMsg" || it.m_strKey == "Result")
		{
			continue;
		}
		strSet += "\toAckMsg.set_" + Stand2Proto(it.m_strKey) + "();\n";
	}

	string strSrc = "\
bool CGame2PubHandler::strHandle(const char* pszData, uint32 nLen, CGame2PubSession* pSession, uint64 nRoleId)\n\
{\n\
	SProtoSpace::strProtoReq oReqMsg;\n\
	PARSE_PROTO(SProtoSpace::strProtoReq, oReqMsg, pszData, nLen);\n\
	CRole* pRole = CRoleMgr::Instance()->FindRole(nRoleId);\n\
	\n\
	if (pRole == nullptr)\n\
	{\n\
		EXLOG_DEBUG << \"[GMCMD] \" << oReqMsg.GetDescriptor()->name() << std::endl << oReqMsg.Utf8DebugString();\n\
		return true;\n\
	}\n\
	\n\
	SProtoSpace::strProtoAck oAckMsg;\n\
	strSet\
	\n\
	EXLOG_DEBUG << \"[GMCMD] \" << oAckMsg.GetDescriptor()->name() << \" size:\" << oAckMsg.ByteSize()\n\
		<< std::endl << oAckMsg.DebugString();\n\
	pSession->Send(oAckMsg.proto_id(), oAckMsg, oReqMsg.gidip_session_id());\n\
	return true;\n\
}\n";
	strSrc = StrReplace(strSrc, "strHandle", strHandle);
	strSrc = StrReplace(strSrc, "strProtoReq", strProtoReq);
	strSrc = StrReplace(strSrc, "strProtoAck", strProtoAck);
	strSrc = StrReplace(strSrc, "strSet", strSet);

	m_oWrite << strSrc << endl;
}

void CReadJson::GenGameRegister()
{
	if (m_strSrcName.find("_REQ") == string::npos)
	{
		return;
	}
	string strProto = Src2Proto(m_strSrcName); // gmcmd_query_info_cur_person_req
	string strHandle = Proto2Stand(strProto); // GmcmdQueryInfoCurPersonReq
	string strProtoId = strProto + "_id";
	m_oWrite << "\t\tRegisterDealer(SProtoSpace::" << strProtoId << ", &CGame2PubHandler::" << strHandle << ") &&" << endl;
}

void CReadJson::GenGame2Gidip()
{
	if (m_strSrcName.find("_RSP") == string::npos)
	{
		return;
	}
	string strProtoId = Src2Proto(m_strSrcName) + "_id";
	string strSrc = "\tm_setToGidipMsgId.insert(strProtoId);";
	m_oWrite << StrReplace(strSrc, "strProtoId", strProtoId) << endl;
}

void CReadJson::GenLogRecord()
{

}

//IDIP_DO_UPDATE_MONEY_REQ to IdipDoUpdateMoneyReq
string CReadJson::Src2Stand(string strSrc)
{
	string strDest = "";
	for (int i = 0; i < strSrc.size(); i++)
	{
		strDest += strSrc[i];
		i++;
		for (; i < strSrc.size() && strSrc[i] != '_'; i++)
		{
			strDest += Up2Low(strSrc[i]);
		}
	}
	return strDest;
}

std::string CReadJson::Src2Proto(string strSrc)
{
	strSrc = strSrc.substr(4);
	string strDest = "gmcmd";
	for (auto i = 0; i < strSrc.size(); i++)
	{
		strDest += Up2Low(strSrc[i]);
	}
	if (strDest.substr(strDest.size() - 3) == "rsp")
	{
		strDest = strDest.substr(0, strDest.size() - 3) + "ack";
	}
	return strDest;
}

string CReadJson::Stand2Proto(string strSrc)
{
	string strDest = "";
	strDest += Up2Low(strSrc[0]);
	for (auto i = 1; i < strSrc.size(); i++)
	{
		if (strSrc[i] >= 'A' && strSrc[i] <= 'Z')
		{
			strDest += "_";
		}
		strDest += Up2Low(strSrc[i]);
	}
	return strDest;
}

std::string CReadJson::Proto2Stand(string strSrc)
{
	string strDest = "";
	for (int i = 0; i < strSrc.size(); i++)
	{
		strDest += Low2Up(strSrc[i]);
		i++;
		for (; i < strSrc.size() && strSrc[i] != '_'; i++)
		{
			strDest += Up2Low(strSrc[i]);
		}
	}
	return strDest;
}

string CReadJson::ProtoReq2Ack(string strSrc)
{
	string strDest = strSrc.substr(0, strSrc.size() - 3);
	return strDest + "ack";
}

std::string CReadJson::ProtoAck2Req(string strSrc)
{
	string strDest = strSrc.substr(0, strSrc.size() - 3);
	return strDest + "req";
}

void CReadJson::Start(int nProtoIdStart)
{
	m_nProtoIdStart = nProtoIdStart;
	//vector<GEN_TYPE> vecGenType = { GEN_CONST_CMD,GEN_GO_STRUCT ,GEN_PROTO ,GEN_PROTO_ID ,GEN_REQ_HANDLE,
	//	GEN_REQ_REGISTER ,GEN_ACK_HANDLE ,GEN_ACK_REGISTER ,GEN_PUB_REGISTER ,GEN_PUB_HANDLE,GEN_PUB_DECLARE,
	//GEN_GAME_REGISTER,GEN_GAME_DECLARE,GEN_GAME_HANDLE,GEN_GAME_TO_GIDIP };
	vector<GEN_TYPE> vecGenType = { GEN_REQ_HANDLE };
	for (const auto & it : vecGenType)
	{
		Deal("input.json", "output.txt", it);
	}
}

CReadJson::CReadJson()
{
	m_setInt64.insert("RoleId");
	m_setInt64.insert("LastLogoutTime");
	m_setInt64.insert("RegTime");
	m_setInt64.insert("UpdateTime");
	m_setInt64.insert("BeginEffectTime");
	m_setInt64.insert("EndEffectTime");
	m_setInt64.insert("Time");
	m_setInt64.insert("Id");

	m_setReqBodyNotProto.insert("AreaId");
	m_setReqBodyNotProto.insert("Partition");
	m_setReqBodyNotProto.insert("PlatId");
	m_setReqBodyNotProto.insert("RetMsg");
	m_setReqBodyNotProto.insert("OpenId");
	m_setReqBodyNotProto.insert("RoleId");

	m_setProtoAckNotSet.insert("proto_id");
	m_setProtoAckNotSet.insert("");
	m_setProtoAckNotSet.insert("");
}

bool CReadJson::IsValidName(char szSrc)
{
	return (szSrc >= 'A'&&szSrc <= 'Z') || szSrc == '_';
}

bool CReadJson::IsNum(char szSrc)
{
	return szSrc >= '0' && szSrc <= '9';
}

bool CReadJson::IsSpace(char szSrc)
{
	return szSrc == ' ' || szSrc == '\t' || szSrc == '\n';
}

int CReadJson::Str2Int(string strSrc)
{
	int nVal = 0;
	for (int i = 0; i < strSrc.size(); i++)
	{
		if (!IsNum(strSrc[i]))
		{
			cout << "Str2Int failed! contain illegal num. strSrc :" << strSrc << endl;
			return 0;
		}
		nVal = nVal * 10 + strSrc[i] - '0';
	}
	return nVal;
}

std::string CReadJson::DelComment(string & strSrc)
{
	int nEnd = strSrc.size();
	int i = 0;
	for (; i + 1 < strSrc.size(); i++)
	{
		if (strSrc[i] =='/' && strSrc[i+1] == '*')
		{
			nEnd = i;
			break;
		}
	}
	return strSrc.substr(0, nEnd);
}

std::string CReadJson::DelSpace(string & strSrc)
{
	string strDest = "";
	for (int i = 0; i < strSrc.size(); i++)
	{
		if (!IsSpace(strSrc[i]))
		{
			strDest += strSrc[i];
		}
	}
	return strDest;
}

char CReadJson::Up2Low(char szSrc)
{
	if (szSrc < 'A' || szSrc>'Z')
	{
		cout << "Up2Low is failed! szSrc : " << szSrc << endl;
		return szSrc;
	}
	return szSrc - 'A' + 'a';
}

char CReadJson::Low2Up(char szSrc)
{
	if (szSrc < 'a' || szSrc>'z')
	{
		cout << "Low2Up is failed! szSrc : " << szSrc << endl;
		return szSrc;
	}
	return szSrc - 'a' + 'A';
}

bool CReadJson::IsInt64(string strKey)
{
	if (m_setInt64.find(strKey) != m_setInt64.end() || strKey.find("Time") != string::npos || strKey.find("Id")!= string::npos)
	{
		return true;
	}
	return false;
}

bool CReadJson::ReqBodyNotInProto(string strKey)
{
	if (m_setReqBodyNotProto.find(strKey) != m_setReqBodyNotProto.end())
	{
		return true;
	}
	return false;
}

bool CReadJson::ProtoAckNotSet(string strKey)
{
	if (m_setProtoAckNotSet.find(strKey) != m_setProtoAckNotSet.end())
	{
		return true;
	}
	return false;
}

bool CReadJson::HasRoleId(CJsonStruct *pJson)
{
	for (const auto &it : pJson->GetVecField())
	{
		if (it.m_strKey == "RoleId")
		{
			return true;
		}
	}
	return false;
}

bool CReadJson::HasOpenId(CJsonStruct *pJson)
{
	for (const auto &it : pJson->GetVecField())
	{
		if (it.m_strKey == "OpenId")
		{
			return true;
		}
	}
	return false;
}

bool CReadJson::HasPartition(CJsonStruct *pJson)
{
	for (const auto &it : pJson->GetVecField())
	{
		if (it.m_strKey == "Partition")
		{
			return true;
		}
	}
	return false;
}

std::string CReadJson::StrReplace(string strSrc, string strSubSrc, string strSubDest)
{
	int nIndex = strSrc.find(strSubSrc);
	while (nIndex != string::npos)
	{
		strSrc = strSrc.substr(0, nIndex) + strSubDest + strSrc.substr(nIndex + strSubSrc.size());
		nIndex = strSrc.find(strSubSrc);
	}
	return strSrc;
}

//AreaId:, to AreaId:null,
std::string CReadJson::AddNum(string & strSrc)
{
	string strDest = "";
	int i = 0;
	for (; i < strSrc.size(); i++)
	{
		if (strSrc[i] == ':')
		{
			if (i + 1 >= strSrc.size() || strSrc[i + 1] == ',')
			{
				strDest += strSrc.substr(0, i + 1) + "null" + strSrc.substr(i + 1);
				return strDest;
			}
			break;
		}
	}
	return strSrc;
}
