#pragma once

#include <vector>
#include <string>
#include <algorithm>
using namespace std;

class BaseGroup
{

public:

	BaseGroup(void):edges_num(0),nodes_num(0){}	//构造函数,内联
	virtual ~BaseGroup(void){} //虚析构函数,内联

	BaseGroup(const BaseGroup & other);	//拷贝构造函数
	BaseGroup & operator = (const BaseGroup & other);//赋值函数
	
	vector<int> & GetNodes();//外部获取节点集合

	virtual void AddNode(int _node) = 0;//增加节点
	virtual void DelNode(int _node) = 0;//删除节点

public:

	int edges_num;//群内边总数
	int nodes_num;//群内节点数

protected:

	vector<int> nodes;//节点集合

};