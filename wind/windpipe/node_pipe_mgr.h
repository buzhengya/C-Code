#pragma once
#include <map>

#include "base_define.h"
#include "singleton.h"

#include "node_pipe.h"

class CNodePipeMgr : public wind::TSingleton<CNodePipeMgr>
{
	using MapNodePipe = std::map<uint64, CNodePipe*>;
public:

private:
	
};
