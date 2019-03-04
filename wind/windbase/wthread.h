#pragma once
#include <string>
#include <sstream>
#include <thread>
#include <mutex>
#include <atomic>
#include "base_define.h"

namespace wind
{
	typedef void * (TThreadProc)(void *);
	class IThread
	{
	public:
		IThread();
		virtual ~IThread();

		bool _Start(TThreadProc pThrdProc, void *pArg);

		bool Start();


		void SetId();

		std::string GetStrId()
		{
			std::ostringstream oss;
			oss << std::this_thread::get_id();
			return oss.str();
		}

		uint64 GetId()
		{
			std::string stid = GetStrId();
			unsigned long long tid = std::stoull(stid);
			return tid;
		}

		virtual void MainLoop() = 0;

		virtual void BeforeMainLoop() {}

		virtual void AfterMainLoop() {}

		static void* ThreadFunc(void* pArgs);

		void ShutDown();

		void Join();

		void				Terminate() { SetRunning(false); }
		void				SetRunning(bool value) { m_bRunning = value; }
		bool				IsRunning() const { return m_bRunning; }

		void SleepMliSec(const uint32 nMilSec)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(nMilSec));
		}

	private:
		std::mutex                      m_oMutex;					/**< 互斥锁 */
		std::condition_variable         m_oCondVar;					/**< 条件变量 */
		volatile bool                   m_bAlive = false;			/**< 线程是否在运行 */

		std::atomic_bool                m_bRunning;				/**< 线程是否停止主循环 */
		std::thread*                    m_pThread = nullptr;
		std::thread::id                 m_oId;
	};
}