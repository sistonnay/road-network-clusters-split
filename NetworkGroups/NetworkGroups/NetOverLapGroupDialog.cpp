// NetOverLapGroupDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "afxdialogex.h"
#include "NetworkGroups.h"
#include "NetOverLapGroupDialog.h"
#include "ExtendCommunicaties.h"

// NetOverLapGroupDialog 对话框

IMPLEMENT_DYNAMIC(NetOverLapGroupDialog, CDialog)

NetOverLapGroupDialog::NetOverLapGroupDialog(CWnd* pParent /*=NULL*/)
	: CDialog(NetOverLapGroupDialog::IDD, pParent)
	, pairs_path(_T(""))
	, group_path(_T(""))
	, hiera_path(_T(""))
	, save_path(_T(""))
	, yuzhi(0.5)
{
	edges_num = 0;
	pairs_num = 0;
	edges_max = 0;
}

NetOverLapGroupDialog::~NetOverLapGroupDialog()
{
}

void NetOverLapGroupDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ORGIN_FILE, pairs_path);
	DDX_Text(pDX, IDC_GROUP_FILE, group_path);
	DDX_Text(pDX, IDC_Hierachical_FILE, hiera_path);
	DDX_Text(pDX, IDC_Hierachical_FILE2, save_path);
	DDX_Text(pDX, IDC_EDIT1, yuzhi);
	DDV_MinMaxDouble(pDX, yuzhi, 0, 5);
}

BEGIN_MESSAGE_MAP(NetOverLapGroupDialog, CDialog)
	ON_BN_CLICKED(IDOK, &NetOverLapGroupDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &NetOverLapGroupDialog::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &NetOverLapGroupDialog::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &NetOverLapGroupDialog::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &NetOverLapGroupDialog::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &NetOverLapGroupDialog::OnBnClickedButton5)
END_MESSAGE_MAP()

// NetOverLapGroupDialog 消息处理程序

void NetOverLapGroupDialog::OnBnClickedOk()
{
	//MCMCOptimizing();
	UpdateData(TRUE);

	int i;
	int group_node_sum = 0;
	if((pairs_path=="")||(group_path=="")||(hiera_path==""))
	{
		AfxMessageBox("请正确选择所需文件路径！");
		return;
	}
	if(save_path=="")
	{
		AfxMessageBox("请正确选择文件保存路径！");
		return;
	}
	if(color_path=="")
	{
		AfxMessageBox("请正确选择配色文件路径！");
		return;
		if(colors.size()<groups.size())
		{
			AfxMessageBox("请选择一个颜色数比群组数多的配色文件");
			return;
		}
	}
	for(i=0;i<groups.size();++i) group_node_sum+=groups[i].size();
	if(!(group_node_sum==hiers.size()&&group_node_sum==pairs_num)){
		AfxMessageBox("数据不匹配\r\n请选择正确文件！");
		return;
	}
	if(yuzhi<0){
		AfxMessageBox("请输入正确的参数！");
		return;
	}
	GetDlgItem(IDC_ReadProgress)->SetWindowText("正在疯狂计算中...");
	CONAOptimizing();//重叠节点条件-标准模块度
	GetDlgItem(IDC_ReadProgress)->SetWindowText("大功告成!");
	CDialog::OnOK();
}

void NetOverLapGroupDialog::OnBnClickedButton1()
{
	char szFilter[] = "Pairs (*.pairs)|*.pairs|All Files (*.*)|*.*||";
	CFileDialog fileOpenDlg(TRUE,"pairs",NULL,OFN_HIDEREADONLY,szFilter);
	if (fileOpenDlg.DoModal() != IDOK)
	{
		AfxMessageBox("打开失败！");
		return;
	}
	pairs_path = fileOpenDlg.GetPathName();
	UpdateData(FALSE);
	
	//进度显示
	CString state_str="Reading Pairs Data ...";
	GetDlgItem(IDC_ReadProgress)->SetWindowText(state_str);

	ifstream file(pairs_path);
	if(!file.is_open())//文件打开失败
	{
		GetDlgItem(IDC_ReadProgress)->SetWindowText("state:");
		AfxMessageBox("读取失败！");
		return;
	}
	edges_num = 0;
	edges_max = 0;
	pairs.clear();
	string buffer;
	getline(file, buffer);;//获取第一行的字符，并提取出总点数
	string counts = buffer.substr(6);//除去"Counts"之后的子串
	pairs_num = atoi(counts.c_str());
	pairs.resize(pairs_num);
	stringstream ss;//sstream
	int buf1,buf2;
	while(getline(file,buffer))
	{
		ss.clear();
		ss.str(buffer);
		ss>>buf1;
		ss>>buf2;
		pairs[buf1].push_back(buf2);
		pairs[buf2].push_back(buf1);
		++edges_num;
	}
	edges_max = pairs_num*(pairs_num-1)/2;
	GetDlgItem(IDC_ReadProgress)->SetWindowText("state:");
}

void NetOverLapGroupDialog::OnBnClickedButton2()
{
	char szFilter[] = "Text (*.txt)|*.txt|All Files (*.*)|*.*||";
	CFileDialog fileOpenDlg(TRUE,"txt",NULL,OFN_HIDEREADONLY,szFilter);
	if (fileOpenDlg.DoModal() != IDOK)
	{
		AfxMessageBox("打开失败！");
		return;
	}
	group_path = fileOpenDlg.GetPathName();
	UpdateData(FALSE);
	
	//进度显示
	CString state_str="Reading Groups Data ...";
	GetDlgItem(IDC_ReadProgress)->SetWindowText(state_str);

	ifstream file(group_path);
	if(!file.is_open())//文件打开失败
	{
		GetDlgItem(IDC_ReadProgress)->SetWindowText("state:");
		AfxMessageBox("读取失败！");
		return;
	}
	groups.clear();
	string buffer;
	getline(file, buffer);;//获取第一行的字符，并提取出总点数
	string counts = buffer.substr(6);//除去"Counts"之后的子串
	int count = atoi(counts.c_str());
	groups.resize(count);
	stringstream ss;//sstream
	int buf1,buf2;
	while(getline(file,buffer))
	{
		ss.clear();
		ss.str(buffer);
		ss>>buf1;
		ss>>buf2;
		groups[buf2].push_back(buf1);
	}
	GetDlgItem(IDC_ReadProgress)->SetWindowText("state:");
}

void NetOverLapGroupDialog::OnBnClickedButton3()
{
	char szFilter[] = "Hiers (*.hiers)|*.hiers|All Files (*.*)|*.*||";
	CFileDialog fileOpenDlg(TRUE,"hiers",NULL,OFN_HIDEREADONLY,szFilter);
	if (fileOpenDlg.DoModal() != IDOK)
	{
		AfxMessageBox("打开失败！");
		return;
	}
	hiera_path = fileOpenDlg.GetPathName();
	UpdateData(FALSE);
	
	//进度显示
	CString state_str="Reading Hierachical Data ...";
	GetDlgItem(IDC_ReadProgress)->SetWindowText(state_str);

	ifstream file(hiera_path);
	if(!file.is_open())//文件打开失败
	{
		GetDlgItem(IDC_ReadProgress)->SetWindowText("state:");
		AfxMessageBox("读取失败！");
		return;
	}
	hiers.clear();
	string buffer;
	getline(file, buffer);;//获取第一行的字符，并提取出总点数
	string counts = buffer.substr(6);//除去"Counts"之后的子串
	int count = atoi(counts.c_str());
	hiers.resize(count);
	stringstream ss;//sstream
	int buf1,buf2;
	while(getline(file,buffer))
	{
		ss.clear();
		ss.str(buffer);
		ss>>buf1;
		ss>>buf2;
		hiers[buf1]=buf2;
	}
	GetDlgItem(IDC_ReadProgress)->SetWindowText("state:");
}

void NetOverLapGroupDialog::CONAOptimizing()//重叠节点条件-标准模块度
{	
	ExtendCommunicaties cluster(pairs,groups,hiers);
	cluster.OverLappingCluster(yuzhi);
	cluster.SaveGroupResult(save_path,colors);
	//cluster.SaveNodeNears(save_path);
}

void NetOverLapGroupDialog::OnBnClickedButton4()
{
	CFileDialog FileSaveDlg(false, ".csv", NULL, NULL, "文本文件|*.csv||");
	if (FileSaveDlg.DoModal() != IDOK)
	{
		AfxMessageBox("选择失败！");
		return;
	}
	save_path = FileSaveDlg.GetPathName();
	UpdateData(FALSE);	
}

void NetOverLapGroupDialog::OnBnClickedButton5()
{
	char szFilter[] = "Color (*.clr)|*.clr|All Files (*.*)|*.*||";
	CFileDialog fileOpenDlg(TRUE,"clr",NULL,OFN_HIDEREADONLY,szFilter);
	if (fileOpenDlg.DoModal() != IDOK)
	{
		AfxMessageBox("打开失败！");
		return;
	}
	color_path = fileOpenDlg.GetPathName();
	ifstream file(color_path);
	if(!file.is_open())//文件打开失败
	{
		GetDlgItem(IDC_ReadProgress)->SetWindowText("state:");
		AfxMessageBox("读取失败！");
		return;
	}	
	colors.clear();
	string buffer;
	stringstream ss;//sstream
	int buf;
	while(getline(file,buffer))
	{
		ss.clear();
		ss.str(buffer);
		ss>>buf;
		colors.push_back(buf);
	}
	GetDlgItem(IDC_ReadProgress)->SetWindowText("配色文件已读入!");
}
