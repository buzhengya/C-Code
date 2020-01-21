#ifndef _GNBASE_H_
#define _GNBASE_H
#include "base_define.h"
struct SVersion
{
uint16 wMajorVersion;      
uint16 wMinorVersion;      
uint16 wCompatibleVersion;  
uint16 wBuildNumber;        
};

const SVersion VERSION = { 3, 2, 0, 0 };

class IBase
{
public:
	virtual WAPI ~IBase(void) {}

	virtual void WAPI AddRef(void) = 0;

	virtual uint32  WAPI QueryRef(void) = 0;

	virtual void WAPI Release(void) = 0;

	virtual SVersion WAPI GetVersion(void) = 0;

	virtual const char * WAPI GetModuleName(void) = 0;
};
#endif // _GNBASE_H_
