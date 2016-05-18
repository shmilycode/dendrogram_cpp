#include "dendrogram.h"
#include <iostream>
#include <string>
#include <iterator>
using namespace std;

node::node()
{
	Parent = NULL;
}
dendrogram::dendrogram()
{
	SortOptCount = 0;
	Duration = 0;
	PreStartTime = 0;
}
dendrogram::~dendrogram()
{
	//clean all memeory
	map<string, pNode>::iterator NodeIte;
	for (NodeIte = Nodes.begin(); NodeIte != Nodes.end(); NodeIte++)
	{
		delete(NodeIte->second);
	}
}

pNode dendrogram::add_node(const string &CurUrl, const string &SortType, const string &OptStr, const string &NextUrl, const time_t &StartTime)
{
	if (CurUrl.empty())
		return false;
	//用户分类操作的次数
	if (SortType == SORT_TYPE)
		SortOptCount++;
	pNode CurNode = Nodes[CurUrl];
	if (CurNode == NULL)
	{
		CurNode = Nodes[CurUrl] = new(std::nothrow) node();
		CurNode->CurUrl = CurUrl;
	}
	if (!OptStr.empty())
	{
		CurNode->OptStr.push_back(OptStr);
	}
	if (StartTime)
	{
		double DiffTime = difftime(StartTime, PreStartTime);
		//将大于10分钟的时间间隔舍弃
		if (DiffTime < TenMinute)
		{
			Duration += DiffTime;
		}
		PreStartTime = StartTime;
	}
	if (!NextUrl.empty())
	{
		pNode NextNode = add_node(NextUrl, string(), string(), string(), 0);
		if (NextNode)
		{
			//防止增加相同的子树
			//已经存在父节点的子树则不增加
			if (!is_child_exist(CurNode, NextNode) && !NextNode->Parent)
			{
				CurNode->Children.push_back(NextNode);
				NextNode->Parent = CurNode;
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
		if (NodeIte->second->Children.empty())
		{
			LeafCount++;
		}
	}
	return LeafCount;
}

int dendrogram::node_count(const string &OptStr1, const string& OptStr2)
{
	if (OptStr1.empty() && OptStr2.empty())
		return 0;
	map<string, pNode>::iterator NodeIte;
	int NodeCount = 0;
	for (NodeIte = Nodes.begin(); NodeIte != Nodes.end(); NodeIte++)
	{
		vector<string> OperatorString = NodeIte->second->OptStr;
		if (!OperatorString.empty())
		{
			for (vector<string>::iterator OptIte = OperatorString.begin(); OptIte != OperatorString.end(); OptIte++)
			{
				if (OptStr1.empty() && OptIte->find(OptStr2) != string::npos)
				{
					NodeCount++;
					break;
				}
				else
				if (OptStr2.empty() && OptIte->find(OptStr1) != string::npos)
				{
					NodeCount++;
					break;
				}
				else 
				if (!OptStr1.empty() && !OptStr2.empty())
				{
					if (OptIte->find(OptStr1) != string::npos || OptIte->find(OptStr2) != string::npos)
					{
						NodeCount++;
						break;
					}
						
				}
			}
		}
	}
	return NodeCount;
}

int dendrogram::get_sort_opt_count()
{
	return SortOptCount;
}

time_t dendrogram::get_duration()
{
	return Duration;
}

bool dendrogram::is_child_exist(pNode Parent, pNode Child)
{
	if (!Parent || !Child)
		return false;
	vector<pNode> Children = Parent->Children;
	if (Children.empty())
		return false;

	vector<pNode>::iterator ChildIte;
	for (ChildIte = Children.begin(); ChildIte != Children.end(); ChildIte++)
	{
		if (*ChildIte == Child)
			return true;
	}
	return false;
}