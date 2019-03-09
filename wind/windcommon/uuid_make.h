#pragma once
#include "base_define.h"
#include "singleton.h"

using namespace wind;

class CUUIDMaker : public TSingleton<CUUIDMaker>
{
	friend class TSingleton<CUUIDMaker>;

public:
	uint64 GenUUID();

private:
	uint32 m_nSeqId;
};