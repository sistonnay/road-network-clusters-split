#pragma once

#ifndef SIMPLENODE_H
#define SIMPLENODE_H

#ifdef DLL_FILE
#define EX_IM_PORT _declspec(dllexport) //导出类
#else
#define EX_IM_PORT _declspec(dllimport) //导入类
#endif

#include <vector>
#include <string>
using namespace std;

class EX_IM_PORT SimpleNode
{

public:

	SimpleNode(void);	//默认构造函数
	~SimpleNode(void);	//析构函数

	SimpleNode(vector<int> _nears);//自定义构造函数
	SimpleNode(const SimpleNode & other);//拷贝构造函数
	SimpleNode & operator = (const SimpleNode & other);//赋值函数

	const vector<int> & GetNears();//外部获取邻居点集合

public:

	int degree;//节点度--邻接节点数量
	bool visit;//节点遍历标记
	string group_id;//节点所在群ID

private:

	vector<int> nears;//邻居点集合

};

#endif