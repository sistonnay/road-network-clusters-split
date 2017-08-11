#pragma once

#ifndef GROUPWITHMAXEDGES_H
#define GROUPWITHMAXEDGES_H

#ifdef DLL_FILE
#define EX_IM_PORT _declspec(dllexport) //导出类
#else
#define EX_IM_PORT _declspec(dllimport) //导入类
#endif

#include "basegroup.h"

class EX_IM_PORT GroupWithMaxEdges : public BaseGroup
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

#endif