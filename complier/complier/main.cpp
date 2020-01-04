#include "re_nfa.h"

int main()
{
	CRe2NFA oRe2NFA;
	oRe2NFA.Init("", "(a|b)*c");
	oRe2NFA.Re2NFA();

	oRe2NFA.Init("", "(a~d|b)*c");
	oRe2NFA.Re2NFA();
	return 0;
}
