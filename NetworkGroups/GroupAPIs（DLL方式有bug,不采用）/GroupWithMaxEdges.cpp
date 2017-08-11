//
#ifndef DLL_FILE
#define DLL_FILE
#endif

#include "GroupWithMaxEdges.h"

//GroupWithMaxEdges::GroupWithMaxEdges(void){}
//GroupWithMaxEdges::~GroupWithMaxEdges(void){}

GroupWithMaxEdges::GroupWithMaxEdges(const GroupWithMaxEdges & other):BaseGroup(other)//拷贝构造函数
{
	max_edges_num = other.max_edges_num;
}

GroupWithMaxEdges & GroupWithMaxEdges::operator = (const GroupWithMaxEdges & other)	//赋值函数
{
	if(this!=&other)
	{
		BaseGroup::operator=(other);
		max_edges_num = other.max_edges_num;
	}
	return *this;
}

void GroupWithMaxEdges::AddNode(int _node)
{
	nodes.push_back(_node);
}

void GroupWithMaxEdges::DelNode(int _node)
{
	vector<int>::iterator iter;
	iter = find(nodes.begin(),nodes.end(),_node);
	if(iter!=nodes.end()) nodes.erase(iter);
}