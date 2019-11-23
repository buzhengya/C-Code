#pragma once
#include "algo.h"

#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

class CJudgeTest
{
public:
	CJudgeTest(int nVal) : m_nVal(nVal) {};

	bool Val() { return m_nVal >= 0; }

	void Print() { std::cout << "JudgeTest Val : " << m_nVal << std::endl; }

private:
	int m_nVal = 0;
};

void UnaryTest()
{
	vector<CJudgeTest> vecJudge;
	vecJudge.emplace_back(2);
	vecJudge.emplace_back(1);
	vecJudge.emplace_back(0);
	vecJudge.emplace_back(-1);
	vecJudge.emplace_back(-2);

	for_each(vecJudge.begin(), vecJudge.end(), TJudge<CJudgeTest>());
	for_each(vecJudge.begin(), vecJudge.end(), TPrint<CJudgeTest>());
}
