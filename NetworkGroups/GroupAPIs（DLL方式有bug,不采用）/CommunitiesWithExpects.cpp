//
#ifndef DLL_FILE
#define DLL_FILE
#endif

#include "CommunitiesWithExpects.h"

CommunitiesWithExpects::CommunitiesWithExpects(void)
{
	inner_expects = 0;
}

CommunitiesWithExpects::~CommunitiesWithExpects(void){}

CommunitiesWithExpects::CommunitiesWithExpects(const CommunitiesWithExpects & other):BaseCommunities(other)
{
	inner_expects = other.inner_expects;
	groups = other.groups;
}

CommunitiesWithExpects & CommunitiesWithExpects::operator = (const CommunitiesWithExpects & other)
{
	if(this!=&other)
	{
		BaseCommunities::operator=(other);
		inner_expects = other.inner_expects;
		groups = other.groups;
	}
	return *this;
}

void CommunitiesWithExpects::InitSetByNumber(const vector<vector<int>> & _pairs,int _groups_num)
{
	inner_edges = 0;
	total_edges = 0;
	inner_expects = 0;
	total_nodes = _pairs.size();

	if(!(_groups_num<=total_nodes && _groups_num>0)) throw new exception("����ķ�������");
	for(int i=0;i<total_nodes;++i)
	{
		SimpleNode temp(_pairs[i]);
		total_edges += temp.degree;
		node_list.insert(pair<int,SimpleNode>(i,temp));	
	}
	total_edges /= 2;
	NetBfs(_groups_num);
}

void CommunitiesWithExpects::InitSetByBridge(const vector<vector<int>> & _pairs,vector<Bridge> & _bridges)
{
	inner_edges = 0;
	total_edges = 0;
	inner_expects = 0;
	total_nodes = _pairs.size();

	for(int i=0;i<total_nodes;++i)
	{
		SimpleNode temp(_pairs[i]);
		total_edges += temp.degree;
		node_list.insert(pair<int,SimpleNode>(i,temp));
	}
	total_edges /= 2;
	NetBfs(_bridges);

}

void CommunitiesWithExpects::NetBfs(int _groups_num)
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
			GroupWithExpects temp_group;
			string group_id = GetGUID();
			queue<int> _queue;
			_queue.push(iter->first);
			while(!_queue.empty())
			{
				k = _queue.front();
				_queue.pop();
				temp_group.AddNode(k);
				node_list[k].group_id = group_id;
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
			temp_group.expects_num = GroupExpects(temp_group.GetNodes());
			
			inner_edges += temp_group.edges_num;
			inner_expects += temp_group.expects_num;
			groups.insert(pair<string,GroupWithExpects>(group_id,temp_group));
		}
	}
	for(iter=node_list.begin();iter!=node_list.end();++iter)
	{
		iter->second.visit = false;
	}
}

void CommunitiesWithExpects::NetBfs(vector<Bridge> & _bridges)
{
	int j,k;
	map<int,SimpleNode>::iterator iter,it;
	for(iter=node_list.begin();iter!=node_list.end();++iter)
	{
		if(!iter->second.visit)
		{
			iter->second.visit = true;
			GroupWithExpects temp_group;
			string group_id = GetGUID();
			queue<int> _queue;
			_queue.push(iter->first);
			while(!_queue.empty())
			{
				k = _queue.front();
				_queue.pop();
				temp_group.AddNode(k);
				node_list[k].group_id = group_id;
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
			temp_group.expects_num = GroupExpects(temp_group.GetNodes());
			
			inner_edges += temp_group.edges_num;
			inner_expects += temp_group.expects_num;
			groups.insert(pair<string,GroupWithExpects>(group_id,temp_group));
		}	
	}
	for(iter=node_list.begin();iter!=node_list.end();++iter)
	{
		iter->second.visit = false;
	}
}

void CommunitiesWithExpects::NetBfs(const vector<int> & NodeIds)
{
	int i,j,k,num=NodeIds.size();
	map<int,SimpleNode>::iterator iter,it;
	for(i=0;i<num;++i)
	{
		if(!node_list[NodeIds[i]].visit)
		{
			node_list[NodeIds[i]].visit = true;
			string new_group_id = GetGUID();
			GroupWithExpects temp_group;
			queue<int> _queue;
			_queue.push(NodeIds[i]);
			while(!_queue.empty())
			{
				k = _queue.front();
				_queue.pop();
				temp_group.AddNode(k);
				node_list[k].group_id = new_group_id;
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
			temp_group.expects_num = GroupExpects(temp_group.GetNodes());
			
			//inner_edges += temp_group.edges_num;//���ﲻ���������ܵ��ڲ���
			inner_expects += temp_group.expects_num;
			groups.insert(pair<string,GroupWithExpects>(new_group_id,temp_group));
		}
	}
	for(i=0;i<num;++i)
	{
		node_list[NodeIds[i]].visit = false;
	}
}

void CommunitiesWithExpects::NetBfs(const vector<int> & NodeIds,vector<Bridge> & _bridges)
{
	int i,j,k,num=NodeIds.size(),temp_node;
	map<int,SimpleNode>::iterator iter,it;
	for(i=0;i<num;++i)
	{
		if(!node_list[NodeIds[i]].visit)
		{
			node_list[NodeIds[i]].visit = true;
			string new_group_id = GetGUID();
			GroupWithExpects temp_group;
			queue<int> _queue;
			_queue.push(NodeIds[i]);
			while(!_queue.empty())
			{
				k = _queue.front();
				_queue.pop();
				temp_group.AddNode(k);
				node_list[k].group_id = new_group_id;
				for(j=0;j<node_list[k].degree;++j)
				{
					temp_node = node_list[k].GetNears()[j];
					if(_bridges.end()!=find(_bridges.begin(),_bridges.end(),Bridge(k,temp_node)))
					{
						continue;//������ű�,��������
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
			temp_group.expects_num = GroupExpects(temp_group.GetNodes());
			
			//inner_edges += temp_group.edges_num;//���ﲻ���������ܵ��ڲ���
			inner_expects += temp_group.expects_num;
			groups.insert(pair<string,GroupWithExpects>(new_group_id,temp_group));
		}
	}
	for(i=0;i<num;++i)
	{
		node_list[NodeIds[i]].visit = false;
	}
}

void CommunitiesWithExpects::BigSetChange(int num)
{
	inner_edges = 0;
	inner_expects = 0;
	groups.clear();
	NetBfs(num);
}

void CommunitiesWithExpects::LargeSetChange(int select_node,vector<NodeCloseInfo> & nears_info)
{
	int diff_near_num = nears_info.size() - 1;//------------------------------//�ھ�Ⱥ������
	if(diff_near_num==0)
		return;
	else
	{
		string select_group_id = node_list[select_node].group_id;//------------//ѡ�е����ڵ�Ⱥ
		int i,j;//-------------------------------------------------------------//ѭ�����Ʊ���i��j
		inner_edges -= groups[select_group_id].edges_num;
		inner_expects -= groups[select_group_id].expects_num;
		for(i=1;i<=diff_near_num;++i)
		{
			vector<int> group_nodes = groups[nears_info[i].group_id].GetNodes();
			for(j=0;j<group_nodes.size();++j)
			{
				groups[select_group_id].AddNode(group_nodes[j]);
				node_list[group_nodes[j]].group_id = select_group_id;
			}	
			inner_edges -= groups[nears_info[i].group_id].edges_num;
			inner_expects -= groups[nears_info[i].group_id].expects_num;
			groups.erase(groups.find(nears_info[i].group_id));//--------------//�Ƴ�ԭʼȺȺ
		}	
		groups[select_group_id].nodes_num = groups[select_group_id].GetNodes().size();
		groups[select_group_id].edges_num = GroupEdges(groups[select_group_id].GetNodes());
		groups[select_group_id].expects_num = GroupExpects(groups[select_group_id].GetNodes());

		inner_edges += groups[select_group_id].edges_num;
		inner_expects += groups[select_group_id].expects_num;
	}
}

int CommunitiesWithExpects::SmallSetChange(int select_node,vector<NodeCloseInfo> & nears_info,vector<Bridge> & _bridges)
{
	if(node_list[select_node].degree==0) return 0;//-----------//�ڵ����ھӵ�,�ǹ�����,�����仯,ֱ�ӷ���

	int i;//---------------------------------------------------//ѭ�����Ʊ���i
	string select_group_id;//----------------------------------//ѡ�е����ڵ�Ⱥ
	int total_same_nears = nears_info[0].close_num;//----------//ͬȺ�ھӵ������
	int diff_near_num = nears_info.size() - 1;//---------------//�ھ�Ⱥ������
	
	if(diff_near_num==0)//-------------------------------------//����Ⱥ��ֻ�����ڽӸ��ʱ��С�ֽ⣬������仯	
	{
		if(nears_info[0].close_num==1) return 0;//-------------//ֻ��һ���ڽӵ㣬��������ֱ�ӷ���
		vector<Bridge> part_bridges;//-------------------------//���ָ�߼���
		for(i=0;i<nears_info[0].close_num;++i)
		{
			Bridge temp_bridge(select_node,nears_info[0].GetCloseNodes()[i]);//-------//���浽�߽ṹ
			if(_bridges.end()!=find(_bridges.begin(),_bridges.end(),temp_bridge))//------//����������Ǹ�ߣ������¶�ѡ�е�����Ⱥ����
			{
				part_bridges.push_back(temp_bridge);
			}
		}
		int part_bridges_num = part_bridges.size();
		if(part_bridges_num>0)//---------------------------------//������ڸ��
		{
			select_group_id = node_list[select_node].group_id;
			NetBfs(groups[select_group_id].GetNodes(),part_bridges);//�ط���
			inner_edges -= part_bridges_num;//-------------------//�ܵ��ڲ��߼�������ֻ��������
			inner_expects -= groups[select_group_id].expects_num;//�ܵ������߼�����
			groups.erase(groups.find(select_group_id));//--------//�Ƴ�ԭʼȺȺ
		}
		return 1;//----------------------------------------------//����1,��ζ�Ų������������Ķ�
	}
	else//-------------------------------------------------------//����Ⱥ�ģ�ֱ�ӽ�������ƶ�������ڽӱߵ���Ⱥ
	{
		int times = 1;//-----------------------------------------//��ʼ������ڽӱߵ���ȺΪһ��
		int max_edge_class = 1;//--------------------------------//��ʼ����һ������������Ⱥ������ڽӱ�
		vector<int> max_edge_nears;//----------------------------//�洢��������ڽӱ���Ⱥ���±�
		max_edge_nears.push_back(max_edge_class);//--------------//�����ʼ����ڽ���Ⱥ	
		for(i=2;i<=diff_near_num;++i)//--------------------------//�ӵڶ�����ʼ�Ƚ�
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

		int trans_near_class;//---------------------------------//��ת�Ƶ���Ⱥ�±�
		if(times==1)//------------------------------------------//�������һ�������Ⱥ����ѡ�е�ת�Ƶ�����
		{
			trans_near_class = max_edge_class;
		}
		else//--------------------------------------------------//����ж�������Ⱥ����ת�Ƶ��ڵ�����Ⱥ��
		{
			trans_near_class = max_edge_nears[0];
			for(i=1;i<times;++i)//------------------------------//�ҽڵ����������ڽ�Ⱥ
			{
				if(groups[nears_info[trans_near_class].group_id].nodes_num
				   <groups[nears_info[max_edge_nears[i]].group_id].nodes_num) 
					trans_near_class = max_edge_nears[i];
			}
		}
		
		select_group_id = node_list[select_node].group_id;//----//ѡȡ�����ڵ�ȺID
		groups[select_group_id].DelNode(select_node);//---------//ɾ��ѡȡ��
		NetBfs(groups[select_group_id].GetNodes());//-----------//ѡȡ������Ⱥ���·���--��ȥѡȡ���,groups[select_group_id].nodes_num=1���������ٲ��������� 
		inner_edges += nears_info[trans_near_class].close_num - nears_info[0].close_num;///��Ⱥ�ڱ߱仯��
		inner_expects -= groups[select_group_id].expects_num;////������ֵ�仯��
		groups.erase(groups.find(select_group_id));//----------//�Ƴ�ѡȡ������Ⱥ

		string trans_group_id = nears_info[trans_near_class].group_id;//��ת��ȺID
		int add_expects;//-------------------------------------//��Ⱥ���ӵ�������
		add_expects = GroupExpectsAdd(groups[trans_group_id].GetNodes(),select_node);
		inner_expects += add_expects;//------------------------//�ܵ�������������
		node_list[select_node].group_id = trans_group_id;//----//����ѡ�е�ȺID

		groups[trans_group_id].expects_num += add_expects;
		groups[trans_group_id].AddNode(select_node);//---------//����ѡ�е㵽��ת��Ⱥ
		groups[trans_group_id].nodes_num ++;//-----------------//�ڵ���������1
		groups[trans_group_id].edges_num += nears_info[trans_near_class].close_num;//��ת��Ⱥ���ӵ�Ⱥ�ڱ�

		return 2;
	}
}

int CommunitiesWithExpects::GroupExpectsAdd(const vector<int> & NodesIds,const int & trans_node)
{
	int i,num=NodesIds.size(),sum(0);
	for(i=0;i<num;++i)
	{
		sum += node_list[NodesIds[i]].degree;
	}
	return sum * node_list[trans_node].degree;
}

int CommunitiesWithExpects::GroupExpects(const vector<int> & NodesIds)
{
	int i,j,expects(0),num=NodesIds.size();
	for(i=0;i<num-1;++i)
	{
		for(j=i+1;j<num;++j)
		{
			expects += node_list[NodesIds[i]].degree * node_list[NodesIds[j]].degree;
		}
	}
	return expects;
}

double CommunitiesWithExpects::CalModularity()
{
	return (double)(inner_edges - (double)inner_expects/total_edges/2)/total_edges;
}

string CommunitiesWithExpects::GetGroupSetsStr()
{
	string bestSet = "��ѷ�Ⱥ��ʽ����:\r\n";
	map<string,GroupWithExpects>::iterator iter;
	for(iter=groups.begin();iter!=groups.end();++iter)
		bestSet += JointVector(iter->second.GetNodes())+"\r\n";
	return bestSet;
}