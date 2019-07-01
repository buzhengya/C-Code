#pragma once
#include "base_define.h"

namespace wind
{
	enum EPkgProtoId
	{
		PKG_PROTO_PING	=	1,
		PKG_PROTO_CONN_REQ	=	2,
		PKG_PROTO_CONN_ACK	=	3,
		PKG_PROTO_CONN_DATA	=	4
	};

	enum EConnRet
	{
		CONN_SUCCESS = 1,
		CONN_RMT_ID_NOT_MATCH = 2, //req rmt id not match server id
		CONN_NOT_IN_CONN_LIST = 3  //req local id not in conn list
	};

	struct SPkgConnReq
	{
		uint64		nLocalId;
		uint64		nRmtId;
	};

	struct SPkgConnAck
	{
		uint32		nRet;
	};
}; // namespace wind
