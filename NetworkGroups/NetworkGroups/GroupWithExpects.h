#pragma once

#include "basegroup.h"

class GroupWithExpects : public BaseGroup
{

public:

	GroupWithExpects(void):expects_num(0){}//���캯��,����
	~GroupWithExpects(void){}//��������,����

	GroupWithExpects(const GroupWithExpects & other);//�������캯��
	GroupWithExpects & operator = (const GroupWithExpects & other);//��ֵ����

	void AddNode(int _node);//���ӽڵ�
	void DelNode(int _node);//ɾ���ڵ�

public:

	int expects_num;//Ⱥ�ڶ�Ӧ���ͼ�ı�����ֵ��2m��,mΪ�����ܱ���

};