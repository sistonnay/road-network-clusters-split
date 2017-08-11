#pragma once

#ifndef GROUPWITHEXPECTS_H
#define GROUPWITHEXPECTS_H

#ifdef DLL_FILE
#define EX_IM_PORT _declspec(dllexport) //������
#else
#define EX_IM_PORT _declspec(dllimport) //������
#endif

#include "basegroup.h"

class EX_IM_PORT GroupWithExpects : public BaseGroup
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

#endif