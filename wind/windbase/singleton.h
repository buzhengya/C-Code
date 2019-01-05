#ifndef _SINGLETON_H_
#define _SINGLETON_H_
namespace wind
{
	template<typename T>
	class TSingleton
	{
	public:
		//���̲߳���ȫ
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

		//���ֶ��ͷ�
		static void DestroyInstance()
		{
			if (m_pInstance != nullptr)
			{
				delete m_pInstance;
				m_pInstance = nullptr;
			}
		}

	private:
		//�������캯�� �͸�ֵ������
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
