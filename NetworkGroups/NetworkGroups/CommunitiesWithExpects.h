#pragma once

#include "basecommunities.h"
#include "GroupWithExpects.h"

class CommunitiesWithExpects :public BaseCommunities
{

public:

	CommunitiesWithExpects(void);//Ĭ�Ϲ��캯��
	~CommunitiesWithExpects(void);//��������

	CommunitiesWithExpects(const CommunitiesWithExpects & other);//�������캯��
	CommunitiesWithExpects & operator = (const CommunitiesWithExpects & other);//��ֵ����

	void InitSetByNumber(const vector<vector<int>> & _pairs,int _groups_num);//����������ʼ������
	void InitSetByBridge(const vector<vector<int>> & _pairs,vector<Bridge> & _bridges);//���ݸ�߳�ʼ������

	void NetBfs();//�޸�������ֱ�Ӹ����ڽӱ���ȡ��ͨ����
	void NetBfs(int GroupsNum);//���ݷ���������ȡ��ͨ����
	void NetBfs(vector<Bridge> & _bridges);//���нڵ���ݸ����ȡ��ͨ����
	void NetBfs(const vector<int> & NodeIds);//�ڹ涨�ڵ㼯�Ϸ�Χ������ͨ����
	void NetBfs(const vector<int> & NodeIds,vector<Bridge> & _bridges);//�ڹ涨�ڵ㼯�Ϸ�Χ���޳��������ͨ����

	int  BigSetChange(int num);//��Ⱥ���
	int  SmallSetChange(int select_node,vector<NodeCloseInfo> & nears_info,vector<Bridge> & _bridges);//��ȺС��
	void AcceptChange(int select_node,vector<NodeCloseInfo> & nears_info,int ans);//���ܷ�ȺС��
	void RefuseChange(int select_node,int ans);

	int  GroupExpectsAdd(const vector<int> & NodesIds,const int & trans_node);
	int  GroupExpects(const vector<int> & NodesIds);//��ĳ��Ⱥ��Ⱥ�ڱ�������2*edges��
	double CalModularity();//����ģ���
	string GetGroupSetsStr();//�ַ�����ʽ��ȡ��Ⱥ���
	void SaveGroupResult(CString path);//������

public:

	int inner_expects;
	map<string,GroupWithExpects> groups;

	int add_temp_expects;
	int add_inner_expects;
	map<string,GroupWithExpects> add_groups;

};