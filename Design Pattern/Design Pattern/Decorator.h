#pragma once
#include<string>
#include<iostream>

using namespace std;

class IBeverage
{
public:
	virtual string Name() = 0;
	virtual double Cost() = 0;
};

class HouseBlend :public IBeverage
{
public:
	string Name()
	{
		return "HouseBlend";
	}

	double Cost()
	{
		return 10;
	}
};

class DarkRoast :public IBeverage
{
public:
	string Name()
	{
		return "DarkRoast";
	}

	double Cost()
	{
		return 15;
	}
};

class Espresso :public IBeverage
{
public:
	string Name()
	{
		return "Espresso";
	}

	double Cost()
	{
		return 20;
	}
};

class CondimentDecorator :public IBeverage
{
protected:
	IBeverage* m_pBeverage;

public:
	CondimentDecorator(IBeverage* pBeverage) :m_pBeverage(pBeverage) {}

	virtual string Name() = 0;
	virtual double Cost() = 0;
	/*string Name()
	{
		return m_pBeverage->Name();
	}

	double Cost()
	{
		return m_pBeverage->Cost();
	}*/
};

class Cream :public CondimentDecorator
{
public:
	Cream(IBeverage* pBeverage) :CondimentDecorator(pBeverage) {}
	string Name()
	{
		return m_pBeverage->Name() + " Cream";
	}
	double Cost()
	{
		return m_pBeverage->Cost() + 0.2;
	}
};

class Mocha :public CondimentDecorator
{
public:
	Mocha(IBeverage* pBeverage) :CondimentDecorator(pBeverage) {}
	string Name()
	{
		return m_pBeverage->Name() + " Mocha";
	}
	double Cost()
	{
		return m_pBeverage->Cost() + 0.3;
	}
};

class Syrup :public CondimentDecorator
{
public:
	Syrup(IBeverage* pBeverage) :CondimentDecorator(pBeverage) {}
	string Name()
	{
		return m_pBeverage->Name() + " Syrup";
	}
	double Cost()
	{
		return m_pBeverage->Cost() + 0.5;
	}
};