#pragma once
#include "Decorator.h"

int DecoratorTest()
{
	IBeverage* pBeverage1 = new HouseBlend();
	cout << pBeverage1->Name() << ": " << pBeverage1->Cost() << endl;

	IBeverage* pBeverage2 = new DarkRoast();
	pBeverage2 = new Cream(pBeverage2);
	pBeverage2 = new Mocha(pBeverage2);
	pBeverage2 = new Syrup(pBeverage2);
	cout << pBeverage2->Name() << ": " << pBeverage2->Cost() << endl;

	IBeverage* pBeverage3 = new Espresso();
	pBeverage3 = new Cream(pBeverage3);
	pBeverage3 = new Mocha(pBeverage3);
	pBeverage3 = new Syrup(pBeverage3);
	pBeverage3 = new Cream(pBeverage3);
	pBeverage3 = new Mocha(pBeverage3);
	pBeverage3 = new Syrup(pBeverage3);
	cout << pBeverage3->Name() << ": " << pBeverage3->Cost() << endl;
	return 0;
}