#pragma once
#include "factory.h"

void FactoryTest()
{
	IFactory* p_oFactory = new FactoryA();
	IProduct* p_oProduct = p_oFactory->CreateProduct();

	p_oProduct->Operation();

	delete p_oFactory;
	delete p_oProduct;

	p_oFactory = new FactoryB();
	p_oProduct = p_oFactory->CreateProduct();

	p_oProduct->Operation();
}