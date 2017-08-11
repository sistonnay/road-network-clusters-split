#pragma once

#include <vector>
#include <string>
using namespace std;

class NodeCloseInfo //某临近群相接点信息
{

public:

	NodeCloseInfo(void); //默认构造函数
	~NodeCloseInfo(void);//析构函数

	NodeCloseInfo(const NodeCloseInfo & other);//拷贝构造函数
	NodeCloseInfo & operator = (const NodeCloseInfo & other);//赋值函数
	
	void AddCloseNode(int _node);//添加某临近群相接点
	const vector<int> & GetCloseNodes();//获取某临近群相接点集合

public:

	string group_id;//群ID
	int close_num;	//相接点总数

private:
	
	vector<int> close_nodes;//相接点集合

};