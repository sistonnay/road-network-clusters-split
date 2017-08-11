#pragma once

#ifndef SIMPLENODE_H
#define SIMPLENODE_H

#ifdef DLL_FILE
#define EX_IM_PORT _declspec(dllexport) //������
#else
#define EX_IM_PORT _declspec(dllimport) //������
#endif

#include <vector>
#include <string>
using namespace std;

class EX_IM_PORT SimpleNode
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

#endif