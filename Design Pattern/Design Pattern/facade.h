#pragma once
#include<iostream>

using namespace std;

class CSystemA
{
public:
	void Operation()
	{
		cout << "SystemA Operation" << endl;
	}
};

class CSystemB
{
public:
	void Operation()
	{
		cout << "SystemB Operation" << endl;
	}
};

class CSystemC
{
public:
	void Operation()
	{
		cout << "SystemC Operation" << endl;
	}
};

class CFacade
{
private:
	CSystemA* m_poSystemA;
	CSystemB* m_poSystemB;
	CSystemC* m_poSystemC;
public:
	CFacade()
	{
		m_poSystemA = new CSystemA();
		m_poSystemB = new CSystemB();
		m_poSystemC = new CSystemC();
	}
	~CFacade()
	{
		delete m_poSystemA;
		delete m_poSystemB;
		delete m_poSystemC;
	}
	void Operation()
	{
		m_poSystemA->Operation();
		m_poSystemB->Operation();
		m_poSystemC->Operation();
	}
};