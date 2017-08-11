//使用dll要将.dll,.lib,.h文件拷贝到项目cpp目录，
//并在Properties->Linker->Input->Additional Dependecies中
//添加facedll.lib或添加语句：#pragma comment(lib, "Bridge.lib")
#pragma once
#define For(x1,x2) for(int i=x1;i<x2;++i)

#ifndef ALGORITHM_H
#define ALGORITHM_H

#ifdef DLL_FILE
#define EX_IM_PORT _declspec(dllexport) //导出类
#else
#define EX_IM_PORT _declspec(dllimport) //导入类
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

	vector<vector<int>> G;	//邻接表
	vector<int> dfn,low,vis;//存储...
	vector<int> cut;		//存储割点	
	vector<Bridge> bridges;	//存储割边
	int dfs_clock,N;		//图的总节点
	int root;				//根节点

};

#endif