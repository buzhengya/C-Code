#include "cstring.h"
#include<string>
#include<iostream>
#include<fstream>

using namespace std;
int main()
{
	CString oStr;
	cin >> oStr;
	cout << oStr;
	std::string xx;
	cin >> xx;
	cout << xx;
	std::string s = "xx" + xx + "y" + "10";
	s.front();
	s.back();
	if(s=="")
	{ 
		std::cout << "empty" << std::endl;
	}
	const std::string s1 = "1";
	s = s1 + "1";
	if (s == "")
	{
		std::cout << "Yes" << std::endl;
	}
	const CString oStr1="1";
	CString ostr2 = oStr1 + "1";
	return 0;
}