#pragma once
#include "abstract_factory.h"
#include<iostream>


void AbstractFactoryTest()
{
	IAbstractFactory* factory = new AbstractFactory1();

	cout << factory->CreateProduct()->Name() << endl;
	delete factory;

	factory = new AbstractFactory2();

	cout << factory->CreateProduct()->Name() << endl;
	delete factory;

	factory = new AbstractFactory3();

	cout << factory->CreateProduct()->Name() << endl;
	delete factory;

	factory = new AbstractFactory4();

	cout << factory->CreateProduct()->Name() << endl;
	delete factory;
}