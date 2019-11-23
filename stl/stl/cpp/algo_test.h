#pragma once
#include "algo.h"
#include "define.h"
#include "log.h"
#include "function.h"

#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

class CJudgeTest
{
	friend CJudgeTest operator+(const CJudgeTest& oLeft, const CJudgeTest& oRight)
	{
		CJudgeTest oTmp = oLeft;
		return oTmp += oRight;
	}

public:
	CJudgeTest() {};
	CJudgeTest(int32 nVal) : m_nVal(nVal) {};

	CJudgeTest& operator=(const CJudgeTest& oRight)
	{
		m_nVal = oRight.m_nVal;
		return *this;
	}

	bool operator<(const CJudgeTest& oRight) const
	{
		return m_nVal < oRight.m_nVal;
	}

	CJudgeTest& operator+=(const CJudgeTest& oRight)
	{
		m_nVal += oRight.m_nVal;
		return *this;
	}

	bool Val() { return m_nVal >= 0; }

	void Print() { std::cout << "JudgeTest Val : " << m_nVal << std::endl; }

private:
	int32 m_nVal = 0;
};

void UnaryTest()
{
	vector<CJudgeTest> vecJudge;
	vecJudge.emplace_back(2);
	vecJudge.emplace_back(0);
	vecJudge.emplace_back(-1);
	vecJudge.emplace_back(1);
	vecJudge.emplace_back(-2);

	for_each(vecJudge.begin(), vecJudge.end(), TJudge<CJudgeTest>());
	LOG_DEBUG << "---------------------- split ----------------------";
	for_each(vecJudge.begin(), vecJudge.end(), TPrint<CJudgeTest>());
	LOG_DEBUG << "---------------------- split ----------------------";
	for_each(vecJudge.begin(), vecJudge.end(), TUnaryNegate<TJudge<CJudgeTest>>(TJudge<CJudgeTest>()));
	LOG_DEBUG << "---------------------- split ----------------------";
	for_each(vecJudge.begin(), vecJudge.end(), UnaryNegate(TJudge<CJudgeTest>()));

	LOG_DEBUG << "---------------------- split ----------------------";
	sort(vecJudge.begin(), vecJudge.end(), TCompare<CJudgeTest>());

	LOG_DEBUG << "---------------------- split ----------------------";
	for_each(vecJudge.begin(), vecJudge.end(), TPrint<CJudgeTest>());

	LOG_DEBUG << "---------------------- split ----------------------";
	sort(vecJudge.begin(), vecJudge.end(), TBinaryNegate<TCompare<CJudgeTest>>(TCompare<CJudgeTest>()));

	LOG_DEBUG << "---------------------- split ----------------------";
	for_each(vecJudge.begin(), vecJudge.end(), TPrint<CJudgeTest>());

	LOG_DEBUG << "---------------------- split ----------------------";
	Accumulate(vecJudge.begin(), vecJudge.end(), TAdd<CJudgeTest>());

	LOG_DEBUG << "---------------------- split ----------------------";
	for_each(vecJudge.begin(), vecJudge.end(), TPrint<CJudgeTest>());

	LOG_DEBUG << "---------------------- split ----------------------";
	Increase(vecJudge.begin(), vecJudge.end(), Bind1st(TAdd<CJudgeTest>(), CJudgeTest(10)));

	LOG_DEBUG << "---------------------- split ----------------------";
	for_each(vecJudge.begin(), vecJudge.end(), TPrint<CJudgeTest>());
}
