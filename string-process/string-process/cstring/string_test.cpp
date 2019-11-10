#include "string_test.h"
#include "../log/log.h"
#include "../common/mem_pool.h"
#include "../common/rand_tool.h"

using namespace std;
const uint32 MAX_STR_LEN = 4095;
const uint32 MIN_STR_LEN = 0;

void TestString(CString& oStr)
{
	const char* pData = oStr.Data();
	
	// 1.测试长度有效 2.测试容量有效 通过下标进行访问
	if (oStr.Capacity() > 0)
	{
		char szC = pData[oStr.Capacity()];
	}

	if (oStr.Size() > 0)
	{
		ASSERT(pData[oStr.Size()] == '\0');
	}
}

void TestStringEq(CString& oStr1, std::string oStr2)
{
	TestString(oStr1);
	ASSERT(oStr1.Size() == oStr2.size());

	const char* pData1 = oStr1.Data();
	const char* pData2 = oStr2.c_str();
	for (uint32 i = 0, j = oStr1.Size(); i < j; ++i)
	{
		ASSERT(pData1[i] == pData2[i]);
	}
}

void TestConstruct(uint32 nCnt)
{
	LOG_INFO << "================ begin test construct. test case count : " << nCnt << " ==================";

	TestStringEq(CString(), string());

	for (uint32 i = 0; i < nCnt; ++i)
	{
		uint32 nTotal = GetRand(MIN_STR_LEN, MAX_STR_LEN);
		string oStr1 = GetRandStr(nTotal);
		CString oStr2 = CString(oStr1.data(), nTotal);
		// construct 1
		TestStringEq(oStr2, oStr1);

		// construct 2
		uint32 nSize = GetRand(0, nTotal);
		TestStringEq(CString(oStr1.data(), nSize), string(oStr1.data(), nSize));

		// construct 3
		TestStringEq(CString(oStr2), oStr1);

		// construct 4
		uint32 nPos = GetRand(0, nTotal);
		uint32 nLen = GetRand(0, nTotal - nPos);
		TestStringEq(CString(oStr2, nPos, nLen), string(oStr1, nPos, nLen));

		// construct 5
		char szC = GetRandChar();
		TestStringEq(CString(nTotal, szC), string(nTotal, szC));

		CString oOp2;
		string  oOp1;
		// operator= 1
		oOp2 = oStr2;
		oOp1 = oStr1;
		TestStringEq(oOp2, oOp1);

		// operator= 2
		oOp2 = oStr2.Data();
		oOp1 = oStr1.data();
		TestStringEq(oOp2, oOp1);

		// operator= 3 && 右值引用
		oOp2 = CString(oStr2);
		oOp1 = string(oStr1);
		TestStringEq(oOp2, oOp1);

		// operator= 4
		oOp2 = szC;
		oOp1 = szC;
		TestStringEq(oOp2, oOp1);

		string  oAgn1;
		CString oAgn2;
		// assign 1 CString
		oAgn1.assign(oStr1);
		oAgn2.Assign(oStr2);
		TestStringEq(oAgn2, oAgn1);

		// assign 2 CString nPos nLen
		oAgn1.assign(oStr1, nPos, nLen);
		oAgn2.Assign(oStr2, nPos, nLen);
		TestStringEq(oAgn2, oAgn1);

		// assign 3 char*
		oAgn1.assign(oStr1.data());
		oAgn2.Assign(oStr2.Data());
		TestStringEq(oAgn2, oAgn1);

		// assign 4 char* nSize
		oAgn1.assign(oStr1.data(), nSize);
		oAgn2.Assign(oStr2.Data(), nSize);
		TestStringEq(oAgn2, oAgn1);

		// assign 5 CString&&
		oAgn1.assign(string(oStr1.data(), nSize));
		oAgn2.Assign(CString(oStr2.Data(), nSize));
		TestStringEq(oAgn2, oAgn1);
	}

	POOL_MGR.DebugInfo();
	if (POOL_MGR.IsAllFree() == false)
	{
		ASSERT(false);
	}
}
