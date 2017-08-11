#include "StdAfx.h"
#include "NodeCloseInfo.h"

NodeCloseInfo::NodeCloseInfo(void)
{
	group_id = "";
	close_num = 0;
}

NodeCloseInfo::~NodeCloseInfo(void){}

NodeCloseInfo::NodeCloseInfo(const NodeCloseInfo & other)
{
	group_id = other.group_id;
	close_num = other.close_num;	
	close_nodes = other.close_nodes;
}

NodeCloseInfo & NodeCloseInfo::operator = (const NodeCloseInfo & other)
{
	if(this!=&other)
	{
		group_id = other.group_id;
		close_num = other.close_num;	
		close_nodes = other.close_nodes;
	}
	return *this;
}

void NodeCloseInfo::AddCloseNode(int _node)
{
	close_nodes.push_back(_node);
}

const vector<int> & NodeCloseInfo::GetCloseNodes()
{
	return close_nodes;
}