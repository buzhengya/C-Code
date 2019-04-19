#pragma once
#include <boost/lockfree/spsc_queue.hpp>
#include <boost/lockfree/queue.hpp>
#include <string>
#include <sstream>
#include <list>
#include <atomic>
#include <mutex>
using namespace std;

template<typename T, int QUEUE_CAP = 10000>
class CLockFreeSpscQueue
{
	using LOCKFREE_QUEUE = boost::lockfree::spsc_queue<T, boost::lockfree::capacity<QUEUE_CAP> >;
public:
	CLockFreeSpscQueue()
	{}
	~CLockFreeSpscQueue()
	{}

	bool Init(const int nSize)
	{
		m_oDataQueue.is_lock_free();
	}

	void PushBack(const T& oData)
	{
		MoveListToQueue();
		if (m_oDataQueue.push(oData) == 0)
		{
			m_oTempList.push_back(oData);
		}
	}

	bool PopFront(T& oData)
	{
		MoveListToQueue();
		if (GetQueReadSize() > 0)
		{
			oData = m_oDataQueue.front();
			m_oDataQueue.pop();
			return true;
		}
		return false;
	}

	void MoveListToQueue()
	{
		while (!m_oTempList.empty())
		{
			auto & it : = m_oTempList.front();
			if (m_oDataQueue.push(it) == 0)
			{
				break;
			}
			m_oTempList.pop_front();
		}
	}

	bool DiscardFront()
	{
		MoveListToQueue();
		if (GetQueReadSize() > 0)
		{
			m_oDataQueue.pop();
			return true;
		}
		return false;
	}

	std::size_t GetQueReadSize() const
	{
		return m_oDataQueue.read_available();
	}

	std::size_t GetQueWriteSize() const
	{
		return m_oDataQueue.write_available();
	}

	std::size_t GetReadSize() const
	{
		return m_oDataQueue.read_available() + m_oTempList.size();
	}

	std::string DebugInfo()
	{
		ostringstream oSS;
		oSS << "[CLockFreeSpscQueue]"
			<< "data_queue size : " << QUEUE_CAP << ", "
			<< "read size : " << GetQueReadSize() << ", "
			<< "write size : " << GetQueWriteSize() << ", "
			<< "tmpList size : " << m_oTempList.size() << ", "
			<< "objName : " << string(typeid(T).name) << ", "
			<< "\n";
		return oSS.str();
	}

private:
	LOCKFREE_QUEUE			m_oDataQueue;
	std::list<T>			m_oTempList;				//中转队列， 当Queue满时，临时存放此队列中
};

#define NXQUEUE_CAPACITY 65534 
template<typename T, int QUEUE_CAP = NXQUEUE_CAPACITY>
class CLockFreeQueue
{
	using LOCKFREE_QUEUE = boost::lockfree::queue<T, boost::lockfree::capacity<QUEUE_CAP> >;
	using AtomicInt = std::atomic_int;
public:
	CLockFreeQueue()
	{}
	~CLockFreeQueue()
	{}

	bool Init()
	{
		return m_oQueue.is_lock_free();
	}

	int Size() const
	{
		return m_nElemtCount;
	}

	int Empty() const
	{
		return m_oQueue.empty() && m_oTempList.empty();
	}

	void PushBack(const T& oData)
	{
		MoveListToQueue();
		if (m_oQueue.push(oData))
		{
			++m_nElemtCount;
			return;
		}

		m_oLocker.lock();
		m_oTempList.push_back(oData);
		++m_nElemtCount;
		m_oLocker.unlock();
	}

	bool PopFront(T& oData)
	{
		if (m_oQueue.empty())
		{
			return false;
		}

		if (!m_oQueue.pop(oData))
		{
			return false;
		}
		--m_nElemtCount;
		return true;
	}

	void MoveListToQueue()
	{
		if (m_oTempList.empty())
		{
			return;
		}

		m_oLocker.lock();
		while (!m_oTempList.empty())
		{
			auto& oData = m_oTempList.front();
			if (m_oQueue.push(oData))
			{
				m_oTempList.pop_front();
				continue;
			}
			break;
		}
		m_oLocker.unlock();
	}

	std::string DebugInfo()
	{
		using namespace std;

		int nTotalSize = QUEUE_CAP * sizeof(T) + m_oTempList.size() * sizeof(T) + m_nTotalRawDataSize;

		stringstream oSS;
		oSS << "[LockFreeQueue] "
			<< "capacity=" << QUEUE_CAP << ", "
			<< "readSize=" << m_nElemtCount << ", "
			<< "tempListSize=" << m_oTempList.size() << ", "
			<< "objName=" << string(typeid(T).name()) << ", "
			<< "totalMem=" << nTotalSize
			<< " \n";

		return oSS.str();
	}

private:
	LOCKFREE_QUEUE			m_oQueue;
	std::list<T>			m_oTempList;       // cache here when queue is full 
	mutex					m_oLocker;
	AtomicInt				m_nElemtCount = { 0 };		// count of elements in queue 
	int						m_nTotalRawDataSize = 0;	//for debug 
};