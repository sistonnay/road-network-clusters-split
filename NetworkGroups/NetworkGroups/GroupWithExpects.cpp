#include "StdAfx.h"
#include "GroupWithExpects.h"

//GroupWithExpects::GroupWithExpects(void){}
//GroupWithExpects::~GroupWithExpects(void){}

GroupWithExpects::GroupWithExpects(const GroupWithExpects & other):BaseGroup(other)//拷贝构造函数
{
	expects_num = other.expects_num;
}

GroupWithExpects & GroupWithExpects::operator = (const GroupWithExpects & other)	//赋值函数
{
	if(this!=&other)
	{
		BaseGroup::operator=(other);
		expects_num = other.expects_num;
	}
	return *this;
}

void GroupWithExpects::AddNode(int _node)
{
	nodes.push_back(_node);
}

void GroupWithExpects::DelNode(int _node)
{
	vector<int>::iterator iter;
	iter = find(nodes.begin(),nodes.end(),_node);
	if(iter!=nodes.end()) nodes.erase(iter);
}