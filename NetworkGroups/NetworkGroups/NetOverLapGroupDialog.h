#pragma once

#include <vector>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "afxwin.h"
using namespace std;

class NetOverLapGroupDialog : public CDialog
{

	DECLARE_DYNAMIC(NetOverLapGroupDialog)

public:

	NetOverLapGroupDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~NetOverLapGroupDialog();
	
	// 对话框数据
	enum { IDD = IDD_OVERLAPPING_DIALOG };

	void CONAOptimizing();

private:

	int pairs_num;
	int edges_num;
	int edges_max;

	vector<vector<int>> pairs;
	vector<vector<int>> groups;
	vector<int> hiers;
	vector<int> colors;

	CString pairs_path;
	CString group_path;
	CString hiera_path;
	CString save_path;
	CString color_path;

	double yuzhi;

public:

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedButton5();
};
