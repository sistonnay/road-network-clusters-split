#include "StdAfx.h"
#include "ExtendCommunicaties.h"

ExtendCommunicaties::ExtendCommunicaties(void)
{
	total_nodes = 0;
	total_edges = 0;
	modularity = 0;
}

ExtendCommunicaties::~ExtendCommunicaties(void){}

ExtendCommunicaties::ExtendCommunicaties(const vector<vector<int>> & _pairs,const vector<vector<int>> & _groups,const vector<int> _hiers)//自定义
{
	modularity=0;
	total_edges = 0;
	total_nodes = _pairs.size();
	node_list.clear();
	group_set.clear();
	low_hier = _hiers[0];
	//初始化节点的度、邻居
	for(int i=0;i<total_nodes;++i)
	{
		if(_hiers[i]>low_hier) low_hier = _hiers[i];
		ExtendNode tempNode(_pairs[i],_hiers[i]);
		total_edges += tempNode.degree;
		node_list.insert(pair<int,ExtendNode>(i,tempNode));
	}
	total_edges /= 2;
	//初始化社区、节点标签
	int num = _groups.size(),total;
	group_set = _groups;
	for(int i=0;i<num;++i)
	{
		total = _groups[i].size();
		for(int j=0;j<total;++j)
		{
			node_list[_groups[i][j]].group_ids.push_back(i);//节点标签初始化
		}
	}
	//获取节点邻居群数量
	int class_num;
	for(int i=0;i<total_nodes;++i)
	{
		GetCloseNum(class_num,i);
		node_list[i].near_c_num = class_num;
	}
	//获取边界节点集合
	group_graph.resize(num*(num-1)/2);	//初始化社团图存储空间
	for(int i=1;i<num;++i)
	{
		for(int j=0;j<i;++j)
		{
			map<int,ExtendBoundaryNode> temp;
			FindBoundaryNodes(i,group_set[i],j,group_set[j],temp);
			group_graph[i*(i-1)/2+j] = 0;
			if(temp.size()>0){
				boundary_nodes.insert(pair<int,map<int,ExtendBoundaryNode>>(i*(i-1)/2+j,temp));
				group_graph[i*(i-1)/2+j] = 1;
			}

		}
	}
	modularity = CalModularity();
	persistence = CalPersistenceRation();
}

ExtendCommunicaties::ExtendCommunicaties(const ExtendCommunicaties & other)
{
	total_edges = other.total_edges;
	total_nodes = other.total_nodes;
	modularity = other.modularity;
	node_list = other.node_list;
	group_set = other.group_set;
	boundary_nodes = other.boundary_nodes;
}

ExtendCommunicaties & ExtendCommunicaties::operator = (const ExtendCommunicaties & other)
{
	if(this!=&other)
	{
		total_edges = other.total_edges;
		total_nodes = other.total_nodes;
		modularity = other.modularity;
		node_list = other.node_list;
		group_set = other.group_set;
		boundary_nodes = other.boundary_nodes;
	}
	return *this;
}

void ExtendCommunicaties::GetCloseNum(int & nears_num,const int & select_node)
{
	bool close_vis;
	int i,j,near_id;
	int near_group_id;
	int this_group_id = node_list[select_node].group_ids[0];	//当前节点所在的群
	vector<int> nears;

	for(i=0;i<node_list[select_node].degree;++i)				//给点select的邻居点分组
	{
		near_id = node_list[select_node].nears[i];				//遍历邻居点
		near_group_id = node_list[near_id].group_ids[0];		//记录邻居点的所在群id

		if(near_group_id!=this_group_id)						//异群的情况
		{
			close_vis = false;
			for(j=1;j<nears.size();++j)
			{
				if(nears[j]==near_group_id)
				{
					close_vis = true;
					break;
				}
			}
			if(!close_vis)
			{
				nears.push_back(near_group_id);
			}
		}
	}
	nears_num = nears.size();
}

void ExtendCommunicaties::FindBoundaryNodes(int a,const vector<int> & group_a,int b,const vector<int> & group_b,map<int,ExtendBoundaryNode> & tempBNode)
{
	int i,j,n,ni,nj,gn,gj;

	for(i=0;i<group_a.size();++i)
	{
		ni = group_a[i];//当前节点
		for(j=0;j<node_list[ni].degree;++j)
		{
			nj = node_list[ni].nears[j];
			gj = node_list[nj].group_ids[0];
			if(gj==b)
			{
				node_list[ni].checked = true;//将点ni设置为边界点
				ExtendBoundaryNode temp_a;
				temp_a.group_id = a;
				temp_a.is_near = true;
				tempBNode.insert(pair<int,ExtendBoundaryNode>(ni,temp_a));

				if(!node_list[nj].checked)//如果该邻居节点不是边界点
				{
					node_list[nj].checked = true;//将点nj设置为边界点
					ExtendBoundaryNode temp_b;
					temp_b.group_id = b;
					temp_b.is_near = true;
					tempBNode.insert(pair<int,ExtendBoundaryNode>(nj,temp_b));
				}
			}
		}
	}

	map<int,ExtendBoundaryNode>::iterator iter;

	for(iter=tempBNode.begin();iter!=tempBNode.end();++iter)
	{
		n = iter->first;
		gn = iter->second.group_id;
		for(i=0;i<node_list[n].degree;++i)
		{
			nj = node_list[n].nears[i];
			gj = node_list[nj].group_ids[0];
			if(gj==gn)
			{
				iter->second.un_in_dgree +=1;
				if(node_list[nj].checked)
					iter->second.ref_nodes.push_back(nj);
			}
			else
			{
				if(gj==a||gj==b)
				{
					iter->second.un_out_degree += 1;
					iter->second.ref_nodes.push_back(nj);
				}
			}
		}
	}

	for(iter=tempBNode.begin();iter!=tempBNode.end();++iter)
	{
		n = iter->first;
		node_list[n].checked = false;
	}

}

void ExtendCommunicaties::OverLappingCluster(double sita)
{
	if(boundary_nodes.size()<0)
	{
		AfxMessageBox("there is a bug！");
		return;
	}
	double maxv,minv,ylow;
	bool b1,b2;
	int j,m1,m2,n,nj,max,min,gj,gmax,gmin,x,y;
	map<int,double>::iterator rt;
	map<int,double> temp_b_n,temp_nb_n;
	map<int,ExtendBoundaryNode>::iterator it;
	map<int,map<int,ExtendBoundaryNode>>::iterator iter;
	ylow = pow((double)low_hier,-2.7);
	for(iter=boundary_nodes.begin();iter!=boundary_nodes.end();++iter)
	{
		temp_b_n.clear();
		temp_nb_n.clear();
		b1 = b2 =false;
		for(it=iter->second.begin();it!=iter->second.end();++it)
		{ 
			temp_nb_n.insert(pair<int,double>(it->first,0.0));
			if(!b1) { m1 = it->second.group_id; b1 = true; }
			if(!b2) { m2 = it->second.group_id; if(m2!=m1) b2 = true; }//m1,m2是用来标记节点初始时属于哪个社团
		}
		//选取过程
		while(temp_nb_n.size()>0)
		{
			for(rt=temp_nb_n.begin();rt!=temp_nb_n.end();++rt)
			{
				n = rt->first;
				x = iter->second[n].un_in_dgree;
				y = iter->second[n].un_out_degree;
				if(x==0)
				{
					if(y==0) rt->second = 1.0;
					else rt->second = 0.0;
				}
				else
					rt->second = exp(1.21*(pow((double)node_list[n].hierachical,-2.7) - ylow)) * y / x;
			}
			max = map_max_element(temp_nb_n)->first;
			maxv = temp_nb_n[max];
			if(maxv < sita) break;
			gmax = iter->second[max].group_id;
			for(j=0;j<iter->second[max].ref_nodes.size();++j)
			{
				nj = iter->second[max].ref_nodes[j];
				gj = iter->second[nj].group_id;
				if(gmax==gj)
					iter->second[nj].un_in_dgree -=1;
				else
					iter->second[nj].un_out_degree -=1;
			}
			temp_b_n.insert(pair<int,double>(max,0.0));
			temp_nb_n.erase(temp_nb_n.find(max));
		}
		//删除过程
		while(temp_b_n.size()>0)
		{
			for(rt=temp_b_n.begin();rt!=temp_b_n.end();++rt)
			{
				n = rt->first;
				x = iter->second[n].un_in_dgree;
				y = iter->second[n].un_out_degree;
				if(x==0)
				{
					if(y==0) rt->second = 1.0;
					else rt->second = 0.0;
				}
				else
					rt->second = exp(1.21*(pow((double)node_list[n].hierachical,-2.7) - ylow)) * y / x;
			}
			min = map_min_element(temp_b_n)->first;
			minv = temp_b_n[min];
			if(minv >= sita) break;
			gmin = iter->second[min].group_id;
			for(j=0;j<iter->second[min].ref_nodes.size();++j)
			{
				nj = iter->second[min].ref_nodes[j];
				gj = iter->second[nj].group_id;
				if(gmin==gj)
					iter->second[nj].un_in_dgree +=1;
				else
					iter->second[nj].un_out_degree +=1;
			}
			temp_b_n.erase(temp_b_n.find(min));
		}
		//标记过程
		for(rt=temp_b_n.begin();rt!=temp_b_n.end();++rt)
		{
			n = iter->second[rt->first].group_id;
			if(n==m1)
			{
				node_list[rt->first].group_ids.push_back(m2);
				group_set[m2].push_back(rt->first);
			}
			else 
			{
				node_list[rt->first].group_ids.push_back(m1);
				group_set[m1].push_back(rt->first);
			}
		}
	}
	
	RefreshGroupGraph();//refresh group graph
	persistence_new = CalPersistenceRation();
	modularity_new = CalModularity();

}

void ExtendCommunicaties::RefreshGroupGraph()
{
	int i,j,k,num,n,a,b;
	num = node_list.size();
	for(k=0;k<num;++k)
	{
		n = node_list[k].group_ids.size();
		if(n>1)
		{
			for(i=1;i<n;++i)
			{
				a =  node_list[k].group_ids[i];
				for(j=0;j<i;++j)
				{
					b =  node_list[k].group_ids[j];
					if(a<b) swap(a,b);
					group_graph[a*(a-1)/2+b] = 1;
				}
			}
		}
	}
}

double ExtendCommunicaties::CalModularity()
{
	int g_num = group_set.size();
	int i,j,k,g_size;
	double sum(0.0),ajk;
	for(i=0;i<g_num;++i)
	{
		g_size = group_set[i].size();
		for(j=1;j<g_size;++j)
			for(k=0;k<j;++k)
			{
				ajk = 0;
				if(node_list[group_set[i][j]].nears.end()!=find(node_list[group_set[i][j]].nears.begin(),node_list[group_set[i][j]].nears.end(),group_set[i][k]))
				{
					ajk = 1;
				}
				sum +=(ajk-(double)node_list[group_set[i][j]].degree*node_list[group_set[i][k]].degree/(2*total_edges)) 
					/(node_list[group_set[i][j]].group_ids.size()*node_list[group_set[i][k]].group_ids.size()*total_edges);
			}
	}
	return sum;
}

double ExtendCommunicaties::CalPersistenceRation()
{
	bool flag;
	int i,j,a,b,n,m,p,q,dist;
	int num = group_set.size();
	int Num = num * (num-1) / 2;
	vector<int> s_nodes,g_nodes;
	vector<int> n_dists(Num,0);
	for(i=0;i<num;++i) g_nodes.push_back(i);
	//最短路径
	for(i=0;i<num;++i)
	{
		dist = 0;
		n = a =i;
		s_nodes = g_nodes;
		s_nodes.erase(find(s_nodes.begin(),s_nodes.end(),a));
		while(s_nodes.size()>0)
		{
			flag = false;
			for(j=0;j<s_nodes.size();++j)//还没找完的点
			{
				p = n;
				q  = b = s_nodes[j];
				if(p<q) swap(p,q);
				if(group_graph[p*(p-1)/2+q]==1)
				{
					m = b;
					p = a;
					if(p<b) swap(p,b);
					if((n_dists[p*(p-1)/2+b]>dist+1)||(n_dists[p*(p-1)/2+b]==0))
						n_dists[p*(p-1)/2+b] = dist+1;
					flag = true;//连通
				}
			}
			if(!flag)
			{
				dist = 0;
				n = a = s_nodes[0];
				s_nodes.erase(find(s_nodes.begin(),s_nodes.end(),a));
			}else
			{
				n = m;
				dist = dist+1;
				s_nodes.erase(find(s_nodes.begin(),s_nodes.end(),n));
			}
		}
	}
	//连通率
	double sum = 0.0;
	for(i=0;i<Num;++i) 
	{
		if(n_dists[i]!=0) 
			sum += 1.0/n_dists[i];
	}
	return (double)sum/Num;
}

string ExtendCommunicaties::JointVector(vector<int> vec)
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

void ExtendCommunicaties::SaveGroupResult(CString path,const vector<int> & color)//保存结果
{
	int index1 = path.ReverseFind('\\');
	int index2 = path.ReverseFind('.');

	CString folder = path.Mid(0,index1+1);
	CString f_extd = path.Mid(index2);
	CString f_name = path.Mid(index1+1,index2-index1-1);

	CString path_hiers = folder + f_name + "_hiers" + f_extd;
	CString path_group = folder + f_name + "_groupSets.txt";
	CString path_cluster = folder + f_name + "_clusters" + f_extd;
	CString path_colour_list = folder + f_name + "_colourList" + f_extd;
	CString path_cluster_num = folder + f_name + "_clusterNum" + f_extd;

	FILE* sFile;
	int i,j;

	//////////////////////////////////////////////////////
	if((sFile=fopen(path_hiers,"w+"))==NULL) exit(1);
	fprintf(sFile,"id,hier\n");
	for(i=0;i<node_list.size();++i)
	{
		fprintf(sFile,"%d,",i);
		fprintf(sFile,"%d\n",node_list[i].hierachical);
	}
	fclose(sFile);

	///////////////////////////////////////////////////////
	if((sFile=fopen(path_group,"w+"))==NULL) exit(1);
	vector<int> hier_total(low_hier,0);
	vector<int> hier_c_nums(low_hier,0),hier_o_nums(low_hier,0);
	for(i=0;i<total_nodes;++i)
	{
		j = node_list[i].hierachical-1;
		if(j<0) continue;
		hier_total[j] += 1;
		hier_c_nums[j] += node_list[i].near_c_num;
		hier_o_nums[j] += node_list[i].group_ids.size() - 1;
	}
	fprintf(sFile,"模块度值：非重叠 %.7f，",modularity);
	fprintf(sFile,"重叠 %.7f\n\n",modularity_new);
	fprintf(sFile,"连通密度：非重叠 %.7f，",persistence);
	fprintf(sFile,"重叠 %.7f\n\n",persistence_new);
	fprintf(sFile,"层次 节点数 总邻接社团数 平均邻接社团数 总重叠社团数 平均重叠社团数\n");
	for(i=0;i<low_hier;++i)
	{
		fprintf(sFile,"  %-5d",i+1);
		fprintf(sFile,"%-7d",hier_total[i]);
		fprintf(sFile,"%-13d",hier_c_nums[i]);
		fprintf(sFile,"%-15.7f",(double)hier_c_nums[i]/hier_total[i]);
		fprintf(sFile,"%-13d",hier_o_nums[i]);
		fprintf(sFile,"%-15.7f\n",(double)hier_o_nums[i]/hier_total[i]);
	}
	fprintf(sFile,"\n社团结构如下：\n");
	string group_str = "";
	for(i=0;i<group_set.size();++i)
	{
		fprintf(sFile,"%d号社团：",i);
		group_str = JointVector(group_set[i])+"\n";
		fprintf(sFile,group_str.c_str());
	}
	fclose(sFile); 

	///////////////////////////////////////////////////////
	if((sFile=fopen(path_cluster,"w+"))==NULL) exit(1);
	fprintf(sFile,"id,cluster\n");
	for(i=0;i<node_list.size();++i)
	{
		fprintf(sFile,"%d",i);
		fprintf(sFile,",\"");
		for(j=0;j<node_list[i].group_ids.size()-1;++j)
		{
			fprintf(sFile,"%d",node_list[i].group_ids[j]);
			fprintf(sFile,",");
		}
		fprintf(sFile,"%d",node_list[i].group_ids[j]);
		fprintf(sFile,"\"");
		fprintf(sFile,"\n");
	}
	fclose(sFile);

	///////////////////////////////////////////////////////
	if((sFile=fopen(path_colour_list,"w+"))==NULL) exit(1);
	fprintf(sFile,"id,colourList\n");
	for(i=0;i<node_list.size();++i)
	{
		fprintf(sFile,"%d",i);
		fprintf(sFile,",\"");
		for(j=0;j<node_list[i].group_ids.size()-1;++j)
		{
			fprintf(sFile,"%d,",color[node_list[i].group_ids[j]]);
		}
		fprintf(sFile,"%d\"\n",color[node_list[i].group_ids[j]]);
	}
	fclose(sFile); 

	/////////////////////////////////////////////////////////////
	if((sFile=fopen(path_cluster_num,"w+"))==NULL) exit(1);
	fprintf(sFile,"id,cluster_num\n");
	for(i=0;i<node_list.size();++i)
	{
		fprintf(sFile,"%d,",i);
		fprintf(sFile,"%d\n",node_list[i].group_ids.size());
	}
	fclose(sFile);

}

