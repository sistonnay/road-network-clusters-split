#pragma once

#include "basecommunities.h"
#include "GroupWithMaxEdges.h"

class CommunitiesWithMaxEdges : public BaseCommunities
{

public:

	CommunitiesWithMaxEdges(void);//默认构造函数
	~CommunitiesWithMaxEdges(void);//析构函数

	CommunitiesWithMaxEdges(const CommunitiesWithMaxEdges & other);//拷贝构造函数
	CommunitiesWithMaxEdges & operator = (const CommunitiesWithMaxEdges & other);//赋值函数

	void InitSetByNumber(const vector<vector<int>> & _pairs,int _groups_num);//根据数量初始化分组
	void InitSetByBridge(const vector<vector<int>> & _pairs,vector<Bridge> & _bridges);//根据割边初始化分组

	void NetBfs();//无附加条件直接根据邻接表求取连通分量
	void NetBfs(int GroupsNum);//根据分组数量求取连通分量
	void NetBfs(vector<Bridge> & _bridges);//所有节点根据割边求取连通分量
	void NetBfs(const vector<int> & NodeIds);//在规定节点集合范围内求连通分量
	void NetBfs(const vector<int> & NodeIds,vector<Bridge> & _bridges);//在规定节点集合范围内剔除割边求连通分量

	int	BigSetChange(int num);//分群大变
	int SmallSetChange(int select_node,vector<NodeCloseInfo> & nears_info,vector<Bridge> & _bridges);//分群小变
	int CalMaxCrossEdges();//计算最多交叉边的函数，也可以用减法来计算
	void AcceptChange(int select_node,vector<NodeCloseInfo> & nears_info,int ans);//接受分群变化
	void RefuseChange(int select_node,int ans);

	double CalModularity();//计算模块度
	string GetGroupSetsStr();//字符串形式获取分群结果
	void SaveGroupResult(CString path);//保存结果

public:

	int cross_edges;
	int max_inner_edges;
	int max_cross_edges;
	int max_total_edges;
	map<string,GroupWithMaxEdges> groups;

	int add_max_inner_edges;
	GroupWithMaxEdges select_group;
	map<string,GroupWithMaxEdges> add_groups;

};