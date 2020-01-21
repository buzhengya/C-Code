#pragma once
#include<iostream>
#include<list>

using namespace std;


class IObserve
{
public:
	virtual void Update(int m) = 0;

	virtual void Display() = 0;

	//	IObserve(ISubject *pISubject)
	//	{
	//		p_ISubject = pISubject;
	//	}
	//
	//private:
	//	ISubject* p_ISubject;
};

class ISubject
{
public:
	virtual bool Register(IObserve* pObserver)
	{
		if (pObserver == NULL) return false;
		list<IObserve*>::iterator itObserver = m_listObserver.begin();
		for (; itObserver != m_listObserver.end(); itObserver++)
		{
			if (pObserver == (*itObserver))
			{
				return false;
			}
		}
		m_listObserver.push_back(pObserver);
		return true;
	}

	virtual bool UnRegister(IObserve* pObserver)
	{
		if (pObserver == NULL) return false;
		list<IObserve*>::iterator itObserver = m_listObserver.begin();
		for (; itObserver != m_listObserver.end(); itObserver++)
		{
			if (pObserver == (*itObserver))
			{
				m_listObserver.remove(*itObserver);
				return true;
			}
		}
		return false;
	}

	virtual void Notify()
	{
		list<IObserve*>::iterator itObserver = m_listObserver.begin();
		for (; itObserver != m_listObserver.end(); itObserver++)
		{
			(*itObserver)->Update(m_nState);
		}
	}

	virtual void Changed()
	{
		Notify();
	}

	virtual void SetValue(int m)
	{
		m_nState = m;
		Changed();
	}
private:
	list<IObserve*> m_listObserver;
	int m_nState;
};

class ConcreteSubject:public ISubject
{

};

class ObserverA :public IObserve
{
public:
	void Update(int m)
	{
		m_nState = m;
		Display();
	}

	void Display()
	{
		cout << "ObserverA值为：" << m_nState << endl;
	}

	ObserverA()
	{
		m_nState = 0;
	}
private:
	int m_nState;
};

class ObserverB :public IObserve
{
public:
	void Update(int m)
	{
		m_nState = m;
		Display();
	}

	void Display()
	{
		cout << "ObserverB值为：" << m_nState << endl;
	}

	ObserverB()
	{
		m_nState = 0;
	}
private:
	int m_nState;
};

class ObserverC :public IObserve
{
public:
	void Update(int m)
	{
		m_nState = m;
		Display();
	}

	void Display()
	{
		cout << "ObserverC值为：" << m_nState << endl;
	}

private:
	int m_nState;
};