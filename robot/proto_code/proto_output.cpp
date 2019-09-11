#include "proto_output.h"
#include <iostream>

using namespace std;

bool CProtoOutput::_OpenFile()
{
	if (!m_fStream.is_open())
	{
		m_fStream.open(m_strDstFile.c_str(), ios::out);
	}

	return m_fStream.is_open();
}

void CProtoOutput::_CloseFile()
{
	if (m_fStream.is_open())
	{
		m_fStream.close();
	}
}

bool CProtoOutput::_Req2Ack(std::string & strReq, std::string & strAck)
{
	if (strReq.find("_req") == string::npos)
	{
		return false;
	}
	size_t nStart = strReq.find("_req");

	strAck = strReq.substr(0, nStart) + "_ack" + strReq.substr(nStart + 4);
	return true;
}

bool CProtoOutput::_Ack2Req(std::string & strAck, std::string & strReq)
{
	if (strAck.find("_ack") == string::npos)
	{
		cout << "ack : " << strAck << " can not find _ack" << endl;
		return false;
	}
	size_t nStart = strAck.find("_ack");
	strReq = strAck.substr(0, nStart) + "_req" + strAck.substr(nStart + 4);
	return true;
}

bool CProtoOutput::_GenProtoId(CProtoMsg & oProtoMsg, string & strProtoId)
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

	strProtoId = oProtoField.GetDefault();
	return true;
}

void CProtoOutput::_Underline2Hump(string & strSrc, string & strDst)
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
