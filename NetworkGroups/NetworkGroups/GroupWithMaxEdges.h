#pragma once

#include "basegroup.h"

class GroupWithMaxEdges : public BaseGroup
{

public:

	GroupWithMaxEdges(void):max_edges_num(0){}//构造函数,内联
	~GroupWithMaxEdges(void){};//析构函数,内联

	GroupWithMaxEdges(const GroupWithMaxEdges & other);//拷贝构造函数
	GroupWithMaxEdges & operator = (const GroupWithMaxEdges & other);//赋值函数

	void AddNode(int _node);//增加节点
	void DelNode(int _node);//删除节点

public:

	int max_edges_num;//群内最多可能出现边数量

};