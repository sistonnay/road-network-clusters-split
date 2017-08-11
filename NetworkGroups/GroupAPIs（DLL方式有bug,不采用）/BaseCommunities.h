#pragma once

#ifndef BASECOMMUNITIES_H
#define BASECOMMUNITIES_H

#ifdef DLL_FILE
#define EX_IM_PORT _declspec(dllexport) //导出类
#else
#define EX_IM_PORT _declspec(dllimport) //导入类
#endif

#include <map>
#include <queue>
#include <algorithm>
#include <time.h>
#include <ObjBase.h>
#include <initguid.h>
#include "Bridge.h"
#include "SimpleNode.h"
#include "NodeCloseInfo.h"

class EX_IM_PORT BaseCommunities
{

public:

	BaseCommunities(void);//默认构造函数
	virtual ~BaseCommunities(void);//析构函数

	BaseCommunities(const BaseCommunities & other);//拷贝构造函数
	BaseCommunities & operator = (const BaseCommunities & other);//赋值函数

	virtual void InitSetByNumber(const vector<vector<int>> & _pairs,int _groups_num) = 0;//根据数量初始化分组
	virtual void InitSetByBridge(const vector<vector<int>> & _pairs,vector<Bridge> & _bridges) = 0;//根据割边初始化分组

	//virtual void NetBfs() = 0;//无附加条件直接根据邻接表求取连通分量
	virtual void NetBfs(int GroupsNum) = 0;//根据分组数量求取连通分量
	virtual void NetBfs(vector<Bridge> & _bridges) = 0;//所有节点根据割边求取连通分量
	virtual void NetBfs(const vector<int> & NodeIds) = 0;//在规定节点集合范围内求连通分量
	virtual void NetBfs(const vector<int> & NodeIds,vector<Bridge> & _bridges) = 0;//在规定节点集合范围内剔除割边求连通分量

	virtual void BigSetChange(int num) = 0;//分群巨变
	virtual void LargeSetChange(int select_node,vector<NodeCloseInfo> & nears_info) = 0;//分群大变
	virtual int  SmallSetChange(int select_node,vector<NodeCloseInfo> & nears_info,vector<Bridge> & _bridges) = 0;//分群小变
	virtual double CalModularity() = 0;//计算模块度
	virtual string GetGroupSetsStr() = 0;//字符串形式获取分群结果

	//virtual int  GroupExpectsAdd(const vector<int> & NodesIds,const int & trans_node) = 0;
	//virtual int  GroupExpects(const vector<int> & NodesIds) = 0;//求某个群的群内边期望的2*edges倍
	//virtual int  MaxCrossEdges() = 0;//求某个分群方式下的最多可能交叉边数

	string GetGUID();//得到一个唯一标识符
	string JointVector(vector<int> vec);//向量元素拼接为字符串
	int  GroupEdges(const vector<int> & NodesIds);//求某个群的群内边数量
	void GetCloseInfo(vector<NodeCloseInfo> & nears,const int & select_node);//计算临近群相接点信息
	

public:

	int inner_edges;
	int total_edges;
	int total_nodes;
	map<int,SimpleNode> node_list;

};

#endif