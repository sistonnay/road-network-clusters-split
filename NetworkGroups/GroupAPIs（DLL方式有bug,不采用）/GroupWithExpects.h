#pragma once

#ifndef GROUPWITHEXPECTS_H
#define GROUPWITHEXPECTS_H

#ifdef DLL_FILE
#define EX_IM_PORT _declspec(dllexport) //导出类
#else
#define EX_IM_PORT _declspec(dllimport) //导入类
#endif

#include "basegroup.h"

class EX_IM_PORT GroupWithExpects : public BaseGroup
{

public:

	GroupWithExpects(void):expects_num(0){}//构造函数,内联
	~GroupWithExpects(void){}//析构函数,内联

	GroupWithExpects(const GroupWithExpects & other);//拷贝构造函数
	GroupWithExpects & operator = (const GroupWithExpects & other);//赋值函数

	void AddNode(int _node);//增加节点
	void DelNode(int _node);//删除节点

public:

	int expects_num;//群内对应随机图的边期望值的2m倍,m为网络总边数

};

#endif