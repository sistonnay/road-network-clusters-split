#pragma once

#ifndef COMMUNITIESWITHMAXEDGES_H
#define COMMUNITIESWITHMAXEDGES_H

#ifdef DLL_FILE
#define EX_IM_PORT _declspec(dllexport) //������
#else
#define EX_IM_PORT _declspec(dllimport) //������
#endif

#include "basecommunities.h"
#include "GroupWithMaxEdges.h"

class EX_IM_PORT CommunitiesWithMaxEdges : public BaseCommunities
{

public:

	CommunitiesWithMaxEdges(void);//Ĭ�Ϲ��캯��
	~CommunitiesWithMaxEdges(void);//��������

	CommunitiesWithMaxEdges(const CommunitiesWithMaxEdges & other);//�������캯��
	CommunitiesWithMaxEdges & operator = (const CommunitiesWithMaxEdges & other);//��ֵ����

	void InitSetByNumber(const vector<vector<int>> & _pairs,int _groups_num);//����������ʼ������
	void InitSetByBridge(const vector<vector<int>> & _pairs,vector<Bridge> & _bridges);//���ݸ�߳�ʼ������

	void NetBfs(int GroupsNum);//���ݷ���������ȡ��ͨ����
	void NetBfs(vector<Bridge> & _bridges);//���нڵ���ݸ����ȡ��ͨ����
	void NetBfs(const vector<int> & NodeIds);//�ڹ涨�ڵ㼯�Ϸ�Χ������ͨ����
	void NetBfs(const vector<int> & NodeIds,vector<Bridge> & _bridges);//�ڹ涨�ڵ㼯�Ϸ�Χ���޳��������ͨ����

	void BigSetChange(int num);//��Ⱥ�ޱ�
	void LargeSetChange(int select_node,vector<NodeCloseInfo> & nears_info);//��Ⱥ���
	int  SmallSetChange(int select_node,vector<NodeCloseInfo> & nears_info,vector<Bridge> & _bridges);//��ȺС��
	
	double CalModularity();//����ģ���
	string GetGroupSetsStr();//�ַ�����ʽ��ȡ��Ⱥ���

public:

	int cross_edges;
	int max_inner_edges;
	int max_cross_edges;
	int max_total_edges;
	map<string,GroupWithMaxEdges> groups;

};

#endif