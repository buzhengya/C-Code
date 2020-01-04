#pragma once
template<class T>
class TSingleton
{
public:
	static T* Instance()
	{
		CreateInstance();
		return m_pInstance;
	}

	static T& GetInstance()
	{
		CreateInstance();
		return *m_pInstance;
	}

	static void CreateInstance()
	{
		if (m_pInstance == nullptr)
		{
			m_pInstance = new T();
		}
	}

	static void Release()
	{
		if (m_pInstance != nullptr)
		{
			delete m_pInstance;
		}
	}

protected:
	TSingleton() {}
	~TSingleton() {}

private:
	TSingleton(const TSingleton&) {}
	TSingleton& operator=(TSingleton& t) {}

	static T* m_pInstance;
};

template<class T>
T* TSingleton<T>::m_pInstance = nullptr;
