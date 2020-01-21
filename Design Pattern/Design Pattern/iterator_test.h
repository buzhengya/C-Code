#pragma once
#include "iterator.h"

#include<iostream>

void IteratorTest()
{
	IAggregate<int>* poAggr = new CConcreteAggregate<int>();
	IIterator<int>* poIt = poAggr->CreateIterator();

	poAggr->push_back(1);
	poAggr->push_back(3);
	poAggr->push_back(2);
	while (!poIt->IsDone())
	{
		cout << *(poIt->CurrentItem()) << endl;
		poIt->Next();
	}
}