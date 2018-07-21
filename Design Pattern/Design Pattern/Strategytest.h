#pragma once
#include"Strategy.h"

void test_strategy()
{
	Context context;

	context.ContextInterface1();
	context.ContextInterface2();

	context.SetStrategy1(new Strategy1A);
	context.SetStrategy2(new Strategy2B);

	context.ContextInterface1();
	context.ContextInterface2();

	context.SetStrategy1(new Strategy1B);
	context.SetStrategy2(new Strategy2A);

	context.ContextInterface1();
	context.ContextInterface2();
}