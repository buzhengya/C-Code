#pragma once
#include "base_define.h"

class SRole 
{
public:
	SRole();

	SRole(const char * szName, const char * szSig);

	~SRole();

	char * Role2Data(uint32& nLen);

	void Data2Role(char *szData);

	void PrintRole(char *szData);

	uint64 GetId() { return m_nId; }

	char * GetName() { return m_szName; }

	char * GetSig() { return m_szSignature; }
private:
	void Init();

	uint64 m_nId;
	uint32 m_nLv;
	char * m_szName;
	char * m_szSignature;
};
