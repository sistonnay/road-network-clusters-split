#include "StdAfx.h"
#include "BaseCommunities.h"

BaseCommunities::BaseCommunities(void)
{
	total_nodes = 0;
	inner_edges = 0;
	total_edges = 0;
	add_inner_edges = 0;
	select_group_id = "";
	trans_group_id = "";
	trans_near_class = -1;
}

BaseCommunities::~BaseCommunities(void){}

BaseCommunities::BaseCommunities(const BaseCommunities & other)
{
	total_edges = other.total_edges;
	total_nodes = other.total_nodes;
	inner_edges = other.inner_edges;
	add_inner_edges = other.add_inner_edges;
	select_group_id = other.select_group_id;
	trans_group_id = other.trans_group_id;
	trans_near_class = other.trans_near_class;
	node_list = other.node_list;
}

BaseCommunities & BaseCommunities::operator = (const BaseCommunities & other)
{
	if(this!=&other)
	{
		total_edges = other.total_edges;
		total_nodes = other.total_nodes;
		inner_edges = other.inner_edges;
		add_inner_edges = other.add_inner_edges;
		select_group_id = other.select_group_id;
		trans_group_id = other.trans_group_id;
		trans_near_class = other.trans_near_class;
		node_list = other.node_list;
	}
	return *this;
}

string BaseCommunities::GetGUID()
{
	GUID m_guid;
	#ifdef WIN32
		CoCreateGuid( &m_guid);
	#else
		uuid_generate(reinterpret_cast<unsigned char *>( &m_guid));
	#endif
 
	char buf[64] = {0};

	#ifdef __GNUC__
		snprintf(
	#else // MSVC
		_snprintf_s(
	#endif

	buf,sizeof(buf),"{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}",
	m_guid.Data1, m_guid.Data2, m_guid.Data3,
	m_guid.Data4[0], m_guid.Data4[1],m_guid.Data4[2], m_guid.Data4[3],
	m_guid.Data4[4], m_guid.Data4[5],m_guid.Data4[6], m_guid.Data4[7]);
       
	return string(buf);
}

void BaseCommunities::GetCloseInfo(vector<NodeCloseInfo> & nears,const int & select_node)
{
	bool close_vis;
	int i,j,k,near_id;
	string near_group_id;
	string this_group_id = node_list[select_node].group_id;	//当前节点所在的群
	NodeCloseInfo this_class;
	this_class.group_id = this_group_id;					//将选取点所在群编号存入对象
	nears.push_back(this_class);							//第一个位置存放同群邻居信息

	for(i=0;i<node_list[select_node].degree;++i)			//给点select的邻居点分组
	{
		near_id = node_list[select_node].GetNears()[i];		//遍历邻居点
		near_group_id = node_list[near_id].group_id;		//记录邻居点的所在群id

		if(near_group_id==this_group_id)					//同群的情况
		{
			nears[0].AddCloseNode(near_id);
		}
		else	//异群的情况
		{
			close_vis = false;
			for(j=1;j<nears.size();++j)
			{
				if(nears[j].group_id==near_group_id)
				{
					nears[j].AddCloseNode(near_id);
					close_vis = true;
					break;
				}
			}
			if(!close_vis)
			{
				NodeCloseInfo temp_class;
				temp_class.group_id = near_group_id;
				temp_class.AddCloseNode(near_id);
				nears.push_back(temp_class);
			}
		}
	}
	for(k=0;k<nears.size();++k)
	{
		nears[k].close_num = nears[k].GetCloseNodes().size();
	}
}

int BaseCommunities::GroupEdges(const vector<int> & NodesIds)
{
	int edges=0,num=NodesIds.size();
	for(int i=0;i<num;++i)
	{
		for(int j=0;j<node_list[NodesIds[i]].degree;++j)
		{
			if(NodesIds.end()!=find(NodesIds.begin(),NodesIds.end(),node_list[NodesIds[i]].GetNears()[j]))
			{
				++edges;
			}
		}
	}
	edges /= 2;
	return edges;
}
 
string BaseCommunities::JointVector(vector<int> vec)
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