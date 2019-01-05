#ifndef _SINGLETON_H_
#define _SINGLETON_H_
namespace wind
{
	template<typename T>
	class TSingleton
	{
	public:
		//多线程不安全
		static bool CreateInstance()
		{
			if (m_pInstance == nullptr)
			{
				m_pInstance = new(T);
			}
			return m_pInstance != nullptr;
		}

		static T &GetInstance()
		{
			return *Instance();
		}

		static T* Instance()
		{
			if (m_pInstance == nullptr)
			{
				CreateInstance();
			}
			return m_pInstance;
		}

		//需手动释放
		static void DestroyInstance()
		{
			if (m_pInstance != nullptr)
			{
				delete m_pInstance;
				m_pInstance = nullptr;
			}
		}

	private:
		//拷贝构造函数 和赋值操作符
		TSingleton(const TSingleton&) {}
		const TSingleton& operator=(const TSingleton&) {}

	protected:
		TSingleton(){}
		~TSingleton() {}

	private:
		static T* m_pInstance;
	};
	template<typename T>
	T* TSingleton<T>::m_pInstance = nullptr;
}
#endif // _SINGLETON_H_
