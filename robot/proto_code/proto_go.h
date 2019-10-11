#pragma once
#include "base_define.h"
#include "singleton.h"
#include "proto_parse.h"
#include <iostream>
#include <fstream>

using namespace std;

class CProtoGolang : public TSingleton<CProtoGolang>
{
	friend class TSingleton<CProtoGolang>;
public:
	bool GenGoCode(vector<CProtoMsg> & vecProtoMsg, const string & strDstFile, const string & strMoudle);

private:
	CProtoGolang() {}
	~CProtoGolang() {}

	bool _OpenFile();
	void _CloseFile();
	void _Clear();

	bool _DealReqMsg(CProtoMsg & oProtoMsg);
	bool _DealAckOrNtfMsg(CProtoMsg & oProtoMsg);
	void _Write2File();

	void _WriteCommonSend();
	void _WriteSpecialSend();
	void _WriteDealFun();
	void _WriteSendStruct();
	void _WriteDealStruct();
	void _WriteStatCount();
	void _WriteStatDelay();

	bool _GenProtoId(CProtoMsg & oProtoMsg);
	bool _GenProtoName(CProtoMsg & oProtoMsg);
	bool _GenProtoStruct(CProtoMsg & oProtoMsg);
	void _Underline2Hump(string & strSrc, string & strDst); //royal_info -> RoyalInfo
	bool _Req2Ack(string & strReq, string & strAck);
	bool _Ack2Req(string & strAck, string & strReq);

	string _StrToLow(const string & strSrc);

	//total val
	string			m_strMoudle;
	ofstream		m_fStream;
	string			m_strDstFile;

	vector<string>	m_vecCommonSend;
	vector<string>	m_vecSpecialSend;
	vector<string>	m_vecDealFun;
	vector<string>	m_vecSendStruct;
	vector<string>	m_vecDealStruct;
	vector<string>	m_vecStatCountId;
	vector<string>	m_vecStatCountMap;
	vector<string>	m_vecStatDelayId;
	vector<string>	m_vecStatDelayMap;

	//one msg tmp data
	string			m_strProtoId;//ECSProtoID_item_packet_req_id
	string			m_strProtoName;//ItemPacketReq
	string			m_strProtoStruct;//SItemPacketReq

};
