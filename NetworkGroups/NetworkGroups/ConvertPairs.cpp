// ConvertPairs.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "NetworkGroups.h"
#include "ConvertPairs.h"
#include "afxdialogex.h"


// ConvertPairs �Ի���

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


// ConvertPairs ��Ϣ�������

void ConvertPairs::OnBnClickedButton1()
{
char szFilter[] = "Pairs (*.pairs)|*.pairs|All Files (*.*)|*.*||";
	CFileDialog fileOpenDlg(TRUE,"pairs",NULL,OFN_HIDEREADONLY,szFilter);
	if (fileOpenDlg.DoModal() != IDOK)
	{
		AfxMessageBox("��ʧ�ܣ�");
		return;
	}
	input_path = fileOpenDlg.GetPathName();
}

void ConvertPairs::OnBnClickedButton2()
{
	CFileDialog FileSaveDlg(false, ".csv", NULL, NULL, "�ı��ļ�|*.csv||");
	if (FileSaveDlg.DoModal() != IDOK)
	{
		AfxMessageBox("ѡ��ʧ�ܣ�");
		return;
	}
	output_path = FileSaveDlg.GetPathName();
}

void ConvertPairs::OnBnClickedOk()
{
	ifstream file(input_path);
	if(!file.is_open())//�ļ���ʧ��
	{
		AfxMessageBox("��ȡʧ�ܣ�");
		return;
	}
	string buffer;
	getline(file, buffer);;//��ȡ��һ�е��ַ�������ȡ���ܵ���
	string counts = buffer.substr(6);//��ȥ"Counts"֮����Ӵ�
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
