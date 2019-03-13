#include "log.h"

void CDefaultLogDispatchCallback::Handle(const CLogDispatchData * pData)
{

}

void CDefaultLogDispatchCallback::Dispatch(string && strLogLine)
{

}

void CLogDispatcher::Dispatch(CLogMessage * pMsg)
{
	//����һ�ι���
	CLogDispatchCallback * pCallback = CStorage::Instance()->GetCallBack();
	if (pCallback == nullptr)
	{
		std::cout << "call back is nullptr." << endl;
	}

	CLogDispatchData oData(pMsg);
	pCallback->Handle(&oData);
}

void CWriter::ProcessMsg()
{
	CLogDispatcher::Instance()->Dispatch(m_pMsg);
}

string CDefaultLogBuilder::Build(CLogMessage * pLogMsg)
{
	return string();
}
