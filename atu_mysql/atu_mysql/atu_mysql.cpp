/*
 * atu_mysql.cpp
 *
 *  Created on: Aug 4, 2012
 *      Author: PArt
 */

#include "atu_mysql.h"
#include <string>
#include <iostream>

namespace atu_libs{
namespace atu_mysql{
atu_mysql::atu_mysql()
: m_port(3306)
, m_encode(ENC_UTF8)
{
}

atu_mysql::~atu_mysql() {
	mysql_close(&m_mysql);
}

void atu_mysql::connect(const char *host,
		const char *user,
		const char *pswd,
		const char *db,
		uint32_t port,
		const char *encode)
{
	mysql_init(&m_mysql);

	m_host = host;
	m_port = port;
	m_db = db;
	m_user = user;
	m_pswd = pswd;
	m_encode = encode;

	mysql_real_connect(&m_mysql, host, user, pswd, db, port, NULL, 0);
	set_encode(encode);
}

void atu_mysql::close(void)
{
	mysql_close(&m_mysql);
}

void atu_mysql::change_db(const char *db)
{
	mysql_close(&m_mysql);
	mysql_init(&m_mysql);
	m_db = db;
	//TODO use select_db();
	mysql_real_connect(&m_mysql, m_host.c_str(), m_user.c_str(), m_pswd.c_str(),
			m_db.c_str(), m_port, NULL, 0);
	set_encode(m_encode.c_str());
}

void atu_mysql::set_encode(const char *encode)
{
	char sql[64] = "";
	sprintf(sql, "SET NAMES %s", encode);
	mysql_query(&m_mysql, sql);
	sprintf(sql, "SET CHARACTER SET %s", encode);
	mysql_query(&m_mysql, sql);
}

//For insert, delete, update... operations.
void atu_mysql::excute(char *sql)
{
	mysql_query(&m_mysql, sql);
}

//For select.
void atu_mysql::excute(char *sql, atu_mysql_result &val)
{
	MYSQL_RES *res = NULL;
	MYSQL_ROW row = NULL;
	MYSQL_FIELD *field = NULL;

	int ret(0);

	ret = mysql_query(&m_mysql, sql);
	res = mysql_store_result(&m_mysql);

	val.clear();

	if (0 != ret)
	{
		return;
	}
#ifdef _DEBUG
#endif
	int field_count = mysql_num_fields(res);

	while (NULL != 	(row = mysql_fetch_row(res)))
	{
		for (int i = 0; i < field_count; i++)
		{
			std::string field_name;

			field = mysql_fetch_field_direct(res, i);
			field_name = field->name;
			if (NULL != row[i])
			{
				val[field_name].push_back(row[i]);
			}
			else
			{
				val[field_name].push_back("null");
			}
		}
#ifdef _DEBUG
#endif
	}
	mysql_free_result(res);
}

void atu_mysql::update(char *table, char *key, char *val, char *mainkey, char *mainval)
{
	char sql[256] = "";
	sprintf(sql, "UPDATE %s SET %s = \'%s\' WHERE %s = \'%s\';",
			table,
			key,
			val,
			mainkey,
			mainval);
	excute(sql);
}

}
}
