#pragma once

#include <vector>
#include <string>
using namespace std;

class ExtendNode
{

public:

	ExtendNode(void);	//Ĭ�Ϲ��캯��
	~ExtendNode(void);	//��������

	ExtendNode(vector<int> _nears,int _hierachical);//�Զ��幹�캯��
	ExtendNode(const ExtendNode & other);//�������캯��
	ExtendNode & operator = (const ExtendNode & other);//��ֵ����

public:

	int degree;//�ڵ��--�ڽӽڵ�����
	int hierachical;//�ڵ���
	int near_c_num;//��ʼ�ڽ�������
	bool checked;//�Ƿ��ǹ�
	vector<int> nears;//�ھӵ㼯��
	vector<int> group_ids;//�ڵ�����

};