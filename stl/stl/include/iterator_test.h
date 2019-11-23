#pragma once
#include "log.h"

template<class T>
void _PrintTypeStl(T)
{
	LOG_DEBUG << "unknown type!";
}

void _PrintTypeStl(int&)
{
	LOG_DEBUG << "int& type!";
}

void _PrintTypeStl(int*)
{
	LOG_DEBUG << "int* type!";
}

void _PrintTypeStl(const int&)
{
	LOG_DEBUG << "const int& type!";
}

void _PrintTypeStl(const int*)
{
	LOG_DEBUG << "const int* type!";
}

void _PrintTypeStl(std::input_iterator_tag)
{
	LOG_DEBUG << "input_iterator_tag type!";
}

void _PrintTypeStl(std::output_iterator_tag)
{
	LOG_DEBUG << "output_iterator_tag type!";
}

void _PrintTypeStl(std::forward_iterator_tag)
{
	LOG_DEBUG << "forward_iterator_tag type!";
}

void _PrintTypeStl(std::bidirectional_iterator_tag)
{
	LOG_DEBUG << "bidirectional_iterator_tag type!";
}

void _PrintTypeStl(std::random_access_iterator_tag)
{
	LOG_DEBUG << "random_access_iterator_tag type!";
}
template<class I>
void PrintTypeStl()
{
	LOG_DEBUG << "------------- val type --------------";
	typename I::value_type val = 0;
	_PrintTypeStl(val);

	LOG_DEBUG << "------------- difference type --------------";
	typename I::difference_type diff = 0;
	_PrintTypeStl(diff);

	LOG_DEBUG << "------------- pointer type --------------";
	typename I::pointer p = 0;
	_PrintTypeStl(p);

	LOG_DEBUG << "------------- reference type --------------";
	typename I::reference r = val;
	_PrintTypeStl(r);

	LOG_DEBUG << "------------- category type --------------";
	_PrintTypeStl(typename I::iterator_category());
}
