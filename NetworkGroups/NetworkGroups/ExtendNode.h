#pragma once

#include <vector>
#include <string>
using namespace std;

class ExtendNode
{

public:

	ExtendNode(void);	//默认构造函数
	~ExtendNode(void);	//析构函数

	ExtendNode(vector<int> _nears,int _hierachical);//自定义构造函数
	ExtendNode(const ExtendNode & other);//拷贝构造函数
	ExtendNode & operator = (const ExtendNode & other);//赋值函数

public:

	int degree;//节点度--邻接节点数量
	int hierachical;//节点层次
	int near_c_num;//初始邻接社团数
	bool checked;//是否标记过
	vector<int> nears;//邻居点集合
	vector<int> group_ids;//节点社团

};