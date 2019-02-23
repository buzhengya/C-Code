#include "wthread.h"

namespace wind
{
	IThread::IThread()
	{
	}



	IThread::~IThread()
	{
		if (m_pThread != nullptr)
		{
			delete m_pThread;
			m_pThread = nullptr;
		}
	}

	bool IThread::_Start(TThreadProc pThrdProc, void *pArg)
	{

		m_pThread = new std::thread(pThrdProc, pArg);
		if (m_pThread == nullptr)
		{
			return false;
		}

		return true;
	}

	bool IThread::Start()
	{
		return _Start(IThread::ThreadFunc, this);
	}

	void IThread::SetId()
	{
		m_oId = std::this_thread::get_id();
	}

	void* IThread::ThreadFunc(void* pArgs)
	{
		auto* pThread = (IThread*)pArgs;
		if (pThread == nullptr)
		{
			return nullptr;
		}

		if (true)
		{
			std::unique_lock<std::mutex> oLock(pThread->m_oMutex);
			pThread->m_bAlive = true;
			pThread->m_oCondVar.notify_all();
		}

		pThread->SetRunning(true);
		pThread->BeforeMainLoop();
		pThread->SetId();
		pThread->MainLoop();
		pThread->AfterMainLoop();

		if (true)
		{
			std::unique_lock<std::mutex> oLock(pThread->m_oMutex);
			pThread->m_bAlive = false;
			pThread->m_oCondVar.notify_all();
		}

		return nullptr;
	}

	void IThread::ShutDown()
	{
		if (IsRunning())
		{
			Terminate();
			Join();
			OnTerminate();
		}
	}

	void IThread::Join()
	{
		if (nullptr != m_pThread)
		{
			m_pThread->join();

			std::unique_lock<std::mutex> lock(m_oMutex);
			while (m_bAlive)
				m_oCondVar.wait(lock);
		}
	}


	std::string IThread::GetDebugInfo()
	{
		std::stringstream  ss;
		ss << "[thread] id : " << m_oId;
		return ss.str();
	}
}