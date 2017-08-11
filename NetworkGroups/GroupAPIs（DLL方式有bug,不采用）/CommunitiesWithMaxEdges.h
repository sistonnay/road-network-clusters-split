#pragma once

#ifndef COMMUNITIESWITHMAXEDGES_H
#define COMMUNITIESWITHMAXEDGES_H

#ifdef DLL_FILE
#define EX_IM_PORT _declspec(dllexport) //导出类
#else
#define EX_IM_PORT _declspec(dllimport) //导入类
#endif

#include "basecommunities.h"
#include "GroupWithMaxEdges.h"

class EX_IM_PORT CommunitiesWithMaxEdges : public BaseCommunities
{

public:

	CommunitiesWithMaxEdges(void);//默认构造函数
	~CommunitiesWithMaxEdges(void);//析构函数

	CommunitiesWithMaxEdges(const CommunitiesWithMaxEdges & other);//拷贝构造函数
	CommunitiesWithMaxEdges & operator = (const CommunitiesWithMaxEdges & other);//赋值函数

	void InitSetByNumber(const vector<vector<int>> & _pairs,int _groups_num);//根据数量初始化分组
	void InitSetByBridge(const vector<vector<int>> & _pairs,vector<Bridge> & _bridges);//根据割边初始化分组

	void NetBfs(int GroupsNum);//根据分组数量求取连通分量
	void NetBfs(vector<Bridge> & _bridges);//所有节点根据割边求取连通分量
	void NetBfs(const vector<int> & NodeIds);//在规定节点集合范围内求连通分量
	void NetBfs(const vector<int> & NodeIds,vector<Bridge> & _bridges);//在规定节点集合范围内剔除割边求连通分量

	void BigSetChange(int num);//分群巨变
	void LargeSetChange(int select_node,vector<NodeCloseInfo> & nears_info);//分群大变
	int  SmallSetChange(int select_node,vector<NodeCloseInfo> & nears_info,vector<Bridge> & _bridges);//分群小变
	
	double CalModularity();//计算模块度
	string GetGroupSetsStr();//字符串形式获取分群结果

public:

	int cross_edges;
	int max_inner_edges;
	int max_cross_edges;
	int max_total_edges;
	map<string,GroupWithMaxEdges> groups;

};

#endif