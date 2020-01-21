#pragma once
#include "wpipe.h"
#include "base_define.h"
#include "noncopy.h"

enum EBizId
{
};

class CPipeBizSession : public wind::IBizSession, public wind::CNonCopy
{
public:
	CPipeBizSession();
	virtual ~CPipeBizSession();

	virtual void OnRecv(const char * pBuf, const uint32 nLen) override {};

	virtual void SetBizId(uint16 nBizId) override final { m_nBizId = nBizId; }
	virtual uint16 GetBizId() override final { return m_nBizId; }

	virtual void SetPipe(wind::IPipe * pPipe) override final { m_pPipe = pPipe; }
	virtual wind::IPipe * GetPipe() override final { return m_pPipe; }

	inline  virtual uint64 GetPipeId() override final;



private:
	uint32			m_nLoad;
	uint16			m_nBizId;
	wind::IPipe *	m_pPipe;
};
