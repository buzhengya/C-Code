#include "tool.h"

bool IsNum(char szCh)
{
	return szCh >= '0' && szCh <= '9';
}

bool IsLowCase(char szCh)
{
	return szCh >= 'a' && szCh <= 'z';
}

bool IsAlpha(char szCh)
{
	return (szCh >= 'a' && szCh <= 'z') || (szCh >= 'A' && szCh <= 'Z');
}

bool IsNumOrAlpha(char szCh)
{
	return IsNum(szCh) || IsAlpha(szCh);
}

bool IsSpace(char szCh)
{
	return szCh == ' ' || szCh == '\r' || szCh == '\n' || szCh == '\t' || szCh == '\f' || szCh == '\v';
}
