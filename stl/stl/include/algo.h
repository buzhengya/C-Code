#pragma once

template<class Iterator, class Func>
void Accumulate(Iterator it1, Iterator it2, Func func)
{
	Iterator itSecond = it1;
	Iterator itFirst = itSecond++;
	for (; itSecond < it2; ++itSecond, ++itFirst)
	{
		*itSecond = func(*itFirst, *itSecond);
	}
}

template<class Iterator, class Func>
void Increase(Iterator it1, Iterator it2, Func func)
{
	for (; it1 != it2; ++it1)
	{
		*it1 = func(*it1);
	}
}
