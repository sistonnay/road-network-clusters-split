#include "StdAfx.h"
#include "ExtendNode.h"

ExtendNode::ExtendNode(void){}

ExtendNode::~ExtendNode(void){}

ExtendNode::ExtendNode(vector<int> _nears,int _hierachical)
{
	nears = _nears;
	checked = false;
	degree = _nears.size();
	hierachical = _hierachical;
	near_c_num = -1;
}

ExtendNode::ExtendNode(const ExtendNode & other)
{
	nears = other.nears;
	degree = other.degree;
	checked = other.checked;
	hierachical = other.hierachical;
	group_ids = other.group_ids;
	near_c_num = other.near_c_num;
}

ExtendNode & ExtendNode::operator = (const ExtendNode & other)
{
	if(this!=&other)
	{
		nears = other.nears;
		degree = other.degree;
		checked = other.checked;
		hierachical = other.hierachical;
		group_ids = other.group_ids;
		near_c_num = other.near_c_num;
	}
	return *this;
}