#include <windows.h>
#include <mysql.h>
#include <string.h>
#include <iostream>


using namespace std;

int main()
{
	MYSQL mysql;
	mysql_init(&mysql);
	mysql_real_connect(&mysql, "192.168.24.54", "root", "Ygame#123", "storm_dev_area_wf", 3306, nullptr, 0);
	string sql = "select * from role_base where id=\"77172804968169537\"";
	mysql_query(&mysql, sql.c_str());
	mysql_close(&mysql);
	return 0;
}