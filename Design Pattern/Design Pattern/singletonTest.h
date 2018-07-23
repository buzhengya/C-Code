#pragma once
#include "singleton.h"
#include<iostream>

using namespace std;

void SingletonTest()
{
	Singleton *s1 = Singleton::getInstance();
	cout << s1->m_nValue << endl;

	//Singleton oS1 = *s1;//默认赋值函数产生了一个新对象
	//Singleton oS2(*s1);//默认复制函数产生了一个新对象
	////产生的新对象 指针指向的位置是同一个 但是其余的非静态成员都是自己的

	//oS1.m_nValue = 1;
	//oS2.m_nValue = 2;

	Singleton *s2 = Singleton::getInstance();
	s2->m_nValue = 3;
	//cout << s1->m_nValue << s2->m_nValue << oS1.m_nValue << oS2.m_nValue << endl;
	system("pause");
}


