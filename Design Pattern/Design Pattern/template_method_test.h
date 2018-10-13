#pragma once
#include"template_method.h"

void TemplateMethodTest()
{
	IAbstractClass* poConcreteClass = new CConcreteClass1();
	poConcreteClass->Frame();
	delete poConcreteClass;

	poConcreteClass = new CConcreteClass2();
	poConcreteClass->Frame();
	delete poConcreteClass;
}