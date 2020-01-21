#pragma once
#include <string>
#include "proto_parse.h"
#include <fstream>

class CProtoOutput
{
protected:
	bool _OpenFile();
	void _CloseFile();

	bool _Req2Ack(std::string & strReq, std::string & strAck);
	bool _Ack2Req(std::string & strAck, std::string & strReq);

	bool _GenProtoId(CProtoMsg & oProtoMsg, string & strProtoId);
	void _Underline2Hump(string & strSrc, string & strDst); //royal_info -> RoyalInfo

protected:
	std::string		m_strDstFile;
	std::ofstream	m_fStream;
};
