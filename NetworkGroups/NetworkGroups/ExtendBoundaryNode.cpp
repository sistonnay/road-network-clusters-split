#include "StdAfx.h"
#include "ExtendBoundaryNode.h"

ExtendBoundaryNode::ExtendBoundaryNode(void)
{
	group_id = -1;
	un_in_dgree = 0;
	un_out_degree = 0;
	is_near = false;
}

ExtendBoundaryNode::~ExtendBoundaryNode(void){}

ExtendBoundaryNode::ExtendBoundaryNode(const ExtendBoundaryNode & other)//拷贝构造函数
{
	group_id = other.group_id;
	un_in_dgree = other.un_in_dgree;
	un_out_degree = other.un_out_degree;
	ref_nodes = other.ref_nodes;
	is_near = other.is_near;
}

ExtendBoundaryNode & ExtendBoundaryNode::operator = (const ExtendBoundaryNode & other)//赋值函数
{
	if(this!=&other)
	{
		group_id = other.group_id;
		un_in_dgree = other.un_in_dgree;
		un_out_degree = other.un_out_degree;
		ref_nodes = other.ref_nodes;
		is_near = other.is_near;
	}
	return *this;
}
