#pragma once
#include<iostream>

using namespace std;

class CTarget
{
public:
	virtual void Request()
	{
		cout << "Request" << endl;
	}
};

class CAdaptee
{
public:
	void SpecialRequest()
	{
		cout << "SpecialRequest" << endl;
	}
};

class CAdapter:public CTarget
{
private:
	CAdaptee* m_poAdaptee;

public:
	CAdapter(CAdaptee* poAdaptee)
	{
		m_poAdaptee = poAdaptee;
	}
	void Request()
	{
		m_poAdaptee->SpecialRequest();
	}
};