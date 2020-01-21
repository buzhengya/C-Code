#include "pipe_helper.h"

CPipeBizSession::CPipeBizSession()
{
	m_nLoad = 0;
	m_nBizId = 0;
	m_pPipe = nullptr;
}

CPipeBizSession::~CPipeBizSession()
{
	//if (m_pPipe != nullptr)
	//{
	//	delete m_pPipe;
	//}
}

inline uint64 CPipeBizSession::GetPipeId()
{
	if (m_pPipe != nullptr)
	{
		m_pPipe->GetRmtId();
	}
	return 0;
}
