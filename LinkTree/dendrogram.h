#include <iostream>
#include <map>
#include <vector>
using namespace std;

typedef struct node
{
	string CurUrl;
	vector<string> OptStr;
	string NextUrl;
	node *parent;
	vector<node*> children;
} *pNode;

class dendrogram
{
public:
	dendrogram();
	~dendrogram();
	pNode add_node(const string &CurUrl, const string &OptStr, const string &NextUrl);
	int	leaf_count();
	int node_count(const string &OptStr);
private:
	bool is_child_exist(pNode Parent, pNode Child);
private:
	map<string, pNode> Root;
	map<string, pNode> Nodes;
};
