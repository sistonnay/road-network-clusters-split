#pragma once

#include <vector>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "afxwin.h"
using namespace std;

class NetGroupDialog : public CDialog
{
	DECLARE_DYNAMIC(NetGroupDialog)

public:

	NetGroupDialog(CWnd* pParent = NULL);   //标准构造函数
	virtual ~NetGroupDialog();
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedOk();
	afx_msg void OnCbnSelchangeComSolution();

public:// 对话框数据
	
	enum { IDD = IDD_NETGROUP_DIALOG };
	CComboBox Solution;
	CComboBox Objection;
	BOOL isExport;
	BOOL isWalk;
	CString SavePath;
	CString p_Str_1;
	CString p_Str_2;
	CString p_Str_3;
	int p_Num_1;
	int p_Num_2;
	float p_Prob;

private:

	int pairs_num;
	int edges_num;
	int edges_max;
	vector<int> nodes;
	vector<vector<int>> pairs;
	CString filePath;
	CString fileName;
	CString state_str;
	CString fileSave;
	int modularity;

public:

	void BaseOptimizing1();//简单模块度优化-标准模块度
	void BaseOptimizing2();//简单模块度优化-自定义分群指标
	void NACAOptimizing1();//蚁群算法优化-标准模块度
	void NACAOptimizing2();//蚁群算法优化-自定义分群指标
	void MCMCOptimizing1();//模拟退火优化-标准模块度
	void MCMCOptimizing2();//模拟退火优化-自定义分群指标
	void SABAOptimizing1();//局部蚁群优化-标准模块度
	void ReadPairsTable(const char* pathname);
	void ShowResult(vector<double> bests,CString result,CString title);

	HBITMAP DrawNacaResult(vector<double> bests,CString result,CDC* pDC);
	HBITMAP MemToBitmap(HDC hScrDC,int nX,int nY,int nWidth,int nHeight);

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedExportAns();
	afx_msg void OnBnClickedSavebtn();

protected:
	CWinThread* pThread;

};

UINT ThreadFunc(LPVOID lpParam);//多线程