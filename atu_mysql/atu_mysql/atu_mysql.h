/*
 * atu_mysql.h
 *
 *  Created on: Aug 4, 2012
 *      Author: PArt
 */

#ifndef _ATU_MYSQL_H_
#define _ATU_MYSQL_H_

extern "C"
{
#include <mysql/mysql.h>
}
#include <stdio.h>
#include <vector>
#include <string>
#include <map>

namespace atu_libs{

namespace atu_mysql{

#define 		ENC_UTF8		"utf8"
#define			ENC_GBK			"gbk"
#define			ENC_GB2312		"gb2312"
#define 		ENC_ASCII		"ascii"
#define			ENC_BIG5		"big5"
#define			ENC_BIN			"binary"
#define			ENC_UCS2		"ucs2"

typedef std::vector<std::string> atu_mysql_rows;
typedef std::map<std::string, atu_mysql_rows> atu_mysql_result;		//row name, result

class atu_mysql {
public:
	atu_mysql();
	virtual ~atu_mysql();

	void connect(const char *host,
			const char *user,
			const char *pswd,
			const char *db,
			uint32_t port = 3306,
			const char *encode = ENC_UTF8);
	void close(void);
	void set_encode(const char *encode);
	void change_db(const char *db);
	void excute(char *sql);
	void excute(char *sql, atu_mysql_result &val);
	void update(char *table, char *key, char *val, char *mainkey, char *mainval);
private:
	std::string m_host;
	uint32_t m_port;
	std::string m_db;
	std::string m_table;
	std::string m_user;
	std::string m_pswd;
	std::string m_encode;

	MYSQL m_mysql;
};

}
}

#endif /* _ATU_MYSQL_H_ */
