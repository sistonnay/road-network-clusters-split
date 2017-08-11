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

int Algorithm::dfs(int u,int father)//��u��DFS���еĸ����father��ջ�������߷����ӱ�
{
	vis[u] = 1;  
	int child=0;  
	dfn[u] = low[u] = ++dfs_clock;	//++dfs_clock !!!!!!!!��
	int degree = G[u].size();
	For(0,degree)
	{
		int v = G[u][i];
		if(!dfn[v])//���ӱ� 
		{
			child++;
			dfs(v, u);
			if(u!=root && dfn[u]<=low[v])
				cut.push_back(u);	//���,Ҫ���Ǹ����
			if(dfn[u]<low[v])
				bridges.push_back(*(new Bridge(u,v)));//���
			low[u] = min(low[u],low[v]);	//�ú���� low �������� u �� low ����
		}
		else if(v != father && vis[v])		//����� if(v!=father)����������ͼ��ȷ����Ҫ�߷����ӱ�  
		{  
			low[u] = min(low[u],dfn[v]);	//�÷���߸���u��low����
		}  
	}
	vis[u] = 0;  
	return child;  
}

void Algorithm::caculate()	//������ͨͼbfs,���㡢��  
{
	root=0;
	if(dfs(0,0)>1) cut.push_back(root);		//uΪ����,��u�ж���һ��������

	vector<int>::iterator iter;	//ȥ��
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
