#pragma once

#include <vector>
#include <string>
using namespace std;

class NodeCloseInfo //ĳ�ٽ�Ⱥ��ӵ���Ϣ
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