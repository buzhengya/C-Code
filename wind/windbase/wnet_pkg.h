#pragma once
#include "base_define.h"

namespace wind
{
	static const uint32 sPkgHeadLen = sizeof(SPkgHead);
	static const uint32 sNetHeadLen = sizeof(SNetHead);
	static const uint32 sPkgNetHeadLen = sPkgHeadLen + sNetHeadLen;

	struct SPkgHead
	{
		uint32		nMsgLen;
		uint32		nCheckSum;
	};

	struct SNetHead
	{
		int16		nMsgId;
		uint32		nMsgLen;
	};

	struct SPipeHead
	{
	};
}//namespace wind
