#pragma once

#include <map>
#include <queue>
#include <algorithm>
#include <time.h>
#include <ObjBase.h>
#include <initguid.h>
#include "Bridge.h"
#include "SimpleNode.h"
#include "NodeCloseInfo.h"

class BaseCommunities
{

public:

	BaseCommunities(void);//默认构造函数
	virtual ~BaseCommunities(void);//析构函数

	BaseCommunities(const BaseCommunities & other);//拷贝构造函数
	BaseCommunities & operator = (const BaseCommunities & other);//赋值函数

	virtual void InitSetByNumber(const vector<vector<int>> & _pairs,int _groups_num) = 0;//根据数量初始化分组
	virtual void InitSetByBridge(const vector<vector<int>> & _pairs,vector<Bridge> & _bridges) = 0;//根据割边初始化分组

	virtual void NetBfs() = 0;//无附加条件直接根据邻接表求取连通分量
	virtual void NetBfs(int GroupsNum) = 0;//根据分组数量求取连通分量
	virtual void NetBfs(vector<Bridge> & _bridges) = 0;//所有节点根据割边求取连通分量
	virtual void NetBfs(const vector<int> & NodeIds) = 0;//在规定节点集合范围内求连通分量
	virtual void NetBfs(const vector<int> & NodeIds,vector<Bridge> & _bridges) = 0;//在规定节点集合范围内剔除割边求连通分量

	virtual int  BigSetChange(int num) = 0;//分群大变
	virtual int  SmallSetChange(int select_node,vector<NodeCloseInfo> & nears_info,vector<Bridge> & _bridges) = 0;//分群小变
	virtual void AcceptChange(int select_node,vector<NodeCloseInfo> & nears_info,int ans) = 0;//接受分群变化
	virtual void RefuseChange(int select_node,int ans) = 0;//拒绝分群变化

	virtual double CalModularity() = 0;//计算模块度
	virtual string GetGroupSetsStr() = 0;//字符串形式获取分群结果
	virtual void SaveGroupResult(CString path) = 0;//保存结果

	string GetGUID();//得到一个唯一标识符
	string JointVector(vector<int> vec);//向量元素拼接为字符串
	int  GroupEdges(const vector<int> & NodesIds);//求某个群的群内边数量
	void GetCloseInfo(vector<NodeCloseInfo> & nears,const int & select_node);//计算临近群相接点信息	
	
public:

	int inner_edges;
	int total_edges;
	int total_nodes;
	map<int,SimpleNode> node_list;

	int add_inner_edges;
	int trans_near_class;//待转移的异群下标
	string select_group_id;//选中点所在的群
	string trans_group_id;//待转移群ID
};