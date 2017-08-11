// ConvertPairs.cpp : 实现文件
//

#include "stdafx.h"
#include "NetworkGroups.h"
#include "ConvertPairs.h"
#include "afxdialogex.h"


// ConvertPairs 对话框

IMPLEMENT_DYNAMIC(ConvertPairs, CDialogEx)

ConvertPairs::ConvertPairs(CWnd* pParent /*=NULL*/)
	: CDialogEx(ConvertPairs::IDD, pParent)
{
	input_path = "";
	output_path = "";
}

ConvertPairs::~ConvertPairs()
{
}

void ConvertPairs::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ConvertPairs, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &ConvertPairs::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &ConvertPairs::OnBnClickedButton2)
	ON_BN_CLICKED(IDOK, &ConvertPairs::OnBnClickedOk)
END_MESSAGE_MAP()


// ConvertPairs 消息处理程序

void ConvertPairs::OnBnClickedButton1()
{
char szFilter[] = "Pairs (*.pairs)|*.pairs|All Files (*.*)|*.*||";
	CFileDialog fileOpenDlg(TRUE,"pairs",NULL,OFN_HIDEREADONLY,szFilter);
	if (fileOpenDlg.DoModal() != IDOK)
	{
		AfxMessageBox("打开失败！");
		return;
	}
	input_path = fileOpenDlg.GetPathName();
}

void ConvertPairs::OnBnClickedButton2()
{
	CFileDialog FileSaveDlg(false, ".csv", NULL, NULL, "文本文件|*.csv||");
	if (FileSaveDlg.DoModal() != IDOK)
	{
		AfxMessageBox("选择失败！");
		return;
	}
	output_path = FileSaveDlg.GetPathName();
}

void ConvertPairs::OnBnClickedOk()
{
	ifstream file(input_path);
	if(!file.is_open())//文件打开失败
	{
		AfxMessageBox("读取失败！");
		return;
	}
	string buffer;
	getline(file, buffer);;//获取第一行的字符，并提取出总点数
	string counts = buffer.substr(6);//除去"Counts"之后的子串
	stringstream ss;//sstream
	int buf1,buf2;
	while(getline(file,buffer))
	{
		ss.clear();
		ss.str(buffer);
		ss>>buf1;
		ss>>buf2;
		edge e;
		e.a = buf1-1;
		e.b = buf2-1;
		edges.push_back(e);
	}

	FILE* sFile;

	if((sFile=fopen(output_path,"w+"))==NULL) exit(1);

	string str;
	char tempa[8],tempb[8];
	for(int i=0;i<edges.size();++i)
	{
		itoa(edges[i].a,tempa,10);
		itoa(edges[i].b,tempb,10);
		str = tempa + ',';
		str += tempb;
		fprintf(sFile,"%s",str);
		fprintf(sFile,"\n");
	}
	fclose(sFile); 

	CDialogEx::OnOK();
}
