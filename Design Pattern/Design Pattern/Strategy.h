#pragma once
#include<iostream>

using namespace std;

//Context有两种方式更改策略 一是初始化时传入参数 二是专门写一个设置该策略的函数接口。既可以随意切换策略 且策略较多时可以初始化参数不会太多。
class Strategy1
{
public:
	virtual void Algorithm1Interface() = 0;
};

class Strategy1A:public Strategy1
{
	void Algorithm1Interface()
	{
		cout << "I am from Strategy1A." << endl;
	}
}; 

class Strategy1B :public Strategy1
{
	void Algorithm1Interface()
	{
		cout << "I am from Strategy1B." << endl;
	}
};

class Strategy2
{
public:
	virtual void Algorithm2Interface() = 0;
};

class Strategy2A :public Strategy2
{
	void Algorithm2Interface()
	{
		cout << "I am from Strategy2A." << endl;
	}
};

class Strategy2B :public Strategy2
{
	void Algorithm2Interface()
	{
		cout << "I am from Strategy2B." << endl;
	}
};

class Context
{
private:
	Strategy1 * pStrategy1;
	Strategy2 * pStrategy2;

public:
	Context()
	{
		pStrategy1 = NULL;
		pStrategy2 = NULL;
	}

	void SetStrategy1(Strategy1 * pStrategy)
	{
		pStrategy1 = pStrategy;
	}

	void SetStrategy2(Strategy2 * pStrategy)
	{
		pStrategy2 = pStrategy;
	}

	void ContextInterface1()
	{
		if(pStrategy1!=NULL)
		{
			pStrategy1->Algorithm1Interface();
		}
		else
		{
			cout << "类指针为NULL" << endl;
		}
	}

	void ContextInterface2()
	{
		if(pStrategy2!=NULL)
		{
			pStrategy2->Algorithm2Interface();
		}
		else
		{
			cout << "类指针为NULL" << endl;
		}
	}
};