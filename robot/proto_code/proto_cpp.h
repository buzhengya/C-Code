#pragma once
#include "base_define.h"
#include "singleton.h"
#include <string>

using namespace std;

class CProtoCpp : public TSingleton<CProtoCpp>
{
	friend class TSingleton<CProtoCpp>;

public:

private:
	CProtoCpp() {}
	~CProtoCpp() {}

	string			m_strClassName;
	string			m_str	
};