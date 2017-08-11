#pragma once

#include <vector>
#include <string>
using namespace std;

class SimpleNode
{

public:

	SimpleNode(void);	//Ĭ�Ϲ��캯��
	~SimpleNode(void);	//��������

	SimpleNode(vector<int> _nears);//�Զ��幹�캯��
	SimpleNode(const SimpleNode & other);//�������캯��
	SimpleNode & operator = (const SimpleNode & other);//��ֵ����

	const vector<int> & GetNears();//�ⲿ��ȡ�ھӵ㼯��

public:

	int degree;//�ڵ��--�ڽӽڵ�����
	bool visit;//�ڵ�������
	string group_id;//�ڵ�����ȺID

private:

	vector<int> nears;//�ھӵ㼯��

};