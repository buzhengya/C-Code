#include "string.h"

static int nCount = 0;

CString::CString()
{
	pnCount = new(size_t);
	*pnCount = 1;
	pcStr = nullptr;
	std::cout << "CString::CString()" << std::endl;
}

CString::CString(const char *)
{
	std::cout << "CString::CString(const char *)" << std::endl;
}

CString::CString(const CString & oStr, size_t n)
{
	std::cout << "CString::CString(const CString & oStr, size_t n)" << std::endl;
}

CString::CString(const CString & oStr)
{
	pnCount = oStr.pnCount;
	pcStr = oStr.pcStr;
	(*pnCount)++;
	std::cout << "CString::CString(const CString & oStr)" << std::endl;
}


CString& CString::operator=(const CString& oStr)
{
	(*pnCount)--;
	if(*pnCount == 0)
	{
		delete pnCount;
		delete pcStr;
		std::cout << "赋值函数中析构" << std::endl;
	}

	pnCount = oStr.pnCount;
	pcStr = oStr.pcStr;
	(*pnCount)++;
	std::cout << "CString& CString::operator=(const CString& oStr)" << std::endl;
	return *this;
}

CString& CString::operator=(const char *)
{
	std::cout << "CString CString::operator=(const char *)" << std::endl;
	return *this;
}

CString::~CString()
{
	if(--*pnCount==0)
	{
		delete pnCount;
		delete pcStr;
		std::cout << "析构函数中析构" << std::endl;
	}
	std::cout << "CString::~CString()" << std::endl;
}

int CString::Length()
{
	return nLen;
}