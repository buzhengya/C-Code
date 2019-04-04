#pragma once
#include "wnet.h"
#include "singleton.h"


using namespace wind;

class CNetWin :public INet, public TSingleton<CNetWin>
{
public:
	//dwNetIOType ¿ØÖÆÍøÂç¿âÀàĞÍ
	IConnector * WAPI CreateConnector(uint32 dwNetIOType);

	IListener * WAPI CreateListener(uint32 dwNetIOType);
private:
};