#pragma once

#include <map>
#include <queue>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include "Bridge.h"
#include "Algorithm.h"

using namespace std;

struct node
{
	int degree;
	int group_id;
	bool visit;
	vector<int> nears;
	
	node(){}
	~node(){}

	node(const vector<int> _nears)
	{
		degree = _nears.size();
		group_id = -1;
		visit = false;
		nears = _nears;
	}

	node(const node & other)
	{
		degree = other.degree;
		group_id = other.group_id;
		visit = other.visit;
		nears = other.nears;
	}

	node & operator = (const node & other)
	{
		if(this!=&other)
		{
			degree = other.degree;
			group_id = other.group_id;
			visit = other.visit;
			nears = other.nears;
		}
		return *this;
	}

};

struct edge_attr
{
	double f_bridge;
	double pheromone;

	edge_attr(){}
	~edge_attr(){}

	edge_attr(double _f_b)
	{
		f_bridge = _f_b;
		pheromone = 1.0;
	}

	edge_attr(const edge_attr & other)
	{
		f_bridge = other.f_bridge;
		pheromone = other.pheromone;
	}

	edge_attr & operator = (const edge_attr & other)
	{
		if(this!=&other)
		{
			f_bridge = other.f_bridge;
			pheromone = other.pheromone;
		}
		return *this;
	}

};

struct group
{
	int id;
	int size;
	int edges;
	int expects;
	vector<int> nodes;

	group(){}
	~group(){}

	group(const group & other)
	{
		id = other.id;
		size = other.size;
		edges = other.edges;
		expects = other.expects;
		nodes = other.nodes;
	}

	group & operator = (const group & other)
	{
		if(this!=&other)
		{
			id = other.id;
			size = other.size;
			edges = other.edges;
			expects = other.expects;
			nodes = other.nodes;
		}
		return *this;
	}

};

struct local_info
{
	int group_id;
	int part_degree;
	vector<int> boundarys;

	local_info(){}
	~local_info(){}

	local_info(const local_info & other)
	{
		group_id = other.group_id;
		part_degree = other.part_degree;
		boundarys = other.boundarys;
	}

	local_info & operator = (const local_info & other)
	{
		if(this!=&other)
		{
			group_id = other.group_id;
			part_degree = other.part_degree;
			boundarys = other.boundarys;
		}
		return *this;
	}
};

class AntDetect
{

public:

	AntDetect(void);
	~AntDetect(void);

	void InitParameters(const vector<vector<int>> & _pairs);
	void NetBfs(vector<Bridge> & _bridges);
	void NetBfs(const vector<int> & NodeIds);
	void NetBfs(const vector<int> & NodeIds,vector<Bridge> & _bridges);
	int  LocalModify(int ant_node,double sita,double temperate,vector<local_info> & _info);
	void AcceptModify(int & ant_node,int ans,double acc_pro,vector<local_info> & _info);
	void GetCloseInfo(vector<local_info> & nears,const int & select_node);
	void UpdatePheromone(double acc_pro);

	int  GroupEdges(const vector<int> & NodesIds);
	int  GroupExpects(const vector<int> & NodesIds);
	int  GroupExpectsAdd(const vector<int> & NodesIds,const int & ant_node);
	
	double BridgeProperty(int a,int b,const vector<vector<int>> & _pairs);
	double CalModularity();

	void SaveGroupResult(CString path);
	string JointVector(vector<int> vec);
	string GetGroupSetsStr();

public:
	
	int nodes_num;
	int edges_num;
	int expects_num;
	int inner_edges;
	double modularity;

	vector<node> nodes_list;
	vector<Bridge> bridges;
	vector<group> groups_list;
	map<int,edge_attr> edges_attr;

	int deta_inner_edges;
	int deta_expects_num;
	int add_inner_edges;
	int extra_expects;
	int add_expects_num;
	vector<group> add_groups_list;
	double deta_modularity;

	int local_group_id;
	int trans_group_id;
	int local_nears_num;
	int trans_nears_num;

	map<int,double> add_pheromone;
};

