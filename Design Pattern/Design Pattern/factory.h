#pragma once
/*
特点：一个工厂对应一个对象 对增加子类扩展 对修改原有代码封闭 但是一但产品种类变多 将极大的增加类数量
*/
#include<iostream>

using namespace std;

class IProduct
{
public:
	virtual void Operation() = 0;
};

class ProductA :public IProduct
{
public:
	void Operation()
	{
		cout << "ProductA's Operation" << endl;
	}
};

class ProductB :public IProduct
{
public:
	void Operation()
	{
		cout << "ProductB's Operation" << endl;
	}
};

class IFactory
{
public:
	virtual IProduct* CreateProduct() = 0;
};

class FactoryA:public IFactory
{
public:
	IProduct* CreateProduct()
	{
		return new ProductA();
	}
};

class FactoryB:public IFactory
{
public:
	IProduct* CreateProduct()
	{
		return new ProductB();
	}
};

