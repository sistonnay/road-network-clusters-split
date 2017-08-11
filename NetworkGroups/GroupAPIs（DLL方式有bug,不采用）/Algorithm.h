//ʹ��dllҪ��.dll,.lib,.h�ļ���������ĿcppĿ¼��
//����Properties->Linker->Input->Additional Dependecies��
//���facedll.lib�������䣺#pragma comment(lib, "Bridge.lib")
#pragma once
#define For(x1,x2) for(int i=x1;i<x2;++i)

#ifndef ALGORITHM_H
#define ALGORITHM_H

#ifdef DLL_FILE
#define EX_IM_PORT _declspec(dllexport) //������
#else
#define EX_IM_PORT _declspec(dllimport) //������
#endif

#include <vector>
#include <algorithm>
#include "Bridge.h"
using namespace std;

class EX_IM_PORT Algorithm
{

public:

	Algorithm(void);
	Algorithm(vector<vector<int>> g);
	~Algorithm(void);
	vector<Bridge> & getBridges();
	vector<int> & getCuts();

private:

	int dfs(int u,int father);
	void caculate();

private:

	vector<vector<int>> G;	//�ڽӱ�
	vector<int> dfn,low,vis;//�洢...
	vector<int> cut;		//�洢���	
	vector<Bridge> bridges;	//�洢���
	int dfs_clock,N;		//ͼ���ܽڵ�
	int root;				//���ڵ�

};

#endif