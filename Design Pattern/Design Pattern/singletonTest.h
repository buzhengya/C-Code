#pragma once
#include "singleton.h"
#include<iostream>

using namespace std;

void SingletonTest()
{
	Singleton *s1 = Singleton::getInstance();
	cout << s1->m_nValue << endl;

	//Singleton oS1 = *s1;//Ĭ�ϸ�ֵ����������һ���¶���
	//Singleton oS2(*s1);//Ĭ�ϸ��ƺ���������һ���¶���
	////�������¶��� ָ��ָ���λ����ͬһ�� ��������ķǾ�̬��Ա�����Լ���

	//oS1.m_nValue = 1;
	//oS2.m_nValue = 2;

	Singleton *s2 = Singleton::getInstance();
	s2->m_nValue = 3;
	//cout << s1->m_nValue << s2->m_nValue << oS1.m_nValue << oS2.m_nValue << endl;
	system("pause");
}


