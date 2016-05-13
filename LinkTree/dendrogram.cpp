#include "dendrogram.h"
#include <iostream>
#include <string>
#include <iterator>
using namespace std;


dendrogram::dendrogram()
{
	Root[""] = new(std::nothrow) node();
}
dendrogram::~dendrogram()
{
	//clear of memeory
}

pNode dendrogram::add_node(const string &CurUrl, const string &OptStr, const string &NextUrl)
{
	if (CurUrl.empty())
		return false;
	pNode CurNode = Nodes[CurUrl];
	if (CurNode == NULL)
	{
		CurNode = nodes[CurUrl] = new(std::nothrow) node();
		CurNode->CurUrl = CurUrl;
	}
	if (!OptStr.empty())
	{
		CurNode->OptStr.push_back(OptStr);
	}
	if (!NextUrl.empty())
	{
		pNode NextNode = add_node(NextUrl, string(), string());
		if (NextNode)
		{
			//防止增加相同的子树
			//已经存在父节点的子树则不增加
			if (!is_child_exist(CurNode, NextNode) && !NextNode->parent)
			{
				CurNode->children.push_back(NextNode);
				NextNode->parent = CurNode;
			}
		}
		
	}
	return CurNode;
}

int dendrogram::leaf_count()
{
	map<string, pNode>::iterator NodeIte;
	int LeafCount = 0;
	for (NodeIte = Nodes.begin(); NodeIte != Nodes.end(); NodeIte++)
	{
		if (NodeIte->second->children.empty())
		{
			LeafCount++;
		}
	}
	return LeafCount;
}

int dendrogram::node_count(const string &OptStr)
{
	map<string, pNode>::iterator NodeIte;
	int NodeCount = 0;
	for (NodeIte = Nodes.begin(); NodeIte != Nodes.end(); NodeIte++)
	{
		vector<string> OperatorString = NodeIte->second->OptStr;
		if (!OperatorString.empty())
		{
			for (vector<string>::iterator OptIte = OperatorString.begin(); OptIte != OperatorString.end(); OptIte++)
			{
				if (OptIte->find(OptStr) != string::npos)
				{
					NodeCount++;
					break;
				}
			}
		}
	}
	return NodeCount;
}