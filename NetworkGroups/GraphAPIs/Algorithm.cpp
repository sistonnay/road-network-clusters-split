//
#ifndef DLL_FILE
#define DLL_FILE
#endif

#include "Algorithm.h"

Algorithm::Algorithm(void){}

Algorithm::Algorithm(vector<vector<int>> g)
{
	G = g;
	dfs_clock = 0;
	N = G.size();
	dfn.resize(N);
	low.resize(N);
	vis.resize(N);
	for(int i=0;i<N;++i) dfn[i] = 0;
	root = 0;
	caculate();
}

Algorithm::~Algorithm(void){}

int Algorithm::dfs(int u,int father)//把u在DFS树中的父结点father入栈，避免走反父子边
{
	vis[u] = 1;  
	int child=0;  
	dfn[u] = low[u] = ++dfs_clock;	//++dfs_clock !!!!!!!!①
	int degree = G[u].size();
	For(0,degree)
	{
		int v = G[u][i];
		if(!dfn[v])//父子边 
		{
			child++;
			dfs(v, u);
			if(u!=root && dfn[u]<=low[v])
				cut.push_back(u);	//割点,要求不是根结点
			if(dfn[u]<low[v])
				bridges.push_back(*(new Bridge(u,v)));//割边
			low[u] = min(low[u],low[v]);	//用后代的 low 函数更新 u 的 low 函数
		}
		else if(v != father && vis[v])		//反向边 if(v!=father)由于是无向图！确保不要走反父子边  
		{  
			low[u] = min(low[u],dfn[v]);	//用反向边更新u的low函数
		}  
	}
	vis[u] = 0;  
	return child;  
}

void Algorithm::caculate()	//无向连通图bfs,求割点、桥  
{
	root=0;
	if(dfs(0,0)>1) cut.push_back(root);		//u为树根,且u有多于一个子树。

	vector<int>::iterator iter;	//去重
	sort(cut.begin(),cut.end());
	iter = unique(cut.begin(),cut.end());
	if(iter != cut.end()) cut.erase(iter,cut.end());
}

vector<Bridge> & Algorithm::getBridges()
{
	return bridges;
}

vector<int> & Algorithm::getCuts()
{
	return cut;
}
