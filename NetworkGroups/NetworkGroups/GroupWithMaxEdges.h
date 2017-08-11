#pragma once

#include "basegroup.h"

class GroupWithMaxEdges : public BaseGroup
{

public:

	GroupWithMaxEdges(void):max_edges_num(0){}//���캯��,����
	~GroupWithMaxEdges(void){};//��������,����

	GroupWithMaxEdges(const GroupWithMaxEdges & other);//�������캯��
	GroupWithMaxEdges & operator = (const GroupWithMaxEdges & other);//��ֵ����

	void AddNode(int _node);//���ӽڵ�
	void DelNode(int _node);//ɾ���ڵ�

public:

	int max_edges_num;//Ⱥ�������ܳ��ֱ�����

};