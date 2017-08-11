#pragma once

#ifndef COMMUNITIESWITHEXPECTS_H
#define COMMUNITIESWITHEXPECTS_H

#ifdef DLL_FILE
#define EX_IM_PORT _declspec(dllexport) //导出类
#else
#define EX_IM_PORT _declspec(dllimport) //导入类
#endif

#include "basecommunities.h"
#include "GroupWithExpects.h"

class EX_IM_PORT CommunitiesWithExpects :public BaseCommunities
{

public:

	CommunitiesWithExpects(void);//默认构造函数
	~CommunitiesWithExpects(void);//析构函数

	CommunitiesWithExpects(const CommunitiesWithExpects & other);//拷贝构造函数
	CommunitiesWithExpects & operator = (const CommunitiesWithExpects & other);//赋值函数

	void InitSetByNumber(const vector<vector<int>> & _pairs,int _groups_num);//根据数量初始化分组
	void InitSetByBridge(const vector<vector<int>> & _pairs,vector<Bridge> & _bridges);//根据割边初始化分组

	void NetBfs(int GroupsNum);//根据分组数量求取连通分量
	void NetBfs(vector<Bridge> & _bridges);//所有节点根据割边求取连通分量
	void NetBfs(const vector<int> & NodeIds);//在规定节点集合范围内求连通分量
	void NetBfs(const vector<int> & NodeIds,vector<Bridge> & _bridges);//在规定节点集合范围内剔除割边求连通分量

	void BigSetChange(int num);//分群巨变
	void LargeSetChange(int select_node,vector<NodeCloseInfo> & nears_info);//分群大变
	int  SmallSetChange(int select_node,vector<NodeCloseInfo> & nears_info,vector<Bridge> & _bridges);//分群小变
	
	int  GroupExpectsAdd(const vector<int> & NodesIds,const int & trans_node);
	int  GroupExpects(const vector<int> & NodesIds);//求某个群的群内边期望的2*edges倍
	double CalModularity();//计算模块度
	string GetGroupSetsStr();//字符串形式获取分群结果

public:

	int inner_expects;
	map<string,GroupWithExpects> groups;

};

#endif