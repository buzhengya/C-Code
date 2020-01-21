#pragma once
#include "cstring.h"
#include <string>

const uint32 DEFAULT_TEST_CNT = 100;

void TestString(CString& oStr);
void TestStringEq(CString& oStr1, std::string oStr2);

void TestConstruct(uint32 nCnt = DEFAULT_TEST_CNT);

void TestOperator(uint32 nCnt = DEFAULT_TEST_CNT);
