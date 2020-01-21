#pragma once
//抽象工厂模式用于这种情况 原料 + 工序 = 产品。 将原料和工序分离 分别定义一个接口。
#include<string>

using namespace std;

class IMaterial
{
public:
	virtual string Name() = 0;
};

class MaterialA :public IMaterial
{
public:
	string Name()
	{
		return "MaterialA";
	}
};

class MaterialB :public IMaterial
{
public:
	string Name()
	{
		return "MaterialB";
	}
};

class IProcess
{
public:
	virtual string Name() = 0;
};

class ProcessA :public IProcess
{
public:
	string Name()
	{
		return "ProcessA";
	}
};

class ProcessB :public IProcess
{
public:
	string Name()
	{
		return "ProcessB";
	}
};

class Product
{
private:
	IMaterial* m_pMaterial;
	IProcess* m_pProcess;
public:
	Product(IMaterial* pMaterial, IProcess* pProcess)
	{
		m_pMaterial = pMaterial;
		m_pProcess = pProcess;
	}
	string Name()
	{
		return m_pMaterial->Name() + " " + m_pProcess->Name();
	}
};

class IAbstractFactory
{
public:
	virtual IMaterial* CreateMaterial() = 0;
	virtual IProcess* CreateProcess() = 0;
	virtual Product* CreateProduct() = 0;
};

class AbstractFactory1 :public IAbstractFactory
{
	IMaterial* CreateMaterial()
	{
		return new MaterialA();
	}

	IProcess* CreateProcess()
	{
		return new ProcessA();
	}

	Product* CreateProduct()
	{
		return new Product(CreateMaterial(), CreateProcess());
	}
};

class AbstractFactory2 :public IAbstractFactory
{
	IMaterial* CreateMaterial()
	{
		return new MaterialA();
	}

	IProcess* CreateProcess()
	{
		return new ProcessB();
	}

	Product* CreateProduct()
	{
		return new Product(CreateMaterial(), CreateProcess());
	}
};

class AbstractFactory3 :public IAbstractFactory
{
	IMaterial* CreateMaterial()
	{
		return new MaterialB();
	}

	IProcess* CreateProcess()
	{
		return new ProcessA();
	}

	Product* CreateProduct()
	{
		return new Product(CreateMaterial(), CreateProcess());
	}
};

class AbstractFactory4 :public IAbstractFactory
{
	IMaterial* CreateMaterial()
	{
		return new MaterialB();
	}

	IProcess* CreateProcess()
	{
		return new ProcessB();
	}

	Product* CreateProduct()
	{
		return new Product(CreateMaterial(), CreateProcess());
	}
};

