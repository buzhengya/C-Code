#include "role.h"
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "uuid_make.h"
#include "log.h"

using namespace std;

char szCharSet[33] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ/@*%&#";

SRole::SRole()
{
	srand((unsigned)time(nullptr));
	m_nLv = rand() % 60 + 1;
	m_nId = CUUIDMaker::Instance()->GenUUID();

	char szName[14];
	uint64 nId = m_nId;
	for (auto i = 0; i < 13; i++)
	{
		szName[i] = szCharSet[nId & 31];
		nId = (nId >> 5);
	}
	szName[13] = '\0';

	uint32 nNameLen = strlen(szName) + 1;
	uint32 nSigLen = strlen(szName) + 1;

	m_szName = new char[nNameLen];
	m_szSignature = new char[nSigLen];

	if (strcpy_s(m_szName, nNameLen, szName) != 0)
	{
		EXLOG_DEBUG << "copy name failed.";
	}
	if (strcpy_s(m_szSignature, nSigLen, szName) != 0)
	{
		EXLOG_DEBUG << "copy sig failed.";
	}
}

SRole::SRole(const char * szName, const char * szSig)
{
	if (szName == nullptr || szSig == nullptr)
	{
		Init();
		return;
	}

	srand((unsigned)time(nullptr));
	m_nLv = rand() % 60 + 1;
	m_nId = CUUIDMaker::Instance()->GenUUID();

	uint32 nNameLen = strlen(szName) + 1;
	uint32 nSigLen = strlen(szSig) + 1;

	m_szName = new char[nNameLen];
	m_szSignature = new char[nSigLen];

	strcpy_s(m_szName, nNameLen, szName);
	strcpy_s(m_szSignature, nSigLen, szSig);
}

SRole::~SRole()
{
	//EXLOG_DEBUG << "begin delete." ;
	if (m_szName != nullptr)
	{
		//EXLOG_DEBUG << "name delete." ;
		delete []m_szName;
	}

	if (m_szSignature != nullptr)
	{
		//EXLOG_DEBUG << "sig delete." ;
		delete []m_szSignature;
	}
}

char * SRole::Role2Data(uint32& nLen)
{
	nLen = 12;
	uint32 nNameLen = strlen(m_szName) + 1;
	uint32 nSigLen = strlen(m_szSignature) + 1;
	nLen += nNameLen + nSigLen;
	char *szData = new char[nLen];
	if (strcpy_s(szData, 8, (char *)&m_nId) != 0)
	{
		EXLOG_DEBUG << "copy id failed." ;
	}
	strcpy_s(szData + 8, 4, (char *)&m_nLv);
	strcpy_s(szData + 12, nNameLen, m_szName);
	strcpy_s(szData + 12 + nNameLen, nSigLen, m_szSignature);
	return szData;
}

void SRole::Data2Role(char * szData)
{
	if (szData == nullptr)
	{
		return;
	}
	uint32 nName = strlen(szData + 12) + 1;
	uint32 nSig = strlen(szData + 12 + nName) + 1;
	m_szName = new char[nName];
	m_szSignature = new char[nSig];
	strcpy_s((char *)&m_nId, 8, szData);
	strcpy_s((char *)&m_nLv, 4, szData);
	strcpy_s(m_szName, nName, szData + 12);
	strcpy_s(m_szSignature, nSig, szData + 12 + nName);
}

void SRole::PrintRole(char * szData)
{
	EXLOG_DEBUG << "id : " << m_nId << "  lv : " << m_nLv << " name : " << m_szName << " sig  : " << m_szSignature ;
}

void SRole::Init()
{
	m_nId = 0;
	m_nLv = 0;
	m_szName = nullptr;
	m_szSignature = nullptr;
}
