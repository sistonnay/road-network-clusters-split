#pragma once

#include "basecommunities.h"
#include "GroupWithMaxEdges.h"

class CommunitiesWithMaxEdges : public BaseCommunities
{

public:

	CommunitiesWithMaxEdges(void);//Ĭ�Ϲ��캯��
	~CommunitiesWithMaxEdges(void);//��������

	CommunitiesWithMaxEdges(const CommunitiesWithMaxEdges & other);//�������캯��
	CommunitiesWithMaxEdges & operator = (const CommunitiesWithMaxEdges & other);//��ֵ����

	void InitSetByNumber(const vector<vector<int>> & _pairs,int _groups_num);//����������ʼ������
	void InitSetByBridge(const vector<vector<int>> & _pairs,vector<Bridge> & _bridges);//���ݸ�߳�ʼ������

	void NetBfs();//�޸�������ֱ�Ӹ����ڽӱ���ȡ��ͨ����
	void NetBfs(int GroupsNum);//���ݷ���������ȡ��ͨ����
	void NetBfs(vector<Bridge> & _bridges);//���нڵ���ݸ����ȡ��ͨ����
	void NetBfs(const vector<int> & NodeIds);//�ڹ涨�ڵ㼯�Ϸ�Χ������ͨ����
	void NetBfs(const vector<int> & NodeIds,vector<Bridge> & _bridges);//�ڹ涨�ڵ㼯�Ϸ�Χ���޳��������ͨ����

	int	BigSetChange(int num);//��Ⱥ���
	int SmallSetChange(int select_node,vector<NodeCloseInfo> & nears_info,vector<Bridge> & _bridges);//��ȺС��
	int CalMaxCrossEdges();//������ཻ��ߵĺ�����Ҳ�����ü���������
	void AcceptChange(int select_node,vector<NodeCloseInfo> & nears_info,int ans);//���ܷ�Ⱥ�仯
	void RefuseChange(int select_node,int ans);

	double CalModularity();//����ģ���
	string GetGroupSetsStr();//�ַ�����ʽ��ȡ��Ⱥ���
	void SaveGroupResult(CString path);//������

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