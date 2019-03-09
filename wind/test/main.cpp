#include "mysql.h"
#include <string>
#include "mysql_test.h"
#include "logger.h"

using namespace std;

int main()
{
	CMysqlTest::Instance()->Test();

	return 0;
}