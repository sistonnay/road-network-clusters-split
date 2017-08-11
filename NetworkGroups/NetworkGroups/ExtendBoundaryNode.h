#pragma once
#include <vector>
using namespace std;

class ExtendBoundaryNode
{

public:

	ExtendBoundaryNode(void);
	~ExtendBoundaryNode(void);
	ExtendBoundaryNode(const ExtendBoundaryNode & other);//拷贝构造函数
	ExtendBoundaryNode & operator = (const ExtendBoundaryNode & other);//赋值函数

public:

	int group_id;
	int un_in_dgree;
	int un_out_degree;
	bool is_near;
	vector<int> ref_nodes;

};

