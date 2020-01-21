#include "algo_test.h"
#include <iostream>
#include <vector>
#include "iterator.h"
#include "iterator_test.h"

using namespace std;

struct Iterator
{
	typedef int value_type;
};

int main()
{
	UnaryTest();
	//vector<int>::iterator it;
	//cout << typeid(it).name() << endl;

	//Iterator::value_type val = 0;
	//_PrintType(val);
	//vector<int> vecTest;
	//vecTest.push_back(1);
	//vector<int>::iterator it = vecTest.begin();
	//PrintTypeStl<vector<int>::iterator>();
}