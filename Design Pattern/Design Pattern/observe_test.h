#pragma once
#include "Observe.h"

int ObserveTest()
{
	IObserve *oObserverA = new ObserverA();
	IObserve *oObserverB = new ObserverB();
	IObserve *oObserverC = new ObserverC();

	ISubject oSubject;

	oSubject.SetValue(0);

	oSubject.Register(oObserverA);
	oSubject.Register(oObserverB);

	oSubject.SetValue(1);

	oSubject.UnRegister(oObserverA);
	oSubject.Register(oObserverC);

	oSubject.SetValue(2);
	return 0;
}