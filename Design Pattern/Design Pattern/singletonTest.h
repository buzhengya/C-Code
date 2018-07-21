#pragma once
#include "singleton.h"
#include<iostream>

using namespace std;


int test()
{

	Singleton *s1 = Singleton::getInstance();
	cout << s1->value;

	Singleton *s2 = Singleton::getInstance();
	cout << s1->value << s2->value;

	s2->value = 2;
	cout << s1->value << s2->value;
	system("pause");
	return 0;
}