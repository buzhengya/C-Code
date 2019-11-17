#include <iostream>
#include <vector>

using namespace std;

int main()
{
	vector<int>::iterator it;
	cout << typeid(it).name() << endl;
}