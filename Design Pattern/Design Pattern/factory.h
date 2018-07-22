#pragma once
/*
�ص㣺һ��������Ӧһ������ ������������չ ���޸�ԭ�д����� ����һ����Ʒ������ �����������������
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

