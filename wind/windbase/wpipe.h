#pragma once
#include "base_define.h"

namespace wind
{
	class IPipe;
	class INetSession;
	class IBizSession
	{
	public:
		virtual void OnRecv(const char * pBuf, const uint32 nLen) = 0;

		virtual	uint64 GetPipeId() = 0;

		virtual void SetPipe(IPipe * pPipe) = 0;
		virtual IPipe * GetPipe() = 0;

		virtual void SetBizId(uint16 nBizId) = 0;
		virtual uint16 GetBizId() = 0;

		virtual uint32 GetLoad() = 0;
		virtual void   AddLoad() = 0;
		virtual void   SubLoad() = 0;
	};

	class IPipe
	{
	public:
		virtual void OnRecv(const char * pBuf, const uint32 nLen) = 0;

		virtual bool SetBizSession(uint16 nBizId, IBizSession * pSession) = 0;
		virtual uint16 GetBizId() = 0;
		virtual IBizSession * GetBizSession() = 0;

		virtual void SetNetSession(INetSession * pSession) = 0;
		virtual INetSession * GetNetSession() = 0;

		virtual void SetRmtId(uint64 nRmtId) = 0;
		virtual uint64 GetRmtId() = 0;

		virtual uint32 GetRmtIp() = 0;
	};

	class IPipeReporter
	{
	public:
		virtual void OnReporter(uint32 nErrCode, uint64 nPipeId) = 0;
	};

	class IPipeModule
	{
	public:
		virtual bool Init(const char * szPipeCfg, const char * szServerCfg, IPipeReporter * pReporter) = 0;
	};
}; //namespace std
