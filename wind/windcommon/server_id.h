#pragma once
#include "base_define.h"

struct SServerId
{
	uint16	nPlat;
	uint16	nArea;
	uint16	nType;
	uint16	nIndex;
};

class CServerId
{

private:
	SServerId		m_oServerId;
	uint64			m_nId;
};
