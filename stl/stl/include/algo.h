#pragma once
#include <functional>
#include <iostream>

template<class T>
class TJudge : public std::unary_function<T, bool>
{
public:
	TJudge() {}

	bool operator()(T t)
	{
		bool bRet = t.Val();
		std::cout << "TJudge::operator() val : " << bRet << std::endl;
		return bRet;
	}
};

template<class T>
class TPrint : public std::unary_function<T, void>
{
public:
	TPrint() {}

	void operator()(T t)
	{
		t.Print();
	}
};
