#include "StdAfx.h"
#include "BaseGroup.h"

//BaseGroup::BaseGroup(void){}
//BaseGroup::~BaseGroup(void){}

BaseGroup::BaseGroup(const BaseGroup & other)
{
	nodes = other.nodes;
	edges_num = other.edges_num;
	nodes_num = other.nodes_num;
}

BaseGroup & BaseGroup::operator = (const BaseGroup & other)
{
	if(this!=&other)
	{
		nodes = other.nodes;
		edges_num = other.edges_num;
		nodes_num = other.nodes_num;
	}
	return *this;
}

vector<int> & BaseGroup::GetNodes()
{
	return nodes;
}