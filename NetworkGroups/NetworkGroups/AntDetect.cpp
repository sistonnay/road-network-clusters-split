#include "StdAfx.h"
#include "AntDetect.h"

AntDetect::AntDetect(void){}

AntDetect::~AntDetect(void){}

void AntDetect::InitParameters(const vector<vector<int>> & _pairs)
{
	int i,j;
	edges_num = 0;
	expects_num = 0;
	inner_edges = 0;
	nodes_num = _pairs.size();

	bridges = (new Algorithm(_pairs))->getBridges();

	for(i=0;i<nodes_num;++i)
	{
		node temp_node(_pairs[i]);
		edges_num += temp_node.degree;
		nodes_list.push_back(temp_node);	
		for(j=0;j<_pairs[i].size();++j)
		{
			if(i>_pairs[i][j])
			{
				edge_attr temp_edge(BridgeProperty(i,_pairs[i][j],_pairs));
				edges_attr.insert(pair<int,edge_attr>(i*(i-1)/2+_pairs[i][j],temp_edge));
			}
		}
	}

	edges_num /= 2;
	NetBfs(bridges);
	
	for(i=0;i<groups_list.size();++i)
		for(j=0;j<groups_list[i].size;++j)
			nodes_list[groups_list[i].nodes[j]].group_id = i;

	modularity = CalModularity();

	add_inner_edges = 0;
	add_expects_num = 0;
	add_groups_list.clear();
}

double AntDetect::BridgeProperty(int a,int b,const vector<vector<int>> & _pairs)
{
	int i,j,sum(0);
	int degree_a,degree_b,min_degree;
	degree_a = _pairs[a].size();
	degree_b = _pairs[b].size();
	for(i=0;i<degree_a;++i)
		for(j=0;j<degree_b;++j)
		{
			_pairs[a][i]==_pairs[b][j] ? ++sum : sum ;
		}
	degree_a < degree_b ? min_degree = degree_a - 1 : min_degree = degree_b -1 ;
	
	if(min_degree==0)
	{
		if(bridges.end()!=find(bridges.begin(),bridges.end(),Bridge(a,b)))//����������Ǹ��
			return 1.0;
		return 0.0;
	}
	return 1.0-(double)sum/min_degree;
}

double AntDetect::CalModularity()
{
	return (double)(inner_edges - (double)expects_num/edges_num/2)/edges_num;
}

int AntDetect::LocalModify(int ant_node,double sita,double temperate,vector<local_info> & _info)
{
	srand((unsigned)time(NULL));
	int i;//--------------------------------------------------------//ѭ�����Ʊ���i
	int diff_near_num = _info.size() - 1;//-------------------------//�ھ�Ⱥ������

	if(diff_near_num==0)//------------------------------------------//����Ⱥ,���ݱߵ��Ŷ���ֵ�ֽ�	
	{
		int total_same_nears = _info[0].part_degree;//--------------//ͬȺ�ھӵ������
		if(total_same_nears==1) return 0;//-------------------------//ֻ��һ���ڽӵ�,��������,ֱ�ӷ���

		int index;//------------------------------------------------//�����±�		
		vector<Bridge> part_bridges;//------------------------------//�ű߼���
		for(i=0;i<total_same_nears;++i)
		{
			ant_node > _info[0].boundarys[i] ? index = ant_node*(ant_node-1)/2 + _info[0].boundarys[i] : index = _info[0].boundarys[i]*(_info[0].boundarys[i]-1)/2 + ant_node ;
			if(edges_attr[index].f_bridge>sita)
			{
				Bridge temp_bridge(ant_node,_info[0].boundarys[i]);
				part_bridges.push_back(temp_bridge);
			}
		}

		if(part_bridges.size()>0)//----------------------------------//����������������ı�
		{
			local_group_id = nodes_list[ant_node].group_id;
			NetBfs(groups_list[local_group_id].nodes,part_bridges);	 //�ֲ��ط���
			if(add_groups_list.size()==1) return 0;//----------------//�ط���ʧ��

			deta_inner_edges = add_inner_edges - groups_list[local_group_id].edges;//�ܵ��ڲ���������ֻ��������
			deta_expects_num = add_expects_num - groups_list[local_group_id].expects;//�ܵ�������������ֻ��������
			deta_modularity = (double)(deta_inner_edges - (double)deta_expects_num/edges_num/2)/edges_num;
			
			if(deta_modularity>0) 
				return 1;//------------------------------------------//����1,��ζ�Ų������������Ķ�
			else
			{
				double rndx = rand()%1000/1000.0;//------------------//�����
				if(rndx>exp(deta_modularity/temperate)) //-----------//ģ���˻����
					return 1;//--------------------------------------//����1,��ζ�Ų����������Ķ�
				else 
					return 0;//--------------------------------------//��������ֱ�ӷ���0
			}
		}
		return 0;//--------------------------------------------------//��������ֱ�ӷ���0
	}
	else//-----------------------------------------------------------//����Ⱥ�ģ�ֱ�ӽ�������ƶ�������ڽӱߵ���Ⱥ
	{
		int max_edge_class = 1;//------------------------------------//��ʼ����һ������������Ⱥ������ڽӱ�
		for(i=2;i<=diff_near_num;++i)//------------------------------//�ӵڶ�����ʼ�Ƚ�
		{
			if(_info[i].part_degree>_info[max_edge_class].part_degree)
			{
				max_edge_class = i;
			}
		}
		
		//int trans_near_class;
		//int times = 1;//---------------------------------------------//��ʼ������ڽӱߵ���ȺΪһ��
		//int max_edge_class = 1;//------------------------------------//��ʼ����һ������������Ⱥ������ڽӱ�
		//vector<int> max_edge_nears;//--------------------------------//�洢��������ڽӱ���Ⱥ���±�
		//max_edge_nears.push_back(max_edge_class);//------------------//�����ʼ����ڽ���Ⱥ	
		//for(i=2;i<=diff_near_num;++i)//------------------------------//�ӵڶ�����ʼ�Ƚ�
		//{
		//	if(_info[i].part_degree>_info[max_edge_class].part_degree)
		//	{
		//		max_edge_nears.clear();
		//		max_edge_nears.push_back(i);
		//		max_edge_class = i;
		//		times = 1;
		//	}
		//	else if(_info[i].part_degree==_info[max_edge_class].part_degree)
		//	{
		//		max_edge_nears.push_back(i);
		//		++times;
		//	}
		//}
		//if(times==1)//----------------------------------------------//�������һ�������Ⱥ����ѡ�е�ת�Ƶ�����
		//{
		//	trans_near_class = max_edge_class;
		//}
		//else//------------------------------------------------------//����ж�������Ⱥ����ת�Ƶ��ڵ�����Ⱥ��
		//{
		//	trans_near_class = max_edge_nears[0];
		//	for(i=1;i<times;++i)//----------------------------------//�ҽڵ����������ڽ�Ⱥ
		//	{
		//		if(groups_list[_info[trans_near_class].group_id].size<groups_list[_info[max_edge_nears[i]].group_id].size) 
		//			trans_near_class = max_edge_nears[i];
		//	}
		//}

		local_group_id = nodes_list[ant_node].group_id;//-----------//ѡȡ�����ڵ�ȺID
		trans_group_id = _info[max_edge_class].group_id;//----------//��ת��ȺID

		groups_list[local_group_id].nodes.erase(find(groups_list[local_group_id].nodes.begin(),groups_list[local_group_id].nodes.end(),ant_node));//ɾ��ѡȡ��
		
		if(groups_list[local_group_id].size>1) 
			NetBfs(groups_list[local_group_id].nodes);//----------------//ѡȡ������Ⱥ���·���--��ȥѡȡ���,groups[select_group_id].nodes_num=1���������ٲ��������� 	
		
		extra_expects = GroupExpectsAdd(groups_list[trans_group_id].nodes,ant_node);//��ת��Ⱥ���ӵ�������
		add_expects_num += extra_expects;

		trans_nears_num = _info[max_edge_class].part_degree;
		local_nears_num = _info[0].part_degree;

		deta_inner_edges = trans_nears_num - local_nears_num;     //��Ⱥ�ڱ߱仯��
		deta_expects_num = add_expects_num - groups_list[local_group_id].expects; //�ܵ�����������;
		deta_modularity = (double)(deta_inner_edges - (double)deta_expects_num/edges_num/2)/edges_num;//ģ�������

		if(deta_modularity>0) 
			return 2;//-------------------------------------------//����2,��ζ�Ų����������Ķ�
		else
		{
			double rndx = rand()%1000/1000.0;//-------------------//�����
			if(rndx>exp(deta_modularity/temperate))  //-----------//ģ���˻����
				return 2;//---------------------------------------//����2,��ζ�Ų����������Ķ�
			else
			{
				groups_list[local_group_id].nodes.push_back(ant_node);//�ӻ���
				return 0;//---------------------------------------//��������ֱ�ӷ���0
			}
		}
	}
}

void AntDetect::AcceptModify(int & ant_node,int ans,double acc_pro,vector<local_info> & _info)
{
	int i,j,near_node,index;
	int group_size,add_group_size,temp_id;
	if(ans!=0)
	{
		inner_edges += deta_inner_edges;
		expects_num += deta_expects_num;
		group_size = groups_list.size();
		add_group_size = add_groups_list.size();
		if(ans==2)
		{
			nodes_list[ant_node].group_id = trans_group_id;//-----//����ѡ�е�ȺID
			groups_list[trans_group_id].nodes.push_back(ant_node);//����ѡ�е㵽��ת��Ⱥ
			groups_list[trans_group_id].expects += extra_expects; //���ӵ�������
			groups_list[trans_group_id].edges += trans_nears_num; //���ӵ�Ⱥ�ڱ�
			groups_list[trans_group_id].size ++;//----------------//�ڵ���������1
			
			if(add_group_size==0)//ֻ��һ���ڵ㣬ת�ƺ���һ��Ⱥ�������һ�������
			{
				if(local_group_id!=group_size-1)
				{
					groups_list[group_size-1].id = local_group_id;
					groups_list[local_group_id] = groups_list[group_size-1];
					for(j=0;j<groups_list[local_group_id].size;++j)
						nodes_list[groups_list[local_group_id].nodes[j]].group_id = local_group_id;
				}
				groups_list.pop_back();
			}
			else if(add_group_size==1)//-------------------------------//ʣ��ڵ�����ͨ
			{
				groups_list[local_group_id].size --;
				groups_list[local_group_id].edges = add_groups_list[0].edges;
				groups_list[local_group_id].expects = add_groups_list[0].expects;
			}else//-----------------------------------------------//ʣ��ڵ�ֽ�
			{
				add_groups_list[0].id = local_group_id;//---------//�����ӵĵ�һ��Ⱥ��ǩ��ΪԭȺ��ǩ
				groups_list[local_group_id] = add_groups_list[0];
				for(j=0;j<groups_list[local_group_id].size;++j)
					nodes_list[groups_list[local_group_id].nodes[j]].group_id = local_group_id;
				for(i=1;i<add_group_size;++i)
				{
					temp_id = group_size + i - 1;
					add_groups_list[i].id = temp_id;
					groups_list.push_back(add_groups_list[i]);
					for(j=0;j<groups_list[temp_id].size;++j)
						nodes_list[groups_list[temp_id].nodes[j]].group_id = temp_id;//���½ڵ�Ⱥ��
				}
			}

		}else
		{
			add_groups_list[0].id = local_group_id;//-------------//�����ӵĵ�һ��Ⱥ��ǩ��ΪԭȺ��ǩ
			groups_list[local_group_id] = add_groups_list[0];
			for(j=0;j<groups_list[local_group_id].size;++j)
				nodes_list[groups_list[local_group_id].nodes[j]].group_id = local_group_id;
			for(i=1;i<add_group_size;++i)
			{
				temp_id = group_size + i - 1;
				add_groups_list[i].id = temp_id;
				groups_list.push_back(add_groups_list[i]);
				for(j=0;j<groups_list[temp_id].size;++j)
					nodes_list[groups_list[temp_id].nodes[j]].group_id = temp_id;//���½ڵ�Ⱥ��
			}
		}
		//��Ϣ�ؼ�ǿ
		map<int,double>::iterator it;
		double p_f = acc_pro * exp(deta_modularity);
		
		for(i=0;i<nodes_list[ant_node].degree;++i)
		{
			near_node = nodes_list[ant_node].nears[i];
			ant_node > near_node ? index = ant_node*(ant_node-1)/2 + near_node : index = near_node*(near_node-1)/2 + ant_node ;
			it = add_pheromone.find(index);
			if(it!=add_pheromone.end())
				it->second += p_f;
			else
				add_pheromone.insert(pair<int,double>(index,p_f));
		}

	}

	//����ת��,�����̷�
	double sum(0.0);
	for(i=0;i<nodes_list[ant_node].degree;++i)
	{
		near_node = nodes_list[ant_node].nears[i];
		ant_node > near_node ? index = ant_node*(ant_node-1)/2 + near_node : index = near_node*(near_node-1)/2 + ant_node ;

		sum += 1.0/edges_attr[index].pheromone;
	}
	double p_sum(0.0),rndx = rand()%1000/1000.0;
	for(i=0;i<nodes_list[ant_node].degree;++i)
	{
		near_node = nodes_list[ant_node].nears[i];
		ant_node > near_node ? index = ant_node*(ant_node-1)/2 + near_node : index = near_node*(near_node-1)/2 + ant_node ;
		p_sum += 1.0/edges_attr[index].pheromone;
		if(p_sum >= rndx*sum)
		{
			ant_node = near_node;
			break;
		}
	}

	add_inner_edges = 0;
	add_expects_num = 0;
	add_groups_list.clear();
}

void AntDetect::UpdatePheromone(double acc_pro)
{
	map<int,edge_attr>::iterator iter;
	for(iter=edges_attr.begin();iter!=edges_attr.end();++iter)
	{
		iter->second.pheromone *= (1-acc_pro);
	}

	map<int,double>::iterator it;
	for(it=add_pheromone.begin();it!=add_pheromone.end();++it)
	{
		edges_attr[it->first].pheromone += it->second;
	}
	add_pheromone.clear();
}

void AntDetect::NetBfs(vector<Bridge> & _bridges)
{
	int i,j,k,it,num,count(0);
	num = nodes_list.size();
	for(i=0;i<num;++i)
	{
		if(!nodes_list[i].visit)
		{
			nodes_list[i].visit = true;
			group temp_group;
			temp_group.id = count++;
			queue<int> _queue;
			_queue.push(i);
			while(!_queue.empty())
			{
				k = _queue.front();
				_queue.pop();
				temp_group.nodes.push_back(k);
				for(j=0;j<nodes_list[k].degree;++j)
				{
					it = nodes_list[k].nears[j];
					if(_bridges.end()==find(_bridges.begin(),_bridges.end(),Bridge(k,it)))//�����ű�
					{
						if(!nodes_list[it].visit)
						{
							_queue.push(it);
							nodes_list[it].visit = true;
						}
					}
				}
			}
			temp_group.size = temp_group.nodes.size();
			temp_group.edges = GroupEdges(temp_group.nodes);
			temp_group.expects = GroupExpects(temp_group.nodes);
			inner_edges += temp_group.edges;
			expects_num += temp_group.expects;
			groups_list.push_back(temp_group);
		}
	}
	for(i=0;i<nodes_list.size();++i)
	{
		nodes_list[i].visit = false;
	}
}

void AntDetect::NetBfs(const vector<int> & NodeIds,vector<Bridge> & _bridges)
{
	int i,j,k,num,n,count(0);
	num=NodeIds.size();
	for(i=0;i<num;++i)
	{
		if(!nodes_list[NodeIds[i]].visit)
		{
			nodes_list[NodeIds[i]].visit = true;
			group temp_group;
			temp_group.id = count++;
			queue<int> _queue;
			_queue.push(NodeIds[i]);
			while(!_queue.empty())
			{
				k = _queue.front();
				_queue.pop();
				temp_group.nodes.push_back(k);
				for(j=0;j<nodes_list[k].degree;++j)
				{
					n = nodes_list[k].nears[j];
					if(_bridges.end()==find(_bridges.begin(),_bridges.end(),Bridge(k,n)))//�����ű�
					{
						if(NodeIds.end()!=find(NodeIds.begin(),NodeIds.end(),n))
						{
							if(!nodes_list[n].visit)
							{
								_queue.push(n);
								nodes_list[n].visit = true;	
							}
						}
					}
				}
			}
			temp_group.size = temp_group.nodes.size();
			temp_group.edges = GroupEdges(temp_group.nodes);
			temp_group.expects = GroupExpects(temp_group.nodes);
			add_inner_edges += temp_group.edges;
			add_expects_num += temp_group.expects;
			add_groups_list.push_back(temp_group);
		}
	}
	for(i=0;i<num;++i)
	{
		nodes_list[NodeIds[i]].visit = false;
	}
}

void AntDetect::NetBfs(const vector<int> & NodeIds)
{
	int i,j,k,num,n,count(0);
	num=NodeIds.size();
	for(i=0;i<num;++i)
	{
		if(!nodes_list[NodeIds[i]].visit)
		{
			nodes_list[NodeIds[i]].visit = true;
			group temp_group;
			temp_group.id = count++;
			queue<int> _queue;
			_queue.push(NodeIds[i]);
			while(!_queue.empty())
			{
				k = _queue.front();
				_queue.pop();
				temp_group.nodes.push_back(k);
				for(j=0;j<nodes_list[k].degree;++j)
				{
					n = nodes_list[k].nears[j];
					if(NodeIds.end()!=find(NodeIds.begin(),NodeIds.end(),n))
					{
						if(!nodes_list[n].visit)
						{
							_queue.push(n);
							nodes_list[n].visit = true;	
						}
					}
				}
			}
			temp_group.size = temp_group.nodes.size();
			temp_group.edges = GroupEdges(temp_group.nodes);
			temp_group.expects = GroupExpects(temp_group.nodes);
			add_expects_num += temp_group.expects;
			add_groups_list.push_back(temp_group);
		}
	}
	for(i=0;i<num;++i)
	{
		nodes_list[NodeIds[i]].visit = false;
	}
}

int AntDetect::GroupExpects(const vector<int> & NodesIds)
{
	int i,j,expects(0),num=NodesIds.size();
	for(i=0;i<num-1;++i)
	{
		for(j=i+1;j<num;++j)
		{
			expects += nodes_list[NodesIds[i]].degree * nodes_list[NodesIds[j]].degree;
		}
	}
	return expects;
}

int AntDetect::GroupExpectsAdd(const vector<int> & NodesIds,const int & ant_node)
{
	int i,num=NodesIds.size(),sum(0);
	for(i=0;i<num;++i)
	{
		sum += nodes_list[NodesIds[i]].degree;
	}
	return sum * nodes_list[ant_node].degree;
}

int AntDetect::GroupEdges(const vector<int> & NodesIds)
{
	int edges=0,num=NodesIds.size();
	for(int i=0;i<num;++i)
	{
		for(int j=0;j<nodes_list[NodesIds[i]].degree;++j)
		{
			if(NodesIds.end()!=find(NodesIds.begin(),NodesIds.end(),nodes_list[NodesIds[i]].nears[j]))
			{
				++edges;
			}
		}
	}
	edges /= 2;
	return edges;
}

void AntDetect::GetCloseInfo(vector<local_info> & nears,const int & select_node)
{
	bool close_vis;
	int i,j,k,near_id;
	int near_group_id;
	int this_group_id = nodes_list[select_node].group_id;	//��ǰ�ڵ����ڵ�Ⱥ
	local_info this_class;
	this_class.group_id = this_group_id;					//��ѡȡ������Ⱥ��Ŵ������
	nears.push_back(this_class);							//��һ��λ�ô��ͬȺ�ھ���Ϣ

	for(i=0;i<nodes_list[select_node].degree;++i)			//����select���ھӵ����
	{
		near_id = nodes_list[select_node].nears[i];			//�����ھӵ�
		near_group_id = nodes_list[near_id].group_id;		//��¼�ھӵ������Ⱥid

		if(near_group_id==this_group_id)					//ͬȺ�����
		{
			nears[0].boundarys.push_back(near_id);
		}
		else	//��Ⱥ�����
		{
			close_vis = false;
			for(j=1;j<nears.size();++j)
			{
				if(nears[j].group_id==near_group_id)
				{
					nears[j].boundarys.push_back(near_id);
					close_vis = true;
					break;
				}
			}
			if(!close_vis)
			{
				local_info temp_class;
				temp_class.group_id = near_group_id;
				temp_class.boundarys.push_back(near_id);
				nears.push_back(temp_class);
			}
		}
	}
	for(k=0;k<nears.size();++k)
	{
		nears[k].part_degree = nears[k].boundarys.size();
	}
}

void AntDetect::SaveGroupResult(CString path)//������
{
	int index1 = path.ReverseFind('\\');
	int index2 = path.ReverseFind('.');

	CString folder = path.Mid(0,index1+1);
	CString f_name = path.Mid(index1+1,index2-index1-1);

	CString path1 = folder + f_name + "_uc.txt";
	CString path2 = folder + f_name + "_groups.txt";

	FILE* sFile;
	int i,j;
	int groups_size = groups_list.size();

	if((sFile=fopen(path1,"w+"))==NULL) exit(1);
	fprintf(sFile,"Counts	");
	fprintf(sFile,"%d",groups_size);
	fprintf(sFile,"\n");
	for(i=0;i<groups_size;++i)
	{
		for(j=0;j<groups_list[i].size;++j)
		{
			fprintf(sFile,"%-8d",groups_list[i].nodes[j]);
			fprintf(sFile,"%d",i);
			fprintf(sFile,"\n");
		}
	}
	fclose(sFile); 

	if((sFile=fopen(path2,"w+"))==NULL) exit(1);

	fprintf(sFile,"����ģ��ȣ�\n\n");

	fprintf(sFile,"%.7f\n\n",CalModularity());

	fprintf(sFile,"���Žṹ���£�\n\n");

	string group_str = "";
	for(i=0;i<groups_size;++i)
	{
		fprintf(sFile,"%d",i);
		fprintf(sFile,"�����ţ�");
		group_str = JointVector(groups_list[i].nodes)+"\n";
		fprintf(sFile,group_str.c_str());
	}
	fclose(sFile); 
}

string AntDetect::JointVector(vector<int> vec)
{
	int num = vec.size();
	string str="( ";
	for(int i=0;i<num;++i)
	{
		char temp[8];
		itoa(vec[i],temp,10);
		str += " ";
		str += temp;
	}
	str += " )";
	return str;
}

string AntDetect::GetGroupSetsStr()
{
	string bestSet = "��ѷ�Ⱥ��ʽ����:\r\n";
	int i,groups_size = groups_list.size();
	for(i=0;i<groups_size;++i)
		bestSet += JointVector(groups_list[i].nodes)+"\r\n";
	return bestSet;
}