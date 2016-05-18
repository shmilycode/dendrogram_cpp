#include <iostream>
#include <map>
#include <vector>
#include <time.h>
using namespace std;

const string SORT_TYPE = "5";
const double TenMinute = 10 * 60;
typedef struct node
{
	string CurUrl;
	vector<string> OptStr;
	node *Parent;
	vector<node*> Children;
public:
	node();
	~node(){}
} *pNode;

class dendrogram
{
public:
	dendrogram();
	~dendrogram();
	pNode add_node(const string &CurUrl, const string &SortType, const string &OptStr, const string &NextUrl, const time_t &StartTime);
	int	leaf_count();
	int node_count(const string &OptStr1, const string& OptStr2 = string());
	int get_sort_opt_count();
	time_t get_duration();
private:
	bool is_child_exist(pNode Parent, pNode Child);
private:
	map<string, pNode> Nodes;
	int SortOptCount;
	time_t PreStartTime;
	time_t Duration;
};
