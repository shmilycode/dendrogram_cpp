#include "database.h"
#include <iostream>
#include <string>

using namespace std;

mysql_connector::mysql_connector()
{
	if (mysql_init(&MySQL) == NULL)
		cerr << "Error: Mysql initial faild!" << std::endl;
}

mysql_connector::~mysql_connector()
{
	destroy_resource();
}

void mysql_connector::destroy_resource()
{
	mysql_free_result(QueryResult);
	mysql_close(&MySQL);
}

bool mysql_connector::connect(const string &host, const string &username, const string &pwd, const string &dbname)
{
	if (!mysql_real_connect(&MySQL, host.c_str(), username.c_str(), pwd.c_str(), dbname.c_str(), 3306, NULL, 0))
	{
		cerr << "Error connect to database: " << mysql_error(&MySQL) << endl;
		return false;
	}
	else
	{
		cout << "Connected ..." << endl;
		return true;
	}
}

MYSQL_RES *mysql_connector::query(const string &QueryStr)
{
	mysql_query(&MySQL, "set names gbk"); //ÉèÖÃ±àÂë¸ñÊ½
	
	if (mysql_query(&MySQL, QueryStr.c_str()))
	{
		cerr << "Query false: " << mysql_error(&MySQL) << endl;
		return NULL;
	}
	else
	{
		cout << "Query completed..." << endl;
	}

	if (!(QueryResult = mysql_store_result(&MySQL)))
	{
		cerr << "Couldn't get result: " << mysql_error(&MySQL) << endl;
		return NULL;
	}
	else
	{
		return QueryResult;
	}
}

MYSQL_RES *mysql_connector::query_by_ip(const string &table_name, const string &ip)
{
	string QueryStr = "select * from " + table_name;
	QueryStr += " where userIp=\"" + ip + "\"";

	return query(QueryStr);
}

MYSQL_RES *mysql_connector::query_all(const string &table_name)
{
	string QueryStr = "select * from " + table_name;
	return query(QueryStr);
}

MYSQL_RES *mysql_connector::query_all_ip(const string &table_name)
{
	string QueryStr = "select userIp from " + table_name;
	QueryStr += " group by userIp order by userIp";
	return query(QueryStr);
}
