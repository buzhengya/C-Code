#pragma once
#include"adapt.h"

void AdatptTest()
{
	CAdaptee oAdaptee;
	CTarget* oTarget = new CAdapter(&oAdaptee);
	oTarget->Request();
	delete oTarget;

	oTarget = new CTarget();
	oTarget->Request();
	delete oTarget;
}