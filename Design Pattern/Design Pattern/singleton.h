#pragma once
#include<iostream>
#include<vector>
#include<map>
#include<list>
#include<unordered_map>
#include<set>
#include<unordered_set>
class Singleton
{
public:
	int m_nValue;
	static Singleton* getInstance()
	{
		if (m_pInstance == 0)
		{
			m_pInstance = new Singleton();
		}
		return m_pInstance;
	}

private:
	static Singleton* m_pInstance;
	Singleton()
	{
		m_nValue = 0;
	}
	Singleton(const Singleton&) {}
	Singleton operator=(const Singleton&) {}
};