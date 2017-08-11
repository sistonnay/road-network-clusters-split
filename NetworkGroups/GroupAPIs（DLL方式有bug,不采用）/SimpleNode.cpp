//
#ifndef DLL_FILE
#define DLL_FILE
#endif

#include "SimpleNode.h"

SimpleNode::SimpleNode(void){}

SimpleNode::~SimpleNode(void){}

SimpleNode::SimpleNode(vector<int> _nears)
{
	//id = _id;
	degree = _nears.size();
	visit = false;
	group_id = "";
	nears = _nears;
}

SimpleNode::SimpleNode(const SimpleNode & other)
{
	//id = other.id;
	degree = other.degree;
	visit = other.visit;
	group_id = other.group_id;
	nears = other.nears;
}

SimpleNode & SimpleNode::operator = (const SimpleNode & other)
{
	if(this!=&other)
	{	//id = other.id;
		degree = other.degree;
		visit = other.visit;
		group_id = other.group_id;
		nears = other.nears;
	}
	return *this;
}

const vector<int> & SimpleNode::GetNears()
{
	return nears;
}