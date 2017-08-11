#pragma once

#ifndef GROUPWITHMAXEDGES_H
#define GROUPWITHMAXEDGES_H

#ifdef DLL_FILE
#define EX_IM_PORT _declspec(dllexport) //������
#else
#define EX_IM_PORT _declspec(dllimport) //������
#endif

#include "basegroup.h"

class EX_IM_PORT GroupWithMaxEdges : public BaseGroup
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

#endif