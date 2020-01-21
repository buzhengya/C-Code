#include "rbtree/rb_tree_test.h"
#include "mem_pool/pool_test.h"
#include "event/event_test.h"
#include "log/log.h"

void Test()
{
	LOG_ERROR << "test" << 111;
}

int main()
{
	Test();
	const char* x = "xyz\194";
	std::cout << sizeof(x) << " " << x << " " << strlen(x);
	//TestRBTree(1000);
	TestEventTimer(10000);
	return 0;
}