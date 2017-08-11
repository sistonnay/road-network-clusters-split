#pragma once

#include <vector>
#include <string>
#include <algorithm>
using namespace std;

class BaseGroup
{

public:

	BaseGroup(void):edges_num(0),nodes_num(0){}	//���캯��,����
	virtual ~BaseGroup(void){} //����������,����

	BaseGroup(const BaseGroup & other);	//�������캯��
	BaseGroup & operator = (const BaseGroup & other);//��ֵ����
	
	vector<int> & GetNodes();//�ⲿ��ȡ�ڵ㼯��

	virtual void AddNode(int _node) = 0;//���ӽڵ�
	virtual void DelNode(int _node) = 0;//ɾ���ڵ�

public:

	int edges_num;//Ⱥ�ڱ�����
	int nodes_num;//Ⱥ�ڽڵ���

protected:

	vector<int> nodes;//�ڵ㼯��

};