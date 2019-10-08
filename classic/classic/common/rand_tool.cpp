#pragma once
#include "rand_tool.h"
#include <random>

using namespace std;

static default_random_engine sRandom;

uint32 GetRand(uint32 nLeft, uint32 nRight)
{
	uniform_int_distribution<uint32> oUniform(nLeft, nRight);

	return oUniform(sRandom);
}

std::string GetRandStr(uint32 nLen)
{
	string strRet(0, nLen);
	uniform_int_distribution<uint32> oUniform(0, 255);

	for (int32 i = 0; i < nLen; ++i)
	{
		strRet[i] = oUniform(sRandom);
	}

	return strRet;
}
