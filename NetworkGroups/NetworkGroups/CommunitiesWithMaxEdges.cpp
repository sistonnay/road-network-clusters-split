#include "StdAfx.h"
#include "CommunitiesWithMaxEdges.h"

CommunitiesWithMaxEdges::CommunitiesWithMaxEdges(void)
{
	cross_edges = 0;
	max_inner_edges = 0;
	max_cross_edges = 0;
	max_total_edges = 0;
	add_max_inner_edges = 0;
}

CommunitiesWithMaxEdges::~CommunitiesWithMaxEdges(void){}

CommunitiesWithMaxEdges::CommunitiesWithMaxEdges(const CommunitiesWithMaxEdges & other):BaseCommunities(other)
{
	cross_edges = other.cross_edges;
	max_inner_edges = other.max_inner_edges;
	max_cross_edges = other.max_cross_edges;
	max_total_edges = other.max_total_edges;
	add_max_inner_edges = other.add_max_inner_edges;
	select_group = other.select_group;
	groups = other.groups;
}

CommunitiesWithMaxEdges & CommunitiesWithMaxEdges::operator = (const CommunitiesWithMaxEdges & other)
{
	if(this!=&other)
	{
		BaseCommunities::operator=(other);
		cross_edges = other.cross_edges;
		max_inner_edges = other.max_inner_edges;
		max_cross_edges = other.max_cross_edges;
		max_total_edges = other.max_total_edges;
		add_max_inner_edges = other.add_max_inner_edges;
		select_group = other.select_group;
		groups = other.groups;
	}
	return *this;
}

void CommunitiesWithMaxEdges::InitSetByNumber(const vector<vector<int>> & _pairs,int _groups_num)
{
	total_edges = 0;
	add_inner_edges = 0;
	add_max_inner_edges = 0;
	total_nodes = _pairs.size();
	max_total_edges = total_nodes * (total_nodes-1) / 2;
	if(!(_groups_num<=total_nodes && _groups_num>0)) throw new exception("错误的分组数量");

	for(int i=0;i<total_nodes;++i)
	{
		SimpleNode temp(_pairs[i]);
		total_edges += temp.degree;
		node_list.insert(pair<int,SimpleNode>(i,temp));	
	}
	total_edges /= 2;

	NetBfs(_groups_num);
	map<string,GroupWithMaxEdges>::iterator it;
	for(it=add_groups.begin();it!=add_groups.end();++it)
	{
		for(int i=0;i<it->second.nodes_num;++i)
			node_list[it->second.GetNodes()[i]].group_id = it->first;//更新节点群属
	}
	groups = add_groups;
	inner_edges = add_inner_edges;
	max_inner_edges = add_max_inner_edges;
	cross_edges = total_edges - inner_edges;
	max_cross_edges = max_total_edges - max_inner_edges;

	add_groups.clear();
	add_inner_edges = 0;
	add_max_inner_edges = 0;
	trans_near_class = -1;
	trans_group_id = "";
	select_group_id = "";
}

void CommunitiesWithMaxEdges::InitSetByBridge(const vector<vector<int>> & _pairs,vector<Bridge> & _bridges)
{
	total_edges = 0;
	add_inner_edges = 0;
	add_max_inner_edges = 0;
	total_nodes = _pairs.size();
	max_total_edges = total_nodes * (total_nodes-1) / 2;

	for(int i=0;i<total_nodes;++i)
	{
		SimpleNode temp(_pairs[i]);
		total_edges += temp.degree;
		node_list.insert(pair<int,SimpleNode>(i,temp));	
	}
	total_edges /= 2;

	NetBfs(_bridges);
	map<string,GroupWithMaxEdges>::iterator it;
	for(it=add_groups.begin();it!=add_groups.end();++it)
	{
		for(int i=0;i<it->second.nodes_num;++i)
			node_list[it->second.GetNodes()[i]].group_id = it->first;//更新节点群属
	}
	groups = add_groups;
	inner_edges = add_inner_edges;
	max_inner_edges = add_max_inner_edges;
	cross_edges = total_edges - inner_edges;
	max_cross_edges = max_total_edges - max_inner_edges;

	add_groups.clear();
	add_inner_edges = 0;
	add_max_inner_edges = 0;
	trans_near_class = -1;
	trans_group_id = "";
	select_group_id = "";
}

void CommunitiesWithMaxEdges::NetBfs()//非连通图才会分成多个群
{
	int j,k;
	map<int,SimpleNode>::iterator iter,it;
	for(iter=node_list.begin();iter!=node_list.end();++iter)
	{
		if(!iter->second.visit)
		{
			iter->second.visit = true;
			GroupWithMaxEdges temp_group;
			string group_id = GetGUID();
			queue<int> _queue;
			_queue.push(iter->first);
			while(!_queue.empty())
			{
				k = _queue.front();
				_queue.pop();
				temp_group.AddNode(k);
				//node_list[k].group_id = group_id;
				for(j=0;j<node_list[k].degree;++j)
				{
					it = node_list.find(node_list[k].GetNears()[j]);
					++temp_group.edges_num;
					if(!it->second.visit && it!=node_list.end())
					{
						_queue.push(it->first);
						it->second.visit = true;
					}
				}
			}
			temp_group.edges_num /= 2;
			temp_group.nodes_num = temp_group.GetNodes().size();
			temp_group.max_edges_num = temp_group.nodes_num * (temp_group.nodes_num-1)/2;
			
			add_inner_edges += temp_group.edges_num;
			add_max_inner_edges += temp_group.max_edges_num;
			add_groups.insert(pair<string,GroupWithMaxEdges>(group_id,temp_group));
		}	
	}
	for(iter=node_list.begin();iter!=node_list.end();++iter)
	{
		iter->second.visit = false;
	}
}

void CommunitiesWithMaxEdges::NetBfs(int _groups_num)
{
	int i,j,k,dep,n_dep;
	dep = total_nodes/_groups_num;
	map<int,SimpleNode>::iterator iter,it;
	for(iter=node_list.begin(),i=0;iter!=node_list.end();++iter,++i)
	{
		if(!iter->second.visit)
		{	
			n_dep = i/dep;
			iter->second.visit = true;
			GroupWithMaxEdges temp_group;
			string group_id = GetGUID();
			queue<int> _queue;
			_queue.push(iter->first);
			while(!_queue.empty())
			{
				k = _queue.front();
				_queue.pop();
				temp_group.AddNode(k);
				//node_list[k].group_id = group_id;
				for(j=0;j<node_list[k].degree;++j)
				{
					if(node_list[k].GetNears()[j]>=dep*n_dep&&node_list[k].GetNears()[j]<dep*(n_dep+1))
					{	
						it = node_list.find(node_list[k].GetNears()[j]);
						++temp_group.edges_num;
						if(!it->second.visit && it!=node_list.end())
						{
							_queue.push(it->first);
							it->second.visit = true;
						}
					}
				}
			}
			temp_group.edges_num /= 2;
			temp_group.nodes_num = temp_group.GetNodes().size();
			temp_group.max_edges_num = temp_group.nodes_num * (temp_group.nodes_num-1)/2;
			
			add_inner_edges += temp_group.edges_num;
			add_max_inner_edges += temp_group.max_edges_num;
			add_groups.insert(pair<string,GroupWithMaxEdges>(group_id,temp_group));
		}
	}
	for(iter=node_list.begin();iter!=node_list.end();++iter)
	{
		iter->second.visit = false;
	}
}

void CommunitiesWithMaxEdges::NetBfs(vector<Bridge> & _bridges)
{
	int j,k;
	map<int,SimpleNode>::iterator iter,it;
	for(iter=node_list.begin();iter!=node_list.end();++iter)
	{
		if(!iter->second.visit)
		{
			iter->second.visit = true;
			GroupWithMaxEdges temp_group;
			string group_id = GetGUID();
			queue<int> _queue;
			_queue.push(iter->first);
			while(!_queue.empty())
			{
				k = _queue.front();
				_queue.pop();
				temp_group.AddNode(k);
				//node_list[k].group_id = group_id;
				for(j=0;j<node_list[k].degree;++j)
				{
					it = node_list.find(node_list[k].GetNears()[j]);
					if(it!=node_list.end())
					{
						if(_bridges.end()==find(_bridges.begin(),_bridges.end(),Bridge(k,it->first)))
						{
							++temp_group.edges_num;
							if(!it->second.visit)
							{
								_queue.push(it->first);
								it->second.visit = true;
							}
						}
					}
				}
			}
			temp_group.edges_num /= 2;
			temp_group.nodes_num = temp_group.GetNodes().size();
			temp_group.max_edges_num = temp_group.nodes_num * (temp_group.nodes_num-1)/2;
			
			add_inner_edges += temp_group.edges_num;
			add_max_inner_edges += temp_group.max_edges_num;
			groups.insert(pair<string,GroupWithMaxEdges>(group_id,temp_group));
		}	
	}
	for(iter=node_list.begin();iter!=node_list.end();++iter)
	{
		iter->second.visit = false;
	}
}

void CommunitiesWithMaxEdges::NetBfs(const vector<int> & NodeIds)
{
	int i,j,k,num=NodeIds.size();
	map<int,SimpleNode>::iterator iter,it;
	for(i=0;i<num;++i)
	{
		if(!node_list[NodeIds[i]].visit)
		{
			node_list[NodeIds[i]].visit = true;
			string new_group_id = GetGUID();
			GroupWithMaxEdges temp_group;
			queue<int> _queue;
			_queue.push(NodeIds[i]);
			while(!_queue.empty())
			{
				k = _queue.front();
				_queue.pop();
				temp_group.AddNode(k);
				//node_list[k].group_id = new_group_id;
				for(j=0;j<node_list[k].degree;++j)
				{
					if(NodeIds.end()!=find(NodeIds.begin(),NodeIds.end(),node_list[k].GetNears()[j]))
					{
						++temp_group.edges_num;
						if(!node_list[node_list[k].GetNears()[j]].visit)
						{
							node_list[node_list[k].GetNears()[j]].visit = true;
							_queue.push(node_list[k].GetNears()[j]);
						}
					}
				}
			}
			temp_group.edges_num /= 2;
			temp_group.nodes_num = temp_group.GetNodes().size();
			temp_group.max_edges_num = temp_group.nodes_num * (temp_group.nodes_num-1)/2;
			
			//inner_edges += temp_group.edges_num;//这里不单独计算总的内部边
			add_max_inner_edges += temp_group.max_edges_num;
			add_groups.insert(pair<string,GroupWithMaxEdges>(new_group_id,temp_group));
		}
	}
	for(i=0;i<num;++i)
	{
		node_list[NodeIds[i]].visit = false;
	}
}

void CommunitiesWithMaxEdges::NetBfs(const vector<int> & NodeIds,vector<Bridge> & _bridges)
{
	int i,j,k,num=NodeIds.size(),temp_node;
	map<int,SimpleNode>::iterator iter,it;
	for(i=0;i<num;++i)
	{
		if(!node_list[NodeIds[i]].visit)
		{
			node_list[NodeIds[i]].visit = true;
			string new_group_id = GetGUID();
			GroupWithMaxEdges temp_group;
			queue<int> _queue;
			_queue.push(NodeIds[i]);
			while(!_queue.empty())
			{
				k = _queue.front();
				_queue.pop();
				temp_group.AddNode(k);
				//node_list[k].group_id = new_group_id;
				for(j=0;j<node_list[k].degree;++j)
				{
					temp_node = node_list[k].GetNears()[j];
					if(_bridges.end()!=find(_bridges.begin(),_bridges.end(),Bridge(k,temp_node)))
					{
						continue;//如果是桥边,则不作处理
					}
					if(NodeIds.end()!=find(NodeIds.begin(),NodeIds.end(),temp_node))
					{
						++temp_group.edges_num;
						if(!node_list[temp_node].visit)
						{
							node_list[temp_node].visit = true;
							_queue.push(temp_node);
						}
					}
				}
			}

			temp_group.edges_num /= 2;
			temp_group.nodes_num = temp_group.GetNodes().size();
			temp_group.max_edges_num = temp_group.nodes_num * (temp_group.nodes_num-1)/2;
			
			//inner_edges += temp_group.edges_num;//这里不单独计算总的内部边
			add_max_inner_edges += temp_group.max_edges_num;
			add_groups.insert(pair<string,GroupWithMaxEdges>(new_group_id,temp_group));
		}
	}
	for(i=0;i<num;++i)
	{
		node_list[NodeIds[i]].visit = false;
	}
}

int CommunitiesWithMaxEdges::BigSetChange(int num)
{
	NetBfs(num);
	swap(groups,add_groups);
	swap(inner_edges,add_inner_edges);
	swap(max_inner_edges,add_max_inner_edges);
	cross_edges = total_edges - inner_edges;
	max_cross_edges = max_total_edges - max_inner_edges;
	return 3;
}

int CommunitiesWithMaxEdges::SmallSetChange(int select_node,vector<NodeCloseInfo> & nears_info,vector<Bridge> & _bridges)
{
	if(node_list[select_node].degree==0) return 0;//-----------//节点无邻居点,是孤立点,不作变化,直接返回

	int i;//---------------------------------------------------//循环控制变量i
	int diff_near_num = nears_info.size() - 1;//---------------//邻居群的数量
	
	if(diff_near_num==0)//-------------------------------------//无异群，只作有邻接割边时的小分解，不作大变化	
	{
		int total_same_nears = nears_info[0].close_num;//------//同群邻居点的数量
		if(total_same_nears==1) return 0;//--------------------//只有一个邻接点，不作处理，直接返回
		vector<Bridge> part_bridges;//-------------------------//部分割边集合
		for(i=0;i<total_same_nears;++i)
		{
			Bridge temp_bridge(select_node,nears_info[0].GetCloseNodes()[i]);//-------//缓存到边结构
			if(_bridges.end()!=find(_bridges.begin(),_bridges.end(),temp_bridge))//------//如果这条边是割边，则重新对选中点所在群分组
			{
				part_bridges.push_back(temp_bridge);
			}
		}
		int part_bridges_num = part_bridges.size();
		if(part_bridges_num>0)//---------------------------------//如果存在割边
		{
			select_group_id = node_list[select_node].group_id;
			select_group = groups.find(select_group_id)->second;//--//缓存选取点所在的群
			NetBfs(groups[select_group_id].GetNodes(),part_bridges);//重分组
			add_inner_edges -= part_bridges_num;//------------------//总的内部边减少量（只减不增）
			add_max_inner_edges -= groups[select_group_id].max_edges_num;//总的内部边减少量
			inner_edges += add_inner_edges;		
			max_inner_edges += add_max_inner_edges;
			cross_edges = total_edges - inner_edges;
			max_cross_edges = max_total_edges - max_inner_edges; //总的交叉边
			groups.erase(groups.find(select_group_id));//--------//移除原始群
			return 1;//------------------------------------------//返回1,意味着不可再做其它改动
		}
		else return 0;//-----------------------------------------//没有邻接割边，不作处理，直接返回
	}
	else//-------------------------------------------------------//有异群的，直接将这个点移动有最多邻接边的异群
	{
		int times = 1;//-----------------------------------------//初始化最多邻接边的异群为一个
		int max_edge_class = 1;//--------------------------------//初始化第一个遍历到的异群有最多邻接边
		vector<int> max_edge_nears;//----------------------------//存储具有最多邻接边异群的下标
		max_edge_nears.push_back(max_edge_class);//--------------//加入初始最大邻接异群	
		for(i=2;i<=diff_near_num;++i)//--------------------------//从第二个开始比较
		{
			if(nears_info[i].close_num>nears_info[max_edge_class].close_num)
			{
				max_edge_nears.clear();
				max_edge_nears.push_back(i);
				max_edge_class = i;
				times = 1;
			}
			else if(nears_info[i].close_num==nears_info[max_edge_class].close_num)
			{
				max_edge_nears.push_back(i);
				++times;
			}
		}
		if(times==1)//------------------------------------------//如果仅有一个最大异群，则将选中点转移到其中
		{
			trans_near_class = max_edge_class;
		}
		else//--------------------------------------------------//如果有多个最大异群，则转移到节点最多的群中
		{
			trans_near_class = max_edge_nears[0];
			for(i=1;i<times;++i)//------------------------------//找节点最多的最多边邻接群
			{
				if(groups[nears_info[trans_near_class].group_id].nodes_num
				   <groups[nears_info[max_edge_nears[i]].group_id].nodes_num) 
					trans_near_class = max_edge_nears[i];
			}
		}
		select_group_id = node_list[select_node].group_id;//--------//选取点所在的群ID
		select_group = groups.find(select_group_id)->second;//------//缓存选取点所在的群
		trans_group_id = nears_info[trans_near_class].group_id;//---//待转移群ID		
		groups[select_group_id].DelNode(select_node);//-------------//删除选取点
		NetBfs(groups[select_group_id].GetNodes());//---------------//选取点所在群重新分组--除去选取点后,groups[select_group_id].nodes_num=1这个情况较少不单独考虑 
		
		add_inner_edges += nears_info[trans_near_class].close_num - nears_info[0].close_num;//总群内边变化量
		add_max_inner_edges -= groups[select_group_id].max_edges_num;//最大内部边变化量
		add_max_inner_edges += groups[trans_group_id].nodes_num;//---//最大内部边增量
		inner_edges += add_inner_edges;
		max_inner_edges += add_max_inner_edges;
		cross_edges = total_edges - inner_edges;
		max_cross_edges = max_total_edges - max_inner_edges;//--------//当前交叉边
		groups.erase(groups.find(select_group_id));//-----------------//移除原始群
		return 2;
	}
}

void CommunitiesWithMaxEdges::AcceptChange(int select_node,vector<NodeCloseInfo> & nears_info,int ans)
{
	if(ans==0) return;
	map<string,GroupWithMaxEdges>::iterator it;
	if(ans==3){
		for(it=add_groups.begin();it!=add_groups.end();++it)
		{
			for(int i=0;i<it->second.nodes_num;++i)
				node_list[it->second.GetNodes()[i]].group_id = it->first;//更新节点群属
		}	
	}
	else{
		//groups.erase(groups.find(select_group_id));//--------------//移除原始群
		for(it=add_groups.begin();it!=add_groups.end();++it)
		{
			for(int i=0;i<it->second.nodes_num;++i)
			{
				node_list[it->second.GetNodes()[i]].group_id = it->first;//更新节点群属
			}
			groups.insert(*it);
		}
		if(ans==2)
		{
			node_list[select_node].group_id = trans_group_id;//----//更改选中点群ID
			groups[trans_group_id].max_edges_num += groups[trans_group_id].nodes_num;
			groups[trans_group_id].AddNode(select_node);//---------//增加选中点到待转移群
			groups[trans_group_id].nodes_num ++;//-----------------//节点总数增加1
			groups[trans_group_id].edges_num += nears_info[trans_near_class].close_num;//待转移群增加的群内边	
		}
	}
	add_groups.clear();
	add_inner_edges = 0;
	add_max_inner_edges = 0;
	trans_near_class = -1;
	trans_group_id = "";
	select_group_id = "";
}

void CommunitiesWithMaxEdges::RefuseChange(int select_node,int ans)
{
	if(ans==0) return;
	if(ans==3){
		swap(groups,add_groups);
		swap(inner_edges,add_inner_edges);
		swap(max_inner_edges,add_max_inner_edges);
	}else{
		inner_edges -= add_inner_edges;
		max_inner_edges -= add_max_inner_edges;
		groups.insert(pair<string,GroupWithMaxEdges>(select_group_id,select_group));//添加回删掉的群
	}
	cross_edges = total_edges - inner_edges;
	max_cross_edges = max_total_edges - max_inner_edges;

	add_groups.clear();
	add_inner_edges = 0;
	add_max_inner_edges = 0;
	trans_near_class = -1;
	trans_group_id = "";
	select_group_id = "";
}

int CommunitiesWithMaxEdges::CalMaxCrossEdges()//计算最多交叉边的函数，也可以用减法来计算
{
	int set_max_cross_edges = 0;
	map<string,GroupWithMaxEdges>::iterator i,j,k;
	for(i=groups.begin();i!=groups.end();)
	{
		for(k=i,j=++i;j!=groups.end();++j)
		{
			set_max_cross_edges += k->second.nodes_num * j->second.nodes_num;
		}
	}
	return set_max_cross_edges;
}

double CommunitiesWithMaxEdges::CalModularity()
{
	double Pn=1,Pe=1,Px=1;
	double EI=0,ETI=0;
	map<string,GroupWithMaxEdges>::iterator iter;
	for(iter=groups.begin();iter!=groups.end();++iter)
	{
		Pn *= pow((double)iter->second.nodes_num/total_nodes,iter->second.nodes_num);
		EI = iter->second.edges_num;
		ETI = iter->second.max_edges_num;
		if(ETI>=1) Pe *= pow(EI/ETI,EI) * pow((ETI-EI)/ETI,ETI-EI);
	}
	for(iter=add_groups.begin();iter!=add_groups.end();++iter)
	{
		Pn *= pow((double)iter->second.nodes_num/total_nodes,iter->second.nodes_num);
		EI = iter->second.edges_num;
		ETI = iter->second.max_edges_num;
		if(ETI>=1) Pe *= pow(EI/ETI,EI) * pow((ETI-EI)/ETI,ETI-EI);
	}
	double EX = cross_edges,ETX = max_cross_edges;
	if(ETX>=1) Px = pow(EX/ETX,EX) * pow((ETX-EX)/ETX,ETX-EX);
	double result = Pn*Pe*Px;
	return result;
	//if(result==1) return RAND_MAX;
	//return -1/log10(result);
}

string CommunitiesWithMaxEdges::GetGroupSetsStr()
{
	string bestSet = "最佳分群方式如下:\r\n";
	map<string,GroupWithMaxEdges>::iterator iter;
	for(iter=groups.begin();iter!=groups.end();++iter)
		bestSet += JointVector(iter->second.GetNodes())+"\r\n";
	return bestSet;
}

void CommunitiesWithMaxEdges::SaveGroupResult(CString path)//保存结果
{
	int index1 = path.ReverseFind('\\');
	int index2 = path.ReverseFind('.');

	CString folder = path.Mid(0,index1+1);
	CString f_name = path.Mid(index1+1,index2-index1-1);

	CString path1 = folder + f_name + "_uc.txt";
	CString path2 = folder + f_name + "_groups.txt";

	FILE* sFile;
	int count = 0,i;
	map<string,GroupWithMaxEdges>::iterator iter;

	if((sFile=fopen(path1,"w+"))==NULL) exit(1);
	fprintf(sFile,"Counts	");
	fprintf(sFile,"%d",groups.size());
	fprintf(sFile,"\n");
	for(iter=groups.begin();iter!=groups.end();++iter)
	{
		for(i=0;i<iter->second.nodes_num;++i)
		{
			fprintf(sFile,"%-8d",iter->second.GetNodes()[i]);
			fprintf(sFile,"%d",count);
			fprintf(sFile,"\n");
		}
		++count;
	}
	fclose(sFile); 

	if((sFile=fopen(path2,"w+"))==NULL) exit(1);
	count = 0;
	fprintf(sFile,"社团结构如下：\n\n");
	string group_str = "";
	for(iter=groups.begin();iter!=groups.end();++iter)
	{
		fprintf(sFile,"%d",count++);
		fprintf(sFile,"号社团：");
		group_str = JointVector(iter->second.GetNodes())+"\n";
		fprintf(sFile,group_str.c_str());
	}
	fclose(sFile); 
}

//---------注释不用-----------------------------------------------------------------------------//

/*void CommunitiesWithMaxEdges::LargeSetChange(int select_node,vector<NodeCloseInfo> & nears_info)
{
	int diff_near_num = nears_info.size() - 1;//------------------------------//邻居群的数量
	if(diff_near_num==0)
		return;
	else
	{
		string select_group_id = node_list[select_node].group_id;//------------//选中点所在的群
		int i,j;//-------------------------------------------------------------//循环控制变量i、j
		inner_edges -= groups[select_group_id].edges_num;
		max_inner_edges -= groups[select_group_id].max_edges_num;
		for(i=1;i<=diff_near_num;++i)
		{
			vector<int> group_nodes = groups[nears_info[i].group_id].GetNodes();
			for(j=0;j<group_nodes.size();++j)
			{
				groups[select_group_id].AddNode(group_nodes[j]);
				node_list[group_nodes[j]].group_id = select_group_id;
			}	
			inner_edges -= groups[nears_info[i].group_id].edges_num;
			max_inner_edges -= groups[nears_info[i].group_id].max_edges_num;
			groups.erase(groups.find(nears_info[i].group_id));//--------------//移除原始群群
		}	
		groups[select_group_id].nodes_num = groups[select_group_id].GetNodes().size();
		groups[select_group_id].edges_num = GroupEdges(groups[select_group_id].GetNodes());
		groups[select_group_id].max_edges_num = groups[select_group_id].nodes_num * (groups[select_group_id].nodes_num-1)/2;

		inner_edges += groups[select_group_id].edges_num;
		max_inner_edges += groups[select_group_id].max_edges_num;
		cross_edges = total_edges - inner_edges;
		max_cross_edges = max_total_edges - max_inner_edges;
	}
}*/