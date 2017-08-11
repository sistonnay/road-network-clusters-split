#pragma once

#include <map>
#include <algorithm>
#include "ExtendNode.h"
#include "ExtendBoundaryNode.h"

template<class T>
bool pairCompare(const T & x, const T & y)
{
  return x.second < y.second;
}

template<class T>
typename T::const_iterator map_max_element(const T &A)
{
    typedef typename T::value_type pair_type;
    return max_element(A.begin(), A.end(), pairCompare<typename T::value_type>);
}

template<class T>
bool pairCompare1(const T & x, const T & y)
{
  return x.second > y.second;
}

template<class T>
typename T::const_iterator map_min_element(const T &A)
{
    typedef typename T::value_type pair_type;
    return min_element(A.begin(), A.end(), pairCompare1<typename T::value_type>);
}

class ExtendCommunicaties
{

public:

	ExtendCommunicaties(void);
	~ExtendCommunicaties(void);

	ExtendCommunicaties(const vector<vector<int>> & _pairs,const vector<vector<int>> & _groups,const vector<int> _hiers);//自定义
	ExtendCommunicaties(const ExtendCommunicaties & other);//拷贝构造函数
	ExtendCommunicaties & operator = (const ExtendCommunicaties & other);//赋值函数
	
	void OverLappingCluster(double sita);
	void GetCloseNum(int & nears_num,const int & select_node);
	void FindBoundaryNodes(int a,const vector<int> & group_a,int b,const vector<int> & group_b,map<int,ExtendBoundaryNode> & tempBNode);
	
	double CalModularity();
	double CalPersistenceRation();
	string JointVector(vector<int> vec);
	void RefreshGroupGraph();
	void SaveGroupResult(CString path,const vector<int> & color);

public:
	
	int total_edges;
	int total_nodes;
	int low_hier;

	vector<vector<int>> group_set;
	vector<int> group_graph;
	map<int,ExtendNode> node_list;
	map<int,map<int,ExtendBoundaryNode>> boundary_nodes;

	double modularity;
	double modularity_new;
	double persistence;
	double persistence_new;

};

