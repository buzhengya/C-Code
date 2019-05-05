#pragma once
#include "base_define.h"
#include "conndatamgr.h"
#include "loop_buf.h"

enum ENetEventType
{
	NETEVT_RCV = 0,
};

struct SNetRecvEvent
{
	CConnData * pConnData;
	uint32      nConnectionId;
	uint32      nLen;
	CLoopBuf *  pLoopBuf;
};

struct SNetEvent
{
	ENetEventType eType;
	union 
	{
		SNetRecvEvent stRecvEvt;
	};
};
