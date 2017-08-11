#pragma once

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;
// ConvertPairs 对话框

struct edge{
	int a;
	int b;
};

class ConvertPairs : public CDialogEx
{
	DECLARE_DYNAMIC(ConvertPairs)

public:
	ConvertPairs(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~ConvertPairs();

// 对话框数据
	enum { IDD = IDD_FORMAT_DIALOG };
public:

	CString input_path;
	CString output_path;
	vector<edge> edges;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedOk();

};
