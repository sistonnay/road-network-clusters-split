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

	BaseCommunities(void);//Ĭ�Ϲ��캯��
	virtual ~BaseCommunities(void);//��������

	BaseCommunities(const BaseCommunities & other);//�������캯��
	BaseCommunities & operator = (const BaseCommunities & other);//��ֵ����

	virtual void InitSetByNumber(const vector<vector<int>> & _pairs,int _groups_num) = 0;//����������ʼ������
	virtual void InitSetByBridge(const vector<vector<int>> & _pairs,vector<Bridge> & _bridges) = 0;//���ݸ�߳�ʼ������

	virtual void NetBfs() = 0;//�޸�������ֱ�Ӹ����ڽӱ���ȡ��ͨ����
	virtual void NetBfs(int GroupsNum) = 0;//���ݷ���������ȡ��ͨ����
	virtual void NetBfs(vector<Bridge> & _bridges) = 0;//���нڵ���ݸ����ȡ��ͨ����
	virtual void NetBfs(const vector<int> & NodeIds) = 0;//�ڹ涨�ڵ㼯�Ϸ�Χ������ͨ����
	virtual void NetBfs(const vector<int> & NodeIds,vector<Bridge> & _bridges) = 0;//�ڹ涨�ڵ㼯�Ϸ�Χ���޳��������ͨ����

	virtual int  BigSetChange(int num) = 0;//��Ⱥ���
	virtual int  SmallSetChange(int select_node,vector<NodeCloseInfo> & nears_info,vector<Bridge> & _bridges) = 0;//��ȺС��
	virtual void AcceptChange(int select_node,vector<NodeCloseInfo> & nears_info,int ans) = 0;//���ܷ�Ⱥ�仯
	virtual void RefuseChange(int select_node,int ans) = 0;//�ܾ���Ⱥ�仯

	virtual double CalModularity() = 0;//����ģ���
	virtual string GetGroupSetsStr() = 0;//�ַ�����ʽ��ȡ��Ⱥ���
	virtual void SaveGroupResult(CString path) = 0;//������

	string GetGUID();//�õ�һ��Ψһ��ʶ��
	string JointVector(vector<int> vec);//����Ԫ��ƴ��Ϊ�ַ���
	int  GroupEdges(const vector<int> & NodesIds);//��ĳ��Ⱥ��Ⱥ�ڱ�����
	void GetCloseInfo(vector<NodeCloseInfo> & nears,const int & select_node);//�����ٽ�Ⱥ��ӵ���Ϣ	
	
public:

	int inner_edges;
	int total_edges;
	int total_nodes;
	map<int,SimpleNode> node_list;

	int add_inner_edges;
	int trans_near_class;//��ת�Ƶ���Ⱥ�±�
	string select_group_id;//ѡ�е����ڵ�Ⱥ
	string trans_group_id;//��ת��ȺID
};