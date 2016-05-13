#include <mysql.h>
#include <iostream>
using namespace std;

class mysql_connector{
public:
	mysql_connector();
	~mysql_connector();
	bool connect(const string &Host, const string &Username, const string &PWD, const string &Dbname);
	MYSQL_RES *query_by_ip(const string &TableName, const string &IP);
	MYSQL_RES *query_all(const string &TableName);
	MYSQL_RES *query_all_ip(const string &TableName);
	MYSQL_RES *query(const string &QueryStr);
private:
	void destroy_resource();
private:
	MYSQL MySQL;
	MYSQL_RES *QueryResult;
};