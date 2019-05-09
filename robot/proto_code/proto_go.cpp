#include "proto_go.h"

bool CProtoGolang::GenGoCode(vector<CProtoMsg>& vecProtoMsg, const string & strDstFile, const string & strMoudle)
{
	_Clear();
	m_strDstFile = strDstFile;
	m_strMoudle = strMoudle;

	if (_OpenFile() == false)
	{
		cout << "open file : " << m_strDstFile << " failed." << endl;
		return false;
	}

	for (auto & it : vecProtoMsg)
	{
		_GenProtoId(it);
		_GenProtoName(it);
		_GenProtoStruct(it);

		if (it.GetMsgType() == MSG_REQ)
		{
			_DealReqMsg(it);
		}
		else if (it.GetMsgType() == MSG_ACK || it.GetMsgType() == MSG_NTF)
		{
			_DealAckOrNtfMsg(it);
		}
		else
		{
			cout << "invalid msg type. msg : " << endl;
			it.PrintMsg();
			_CloseFile();
			return false;
		}
	}

	_Write2File();
	_CloseFile();
	return true;
}

bool CProtoGolang::_OpenFile()
{
	if (!m_fStream.is_open())
	{
		m_fStream.open(m_strDstFile.c_str(), ios::out);
	}

	return m_fStream.is_open();
}

void CProtoGolang::_CloseFile()
{
	if (m_fStream.is_open())
	{
		m_fStream.close();
	}
}

void CProtoGolang::_Clear()
{
	_CloseFile();

	m_strDstFile = "";
	m_strMoudle = "";

	m_vecCommonSend.clear();
	m_vecSpecialSend.clear();
	m_vecDealFun.clear();
	m_vecSendStruct.clear();
	m_vecDealStruct.clear();
}

bool CProtoGolang::_DealReqMsg(CProtoMsg & oProtoMsg)
{
	//gen common send register
	string strRes = "";
	strRes = "p.MapProtoMsg[" + m_strProtoId + "] = new(" + m_strProtoName + ")";
	m_vecCommonSend.push_back(strRes);

	//gen special send register
	strRes = "";
	strRes = "p.MapSpecialProtoMsg[" + m_strProtoId + "] = new(" + m_strProtoStruct + ")";
	m_vecSpecialSend.push_back(strRes);

	//gen special send struct
	strRes = "";
	strRes += "type " + m_strProtoStruct + " struct {\n}\n\n";
	strRes += "func (p *" + m_strProtoStruct + ")SendMsg(pClient *SNetClient, strContent *string) {\n";
	strRes += "\tpProto := new(" + m_strProtoName + ")\n\n";
	strRes += "\terr := JSON2PB(*strContent, pProto)\n";
	strRes += "\tif err != nil {\n";
	strRes += "\t\tgolog.Logger.Warnf(\"unmarshaling " + m_strProtoName + " error : \", err)\n";
	strRes += "\t\treturn\n\t}\n";
	strRes += "\tdata, err := proto.Marshal(pProto)\n";
	strRes += "\tif err != nil {\n";
	strRes += "\t\tgolog.Logger.Warnf(\"" + m_strProtoName + " marshaling  error : %s\", err)\n";
	strRes += "\t\treturn\n";
	strRes += "\t}\n";
	
	string strProtoId = "";
	CProtoField oField;
	if (oProtoMsg.GetFieldByName("protoid", oField))
	{
		strProtoId = "GetProtoid()";
	}
	else if (oProtoMsg.GetFieldByName("proto_id", oField))
	{
		strProtoId = "GetProtoId()";
	}
	else
	{
		cout << "can not find valid proto id. msg : " << endl;
		oProtoMsg.PrintMsg();
		return false;
	}
	strRes += "\tpClient.SendMsg(data, int(pProto." + strProtoId + "))\n";
	strRes += "}\n";
	m_vecSendStruct.push_back(strRes);

	//gen stat count proto id register
	strRes = "";
	strRes = "p.SliStatProtoID = append(p.SliStatProtoID, " + m_strProtoId + ")";
	m_vecStatCountId.push_back(strRes);

	//gen stat count proto id map req -> ack
	strRes = "";
	string strAck = "";
	if (_Req2Ack(m_strProtoId, strAck) == false)
	{
		cout << "req msg can not find _req_. message : " << endl;
		oProtoMsg.PrintMsg();
		return false;
	}
	strRes = "p.ArrReqAndAck[" + m_strProtoId + "] = " + strAck;

	return true;
}

bool CProtoGolang::_DealAckOrNtfMsg(CProtoMsg & oProtoMsg)
{
	//gen deal func register
	string strRes = "";
	strRes = "p.MapDealFun[int(" + m_strProtoId + ")] = new(" + m_strProtoStruct + ")";
	m_vecDealFun.push_back(strRes);

	//gen handle msg struct
	strRes = "";
	strRes += "type " + m_strProtoStruct + " struct {\n";
	strRes += "\n";
	strRes += "}\n";
	strRes += "func (p *" + m_strProtoStruct + ")HandleMsg(pMsgpMsgConten []byte, pClient *SNetClient) int {\n";
	strRes += "\tpProto := new(" + m_strProtoName + ")\n";
	strRes += "\terr := proto.Unmarshal(pMsgpMsgConten, pProto)\n";
	strRes += "\tif err != nil {\n";
	strRes += "\t\tgolog.Logger.Warnf(\"unmarshaling " + m_strProtoName + " error : %s\", err)\n";
	strRes += "\t\treturn 1\n";
	strRes += "\t}\n";
	strRes += "\t";
	
	string strErr = "";
	CProtoField oField;
	if (oProtoMsg.GetFieldByName("ret", oField))
	{
		strErr = "GetRet()";
	}
	else if (oProtoMsg.GetFieldByName("error_code", oField))
	{
		strErr = "GetErrorCode()";
	}
	else if (oProtoMsg.GetFieldByName("error_codes", oField))
	{
		strErr = "GetErrorCodes()";
	}
	if (strErr != "")
	{
		strRes += "if pProto." + strErr + " != 0{\n";
		strRes += "\t\tgolog.Logger.Warnf(\"" + m_strProtoName + " ret is %d\",pProto." + strErr + ")\n";
		strRes += "\t}\n";
	}
	
	strRes += "\treturn 0\n";
	strRes += "}\n";
	m_vecDealStruct.push_back(strRes);

	//gen stat delay proto id register
	strRes = "";
	strRes = "p.SliStatProtoID = append(p.SliStatProtoID, " + m_strProtoId + ")";
	m_vecStatCountId.push_back(strRes);

	//gen stat delay proto id map  ack -> req
	strRes = "";
	strRes = "p.SliValidProtoID = append(p.SliValidProtoID," + m_strProtoId + ")";
	m_vecStatDelayId.push_back(strRes);
	
	if (m_strProtoId.find("_ack_") != string::npos)
	{
		strRes = "";
		string strReq = "";
		if (_Ack2Req(m_strProtoId, strReq) == false)
		{
			cout << "ack 2 req failed. proto id : " << m_strProtoId << endl;
			return false;
		}
		strReq = "p.ArrAckAndReq[" + m_strProtoId + "]  = " + strReq;
		m_vecStatDelayMap.push_back(strRes);
	}

	return true;
}

void CProtoGolang::_Write2File()
{
	_WriteCommonSend();
	_WriteSpecialSend();
	_WriteDealFun();
	_WriteSendStruct();
	_WriteDealStruct();
	_WriteStatCount();
	_WriteStatDelay();
}

void CProtoGolang::_WriteCommonSend()
{
	m_fStream << "//普通发送协议注册\n";
	m_fStream << "func " << m_strMoudle << "Register(p *SSender){\n";
	m_fStream << "\tSpecial" << m_strMoudle << "Register(p)\n\n";
	
	for (auto & it : m_vecCommonSend)
	{
		m_fStream << "\t" << it << endl;
	}

	m_fStream << "}\n\n";
}

void CProtoGolang::_WriteSpecialSend()
{
	m_fStream << "//特殊发送协议注册\n";
	m_fStream << "func Special" << m_strMoudle << "Register(p *SSender){\n\n";

	for (auto & it : m_vecSpecialSend)
	{
		m_fStream << "\t" << it << endl;
	}

	m_fStream << "}\n\n";
}

void CProtoGolang::_WriteDealFun()
{
	m_fStream << "//注册接收到信息时的处理函数\n";
	m_fStream << "func Init" << m_strMoudle << "DealFun(p *SHandler){\n\n";

	for (auto & it : m_vecDealFun)
	{
		m_fStream << "\t" << it << endl;
	}

	m_fStream << "}\n\n";
}

void CProtoGolang::_WriteSendStruct()
{
	m_fStream << "// * * * * * * * * * * * * * * * * *特殊发送协议结构体 * * * * * * * * *  * * * * * * * * * * * *\n\n";
	for (auto & it : m_vecSendStruct)
	{
		m_fStream << it << endl;
	}
	m_fStream << endl;
}

void CProtoGolang::_WriteDealStruct()
{
	m_fStream << "// * * * * * * * * * * * * * * * * * *处理函数结构体 * * * * * * * * * * * * * * * * * * * * * * * \n";
	for (auto & it : m_vecDealStruct)
	{
		m_fStream << it << endl;
	}
	m_fStream << endl;
}

void CProtoGolang::_WriteStatCount()
{
	m_fStream << "func (p *SStatProtoCount)" << m_strMoudle << "ProtoCount(){\n\n";

	for (auto & it : m_vecStatCountId)
	{
		m_fStream << "\t" << it << endl;
	}
	m_fStream << endl;

	for (auto & it : m_vecStatCountMap)
	{
		m_fStream << "\t" << it << endl;
	}

	m_fStream << "}\n\n";
}

void CProtoGolang::_WriteStatDelay()
{
	m_fStream << "func (p *SStatProtoDelay)" << m_strMoudle << "ProtoDelay(){\n\n";

	for (auto & it : m_vecStatDelayId)
	{
		m_fStream << "\t" << it << endl;
	}
	m_fStream << endl;

	for (auto & it : m_vecStatDelayMap)
	{
		m_fStream << "\t" << it << endl;
	}
	m_fStream << "}\n\n";
}

bool CProtoGolang::_GenProtoId(CProtoMsg & oProtoMsg)
{
	CProtoField oProtoField;
	if (!oProtoMsg.GetFieldByIndex(1,oProtoField) && !oProtoMsg.GetFieldByName("protoid", oProtoField) && 
		!oProtoMsg.GetFieldByName("proto_id", oProtoField))
	{
		cout << "can not find valid proto id field. proto : " << endl;
		oProtoMsg.PrintMsg();
		return false;
	}

	if (oProtoField.GetDefault() == "")
	{
		cout << "proto id default is null. proto : " << endl;
		oProtoMsg.PrintMsg();
		return false;
	}

	m_strProtoId = "SProtoSpace." + oProtoField.GetValType() + "_" + oProtoField.GetDefault();
	return true;
}

bool CProtoGolang::_GenProtoName(CProtoMsg & oProtoMsg)
{
	string strName = oProtoMsg.GetMsgName();
	string strHumpName = "";
	_Underline2Hump(strName, strHumpName);
	m_strProtoName = "SProtoSpace." + strHumpName;
	return true;
}

bool CProtoGolang::_GenProtoStruct(CProtoMsg & oProtoMsg)
{
	string strName = oProtoMsg.GetMsgName();
	string strHumpName = "";
	_Underline2Hump(strName, strHumpName);
	m_strProtoStruct = "S" + strHumpName;
	return true;
}

void CProtoGolang::_Underline2Hump(string & strSrc, string & strDst)
{
	vector<string> vecSplitStr;

	size_t nStart = 0, nEnd = 0;
	while (nStart < strSrc.size())
	{
		nEnd = nStart;
		while (nEnd < strSrc.size() && strSrc[nEnd] != '_')
		{
			nEnd++;
		}

		vecSplitStr.push_back(strSrc.substr(nStart, nEnd - nStart));
		nStart = nEnd + 1;
	}

	strDst = "";
	for (auto & it : vecSplitStr)
	{
		if (it.size() > 0 && IsLowCase(it[0]))
		{
			it[0] = 'A' + it[0] - 'a';
		}

		strDst += it;
	}
}

bool CProtoGolang::_Req2Ack(string & strReq, string & strAck)
{
	if (strReq.find("_req_") == string::npos)
	{
		return false;
	}
	size_t nStart = strReq.find("_req_");
	strAck = strReq.substr(0, nStart) + "_ack_" + strReq.substr(nStart + 5);
	return true;
}

bool CProtoGolang::_Ack2Req(string & strAck, string & strReq)
{
	if (strAck.find("_ack_") == string::npos)
	{
		return false;
	}
	size_t nStart = strAck.find("_ack_");
	strReq = strAck.substr(0, nStart) + "_req_" + strAck.substr(nStart + 5);
	return true;
}
