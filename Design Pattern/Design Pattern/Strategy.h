#pragma once
#include<iostream>

using namespace std;

//Context�����ַ�ʽ���Ĳ��� һ�ǳ�ʼ��ʱ������� ����ר��дһ�����øò��Եĺ����ӿڡ��ȿ��������л����� �Ҳ��Խ϶�ʱ���Գ�ʼ����������̫�ࡣ
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
			cout << "��ָ��ΪNULL" << endl;
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
			cout << "��ָ��ΪNULL" << endl;
		}
	}
};