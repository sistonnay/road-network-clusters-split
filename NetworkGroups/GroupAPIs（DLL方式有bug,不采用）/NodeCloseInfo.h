#pragma once

#ifndef NODECLOSEINFO_H
#define NODECLOSEINFO_H

#ifdef DLL_FILE
#define EX_IM_PORT _declspec(dllexport) //������
#else
#define EX_IM_PORT _declspec(dllimport) //������
#endif

#include <vector>
#include <string>
using namespace std;

class EX_IM_PORT NodeCloseInfo //ĳ�ٽ�Ⱥ��ӵ���Ϣ
{

public:

	NodeCloseInfo(void); //Ĭ�Ϲ��캯��
	~NodeCloseInfo(void);//��������

	NodeCloseInfo(const NodeCloseInfo & other);//�������캯��
	NodeCloseInfo & operator = (const NodeCloseInfo & other);//��ֵ����
	
	void AddCloseNode(int _node);//���ĳ�ٽ�Ⱥ��ӵ�
	const vector<int> & GetCloseNodes();//��ȡĳ�ٽ�Ⱥ��ӵ㼯��

public:

	string group_id;//ȺID
	int close_num;	//��ӵ�����

private:
	
	vector<int> close_nodes;//��ӵ㼯��

};

#endif