#pragma once
#include<iostream>

using namespace std;

class IAbstractClass
{
public:
	void Frame()
	{
		Algorithm1();
		Algorithm2();
		Hook();
	}
	virtual void Algorithm1() = 0;
	virtual void Algorithm2() = 0;
	virtual void Hook() {};
};

class CConcreteClass1:public IAbstractClass
{
public:
	void Algorithm1()
	{
		cout << "CConcreteClass1 algorithm1" << endl;
	}
	void Algorithm2()
	{
		cout << "CConcreteClass1 algorithm2" << endl;
	}
};

class CConcreteClass2 :public IAbstractClass
{
public:
	void Algorithm1()
	{
		cout << "CConcreteClass2 algorithm1" << endl;
	}
	void Algorithm2()
	{
		cout << "CConcreteClass2 algorithm2" << endl;
	}
	void Hook()
	{
		cout << "I have another method" << endl;
	}
};