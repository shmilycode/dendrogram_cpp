#include "database.h"
#include "dendrogram.h"
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>
#include <time.h>
#include <regex>
using namespace std;

const string host = "120.27.110.28";
const string username = "root";
const string pwd = "weiqi123";
const string dbname = "chrome";
const string tablename = "user_data_student_test";
const int URL_COL = 1;
const int OPT_TYPE_COL = 2;
const int OPT_COL = 3;
const int TIME_COL = 4;
const int NEXTURL_COL = 5;
const int FiveMinute = 5 * 60;

bool statistics(dendrogram &linktree, const string& userip, mysql_connector &mysql)
{
	float leaf_count = linktree.leaf_count();
	float succeed_links = linktree.node_count("立即购买", "加入购物车");
	float failed_links = leaf_count - succeed_links;
	float sort_opt = linktree.get_sort_opt_count();
	float duration = linktree.get_duration();
	if (duration < FiveMinute)
		return true;
	std::cout << "ip: " << userip << std::endl;
	std::cout << leaf_count << std::endl;
	std::cout << succeed_links << std::endl;
	std::cout << sort_opt << std::endl;
	std::cout << duration/60 << std::endl;

	float times = duration / 60.0 /10.0;
	if (times < 1)
		times = 1;
	stringstream s_all_links, s_succeed_links, s_duration, s_sort_opt;
	s_all_links << leaf_count / times;
	s_succeed_links << succeed_links / times;
	s_duration << duration;
	s_sort_opt << sort_opt /times;
	string insert_query = "insert into stats (ip, all_links, succeed_links, duration, sort_opt) values (\"" + 
		userip + "\"," + s_all_links.str() + ',' + s_succeed_links.str() + ',' +
		s_duration.str() + ',' + s_sort_opt.str() + ')';
	mysql.query(insert_query);
	return true;
}

bool str2time_t(const string &Time, time_t &TStartTime)
{
	tm StartTime;
	const regex pattern("(\\d{4})-(\\d{2})-(\\d{2}) (\\d{1,2}):(\\d{1,2}):(\\d{1,2})");
	match_results<std::string::const_iterator> result;
	bool valid = regex_match(Time, result, pattern);
	if (valid)
	{
		string year = result[1];
		StartTime.tm_year = atoi(year.c_str()) - 1900;
		string month = result[2];
		StartTime.tm_mon = atoi(month.c_str());
		string day = result[3];
		StartTime.tm_mday = atoi(day.c_str());
		string hour = result[4];
		StartTime.tm_hour = atoi(hour.c_str());
		string minute = result[5];
		StartTime.tm_min = atoi(minute.c_str());
		string second = result[6];
		StartTime.tm_sec = atoi(second.c_str());
		tm *p_StartTime = &StartTime;
		TStartTime = mktime(p_StartTime);
		return true;
	}
	return false;
}

int main(int argc, char ** argv)
{
	mysql_connector mysql;
	if (!mysql.connect(host, username, pwd, dbname))
	{
		return 0;
	}
	MYSQL_RES *res = mysql.query_all_ip(tablename);
	if (res == NULL)
	{
		return 0;
	}
	vector<string> ip_vector;
	MYSQL_ROW m_row;
	while (m_row = mysql_fetch_row(res))
	{
		ip_vector.push_back(m_row[0]);
	}

	vector<string>::iterator ip_ite;
	for (ip_ite = ip_vector.begin(); ip_ite != ip_vector.end(); ip_ite++)
	{

		MYSQL_RES *res = mysql.query_by_ip(tablename, *ip_ite);
		if (res == NULL)
		{
			return 0;
		}
		dendrogram linktree;
		MYSQL_ROW m_row;
		while (m_row = mysql_fetch_row(res))
		{
			//将时间字符串转化为tm结构
			time_t StartTime = 0;
			string StrTime = m_row[TIME_COL];
			if (!str2time_t(StrTime, StartTime))
			{
				cerr << "Time transform error!" << std::endl;
				return 0;
			}
			linktree.add_node(m_row[URL_COL], m_row[OPT_TYPE_COL], m_row[OPT_COL], m_row[NEXTURL_COL], StartTime);
		}
		statistics(linktree, *ip_ite, mysql);
	}
	return 0;
}