#include <iostream>
#include <map>
#include "atu_mysql.h"

int main(int argc, void **argv)
{
	atu_libs::atu_mysql::atu_mysql mysql;
	atu_libs::atu_mysql::atu_mysql_result ret;
	int cnt;
	char sql[128] = "SELECT * FROM `user`";

	mysql.connect("localhost", "root", "p2pppt", "ad");
	mysql.excute(sql, ret);
	for (int i = 0; i < ret.size(); i++)
	{
		for (atu_libs::atu_mysql::atu_mysql_result::iterator it = ret.begin();
				it != ret.end(); it++)
		{
			atu_libs::atu_mysql::atu_mysql_rows &row = it->second;
			std::cout << it->first << ": " << row[i] << std::endl;
		}
		std::cout << std::endl;
	}

	mysql.close();
	return 0;
}
