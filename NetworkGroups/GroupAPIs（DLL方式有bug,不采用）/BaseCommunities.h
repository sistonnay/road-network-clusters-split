#pragma once

#ifndef BASECOMMUNITIES_H
#define BASECOMMUNITIES_H

#ifdef DLL_FILE
#define EX_IM_PORT _declspec(dllexport) //������
#else
#define EX_IM_PORT _declspec(dllimport) //������
#endif

#include <map>
#include <queue>
#include <algorithm>
#include <time.h>
#include <ObjBase.h>
#include <initguid.h>
#include "Bridge.h"
#include "SimpleNode.h"
#include "NodeCloseInfo.h"

class EX_IM_PORT BaseCommunities
{

public:

	BaseCommunities(void);//Ĭ�Ϲ��캯��
	virtual ~BaseCommunities(void);//��������

	BaseCommunities(const BaseCommunities & other);//�������캯��
	BaseCommunities & operator = (const BaseCommunities & other);//��ֵ����

	virtual void InitSetByNumber(const vector<vector<int>> & _pairs,int _groups_num) = 0;//����������ʼ������
	virtual void InitSetByBridge(const vector<vector<int>> & _pairs,vector<Bridge> & _bridges) = 0;//���ݸ�߳�ʼ������

	//virtual void NetBfs() = 0;//�޸�������ֱ�Ӹ����ڽӱ���ȡ��ͨ����
	virtual void NetBfs(int GroupsNum) = 0;//���ݷ���������ȡ��ͨ����
	virtual void NetBfs(vector<Bridge> & _bridges) = 0;//���нڵ���ݸ����ȡ��ͨ����
	virtual void NetBfs(const vector<int> & NodeIds) = 0;//�ڹ涨�ڵ㼯�Ϸ�Χ������ͨ����
	virtual void NetBfs(const vector<int> & NodeIds,vector<Bridge> & _bridges) = 0;//�ڹ涨�ڵ㼯�Ϸ�Χ���޳��������ͨ����

	virtual void BigSetChange(int num) = 0;//��Ⱥ�ޱ�
	virtual void LargeSetChange(int select_node,vector<NodeCloseInfo> & nears_info) = 0;//��Ⱥ���
	virtual int  SmallSetChange(int select_node,vector<NodeCloseInfo> & nears_info,vector<Bridge> & _bridges) = 0;//��ȺС��
	virtual double CalModularity() = 0;//����ģ���
	virtual string GetGroupSetsStr() = 0;//�ַ�����ʽ��ȡ��Ⱥ���

	//virtual int  GroupExpectsAdd(const vector<int> & NodesIds,const int & trans_node) = 0;
	//virtual int  GroupExpects(const vector<int> & NodesIds) = 0;//��ĳ��Ⱥ��Ⱥ�ڱ�������2*edges��
	//virtual int  MaxCrossEdges() = 0;//��ĳ����Ⱥ��ʽ�µ������ܽ������

	string GetGUID();//�õ�һ��Ψһ��ʶ��
	string JointVector(vector<int> vec);//����Ԫ��ƴ��Ϊ�ַ���
	int  GroupEdges(const vector<int> & NodesIds);//��ĳ��Ⱥ��Ⱥ�ڱ�����
	void GetCloseInfo(vector<NodeCloseInfo> & nears,const int & select_node);//�����ٽ�Ⱥ��ӵ���Ϣ
	

public:

	int inner_edges;
	int total_edges;
	int total_nodes;
	map<int,SimpleNode> node_list;

};

#endif