#pragma once

#ifndef COMMUNITIESWITHEXPECTS_H
#define COMMUNITIESWITHEXPECTS_H

#ifdef DLL_FILE
#define EX_IM_PORT _declspec(dllexport) //������
#else
#define EX_IM_PORT _declspec(dllimport) //������
#endif

#include "basecommunities.h"
#include "GroupWithExpects.h"

class EX_IM_PORT CommunitiesWithExpects :public BaseCommunities
{

public:

	CommunitiesWithExpects(void);//Ĭ�Ϲ��캯��
	~CommunitiesWithExpects(void);//��������

	CommunitiesWithExpects(const CommunitiesWithExpects & other);//�������캯��
	CommunitiesWithExpects & operator = (const CommunitiesWithExpects & other);//��ֵ����

	void InitSetByNumber(const vector<vector<int>> & _pairs,int _groups_num);//����������ʼ������
	void InitSetByBridge(const vector<vector<int>> & _pairs,vector<Bridge> & _bridges);//���ݸ�߳�ʼ������

	void NetBfs(int GroupsNum);//���ݷ���������ȡ��ͨ����
	void NetBfs(vector<Bridge> & _bridges);//���нڵ���ݸ����ȡ��ͨ����
	void NetBfs(const vector<int> & NodeIds);//�ڹ涨�ڵ㼯�Ϸ�Χ������ͨ����
	void NetBfs(const vector<int> & NodeIds,vector<Bridge> & _bridges);//�ڹ涨�ڵ㼯�Ϸ�Χ���޳��������ͨ����

	void BigSetChange(int num);//��Ⱥ�ޱ�
	void LargeSetChange(int select_node,vector<NodeCloseInfo> & nears_info);//��Ⱥ���
	int  SmallSetChange(int select_node,vector<NodeCloseInfo> & nears_info,vector<Bridge> & _bridges);//��ȺС��
	
	int  GroupExpectsAdd(const vector<int> & NodesIds,const int & trans_node);
	int  GroupExpects(const vector<int> & NodesIds);//��ĳ��Ⱥ��Ⱥ�ڱ�������2*edges��
	double CalModularity();//����ģ���
	string GetGroupSetsStr();//�ַ�����ʽ��ȡ��Ⱥ���

public:

	int inner_expects;
	map<string,GroupWithExpects> groups;

};

#endif