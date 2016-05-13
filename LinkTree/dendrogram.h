#include <iostream>
#include <map>
#include <vector>
using namespace std;

const string SORT_TYPE = "5";
typedef struct node
{
	string CurUrl;
	vector<string> OptStr;
	string NextUrl;
	node *parent;
	vector<node*> children;
	node();
} *pNode;

class dendrogram
{
public:
	dendrogram();
	~dendrogram();
	pNode add_node(const string &CurUrl, const string &SortType, const string &OptStr, const string &NextUrl);
	int	leaf_count();
	int node_count(const string &OptStr1, const string& OptStr2 = string());
	int get_sort_opt_count();
private:
	bool is_child_exist(pNode Parent, pNode Child);
private:
	map<string, pNode> Nodes;
	int SortOptCount;
};
