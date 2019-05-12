#pragma once
#include "wnet.h"
#include "singleton.h"


using namespace wind;

class CNetWin :public INet, public TSingleton<CNetWin>
{
public:
	//dwNetIOType øÿ÷∆Õ¯¬Áø‚¿‡–Õ
	IConnector * WAPI CreateConnector(uint32 dwNetIOType);

	IListener * WAPI CreateListener(uint32 dwNetIOType);

	bool        WAPI Run(int32 nCount);

	bool		Init(uint32 nSize);
private:
	char *		m_pBuf;
	uint32		m_nBufSize;
};