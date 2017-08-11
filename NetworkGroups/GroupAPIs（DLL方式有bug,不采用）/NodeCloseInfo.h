#pragma once

#ifndef NODECLOSEINFO_H
#define NODECLOSEINFO_H

#ifdef DLL_FILE
#define EX_IM_PORT _declspec(dllexport) //导出类
#else
#define EX_IM_PORT _declspec(dllimport) //导入类
#endif

#include <vector>
#include <string>
using namespace std;

class EX_IM_PORT NodeCloseInfo //某临近群相接点信息
{

public:

	NodeCloseInfo(void); //默认构造函数
	~NodeCloseInfo(void);//析构函数

	NodeCloseInfo(const NodeCloseInfo & other);//拷贝构造函数
	NodeCloseInfo & operator = (const NodeCloseInfo & other);//赋值函数
	
	void AddCloseNode(int _node);//添加某临近群相接点
	const vector<int> & GetCloseNodes();//获取某临近群相接点集合

public:

	string group_id;//群ID
	int close_num;	//相接点总数

private:
	
	vector<int> close_nodes;//相接点集合

};

#endif