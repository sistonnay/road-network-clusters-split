#include "stdafx.h"
#include "afxdialogex.h"
#include "ChildFrm.h"
#include "BmpDocument.h"
#include "TextDocument.h"
#include "MemDCView.h"
#include "NetworkGroups.h"
#include "NetGroupDialog.h"
#include "Algorithm.h"
#include "CommunitiesWithExpects.h"
#include "CommunitiesWithMaxEdges.h"
#include "AntDetect.h"

IMPLEMENT_DYNAMIC(NetGroupDialog, CDialog)

NetGroupDialog::NetGroupDialog(CWnd* pParent /*=NULL*/)
	: CDialog(NetGroupDialog::IDD, pParent)
	, isExport(FALSE)
	, p_Str_1(_T(""))
	, p_Str_2(_T(""))
	, p_Num_1(0)
	, p_Num_2(0)
	, p_Str_3(_T(""))
	, p_Prob(0)
	, SavePath(_T(""))
	, isWalk(FALSE)
{
	edges_num = 0;
	pairs_num = 0;
	edges_max = 0;
	filePath = "";
	fileName = "";
	state_str = "";
	fileSave = "";
}

NetGroupDialog::~NetGroupDialog(){}

void NetGroupDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COM_SOLUTION, Solution);
	DDX_Control(pDX, IDC_COM_OBJECT, Objection);
	DDX_Check(pDX, IDC_EXPORT_ANS, isExport);
	DDX_Text(pDX, IDC_STATIC1, p_Str_1);
	DDX_Text(pDX, IDC_STATIC2, p_Str_2);
	DDX_Text(pDX, IDC_EDIT1, p_Num_1);
	DDX_Text(pDX, IDC_EDIT2, p_Num_2);
	DDX_Text(pDX, IDC_STATIC3, p_Str_3);
	DDX_Text(pDX, IDC_EDIT3, p_Prob);
	DDV_MinMaxFloat(pDX, p_Prob, 0, 1);
	DDX_Text(pDX, IDC_STATICPATH, SavePath);
	DDX_Check(pDX, IDC_EXPORT_ANS2, isWalk);
}

BEGIN_MESSAGE_MAP(NetGroupDialog, CDialog)
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDOK, &NetGroupDialog::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COM_SOLUTION, &NetGroupDialog::OnCbnSelchangeComSolution)
	ON_BN_CLICKED(IDC_EXPORT_ANS, &NetGroupDialog::OnBnClickedExportAns)
	ON_BN_CLICKED(IDC_SAVEBTN, &NetGroupDialog::OnBnClickedSavebtn)
END_MESSAGE_MAP()

BOOL NetGroupDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	Solution.AddString("��Ⱥ�㷨�Ż�");
	Solution.AddString("ģ���˻��Ż�");
	Solution.AddString("��ģ����Ż�");
	Solution.SetCurSel(0);
	Objection.AddString("��׼ģ���");
	Objection.AddString("�Զ����Ⱥָ��");
	Objection.SetCurSel(0);
	p_Str_1="���ߴ�����";
	p_Str_2="����������";
	p_Str_3="���ܸ��ʣ�";
	p_Num_1=50;
	p_Num_2=100;
	p_Prob =0.8;
	isWalk = TRUE;
	UpdateData(FALSE);
	return TRUE; 
}

BOOL NetGroupDialog::OnEraseBkgnd(CDC* pDC)
{
	CRect r;
	GetClientRect(&r);

	CBitmap Cbmp;
	BITMAP bmp;
	Cbmp.LoadBitmap(IDB_BACKGROUND);
	Cbmp.GetBitmap(&bmp);//�õ�λͼ�ߴ�

	CDC MemDC;
	MemDC.CreateCompatibleDC(pDC);
	CBitmap* oldBmp = MemDC.SelectObject(&Cbmp);
	pDC->SetStretchBltMode(COLORONCOLOR);//���ģʽ�����õĻ��ᵼ��ͼƬ����ʧ��
	pDC->StretchBlt(0,0,r.Width(),r.Height(),&MemDC,0,0,bmp.bmWidth,bmp.bmHeight,SRCCOPY);
	MemDC.SelectObject(oldBmp);
	return TRUE;
}

void NetGroupDialog::OnBnClickedOk()
{
	UpdateData(TRUE);
	if(isExport&&(fileSave=="")){
		AfxMessageBox("�����뱣��·����");
		return;
	}
	CMDIFrameWnd* FrameWnd=(CMDIFrameWnd*)::AfxGetMainWnd();
	CDocument* pDoc = FrameWnd->MDIGetActive()->GetActiveView()->GetDocument();

	filePath = pDoc->GetPathName();
	int NameIndex=filePath.ReverseFind('\\');
	int ExtIndex=filePath.ReverseFind('.');
	fileName=filePath.Mid(NameIndex+1,ExtIndex-NameIndex-1);
	ReadPairsTable(filePath);
	CDialog::OnOK();

	CString solution,objection;
	Solution.GetWindowTextA(solution);
	Objection.GetWindowTextA(objection);

	if(solution=="��ģ����Ż�"){
		if(objection=="��׼ģ���"){
			BaseOptimizing1();
		}else if(objection=="�Զ����Ⱥָ��"){
			BaseOptimizing2();
		}
	}else if(solution=="��Ⱥ�㷨�Ż�"){
		if(objection=="��׼ģ���"){
			if(isWalk)
				SABAOptimizing1();//�ֲ���Ⱥ�Ż�-��׼ģ���
			else
				NACAOptimizing1();
			//pThread=AfxBeginThread(ThreadFunc,this);//����MFC�߳�
		}else if(objection=="�Զ����Ⱥָ��"){
			NACAOptimizing2();
		}
	}else if(solution=="ģ���˻��Ż�"){
		if(objection=="��׼ģ���"){
			MCMCOptimizing1();
		}else if(objection=="�Զ����Ⱥָ��"){
			MCMCOptimizing2();
		}
	}
}

void NetGroupDialog::SABAOptimizing1()//�ֲ���Ⱥ�Ż�-��׼ģ���
{
	CMDIFrameWnd* pFrame=(CMDIFrameWnd*)::AfxGetMainWnd();//Dialog�е���
	CMFCStatusBar* pStatusBar = (CMFCStatusBar*)pFrame->GetDescendantWindow(AFX_IDW_STATUS_BAR); 
	srand((unsigned)time(NULL));
	int ans,temperate(20);
	vector<int> ant;
	for(int i=0;i<p_Num_2;++i)
		ant.push_back(rand()%pairs_num);

	AntDetect antwalk;
	antwalk.InitParameters(pairs);
	vector<local_info> info;
	vector<double> m_modularitys;
	double pre_modularity,now_modularity;
	pre_modularity = antwalk.CalModularity();
	int ger(0);
	while(1)
	{
		for(int i=0;i<p_Num_2;++i)
		{
			info.clear();
			antwalk.GetCloseInfo(info,ant[i]);
			ans = antwalk.LocalModify(ant[i],0.5,temperate,info);
			antwalk.AcceptModify(ant[i],ans,p_Prob,info);	
		}
		antwalk.UpdatePheromone(p_Prob);
		now_modularity = antwalk.CalModularity();
		m_modularitys.push_back(now_modularity);
		temperate --;
		state_str.Format("%d",++ger);
		pStatusBar->SetPaneText(0,"��������"+state_str+"%",TRUE);
		if(abs(now_modularity-pre_modularity)<1.0e-8) break;
		pre_modularity = now_modularity;
	}

	CString str_best = antwalk.GetGroupSetsStr().c_str();//��ѷ�Ⱥ��ʽ
	ShowResult(m_modularitys,str_best,"�ֲ���Ⱥ�Ż�����׼ģ��ȣ�");
	if(isExport) antwalk.SaveGroupResult(fileSave);//����
}

void NetGroupDialog::BaseOptimizing1()//��ģ������-��׼ģ���
{
	srand(time(NULL));
	CMDIFrameWnd* pFrame=(CMDIFrameWnd*)::AfxGetMainWnd();
	CMFCStatusBar* pStatusBar = (CMFCStatusBar*)pFrame->GetDescendantWindow(AFX_IDW_STATUS_BAR); 

	state_str = "���ڳ�ʼ��";
	pStatusBar->SetPaneText(0,state_str,TRUE);

	Algorithm* test = new Algorithm(pairs);
	vector<Bridge> bridges = test->getBridges();	//�����ű�
	CommunitiesWithExpects tempSet,tempBest,buffer; //�����Ⱥ
	tempSet.InitSetByBridge(pairs,bridges);			//����߳�ʼ��
	//tempSet.InitSetByNumber(pairs,1);				//������������ʼ��

	int i,j,k,ans,imax;
	double tempFit,bufferFit,tempBestFit;
	double oldfitDelta(0),newfitDelta;
	vector<NodeCloseInfo> class_nears;
	vector<int> remain_nodes = nodes;
	vector<double> bestFits;

	state_str = "�������� 0.00%";
	pStatusBar->SetPaneText(0,state_str,TRUE);

	tempBest = tempSet;
	tempBestFit = tempSet.CalModularity();
	for(k=0;k<p_Num_1;k++)
	{
		buffer = tempBest;
		remain_nodes = nodes;
		for(j=0;j<pairs_num;++j)
		{
			imax = remain_nodes[0];
			tempFit = buffer.CalModularity();
			for(i=0;i<remain_nodes.size();++i)
			{
				class_nears.clear();
				buffer.GetCloseInfo(class_nears,remain_nodes[i]);
				ans = buffer.SmallSetChange(remain_nodes[i],class_nears,bridges);//ans=0,1,2
				if(ans==0){
					bufferFit = tempFit;
					continue;
				}else
					bufferFit = buffer.CalModularity();

				newfitDelta = bufferFit-tempFit;
				if(newfitDelta!=0)
				{
					if((oldfitDelta==0)||(newfitDelta>oldfitDelta))
					{
						buffer.AcceptChange(remain_nodes[i],class_nears,ans);
						imax = remain_nodes[i];
						oldfitDelta = newfitDelta;
					}
					else{
						buffer.RefuseChange(remain_nodes[i],ans);
						bufferFit = tempFit;
					}
				}
				else{
					buffer.RefuseChange(remain_nodes[i],ans);
					bufferFit = tempFit;
				}
			}
			remain_nodes.erase(find(remain_nodes.begin(),remain_nodes.end(),imax));
		}
		//һ���غϺ����Ƿ����
		if(bufferFit>tempBestFit)
		{
			tempBest = buffer;
			tempBestFit = bufferFit;
		}
		bestFits.push_back(tempBestFit);
		state_str.Format("%d",100*k/p_Num_1);
		pStatusBar->SetPaneText(0,"�������� "+state_str+"%",TRUE);
	}
	CString str_best = tempBest.GetGroupSetsStr().c_str();//��ѷ�Ⱥ��ʽ
	ShowResult(bestFits,str_best,"��ģ����Ż�����׼ģ��ȣ�");
	if(isExport) tempBest.SaveGroupResult(fileSave);
}
void NetGroupDialog::BaseOptimizing2()//��ģ������-�Զ����Ⱥָ��
{
	srand(time(NULL));
	CMDIFrameWnd* pFrame=(CMDIFrameWnd*)::AfxGetMainWnd();
	CMFCStatusBar* pStatusBar = (CMFCStatusBar*)pFrame->GetDescendantWindow(AFX_IDW_STATUS_BAR); 

	state_str = "���ڳ�ʼ��";
	pStatusBar->SetPaneText(0,state_str,TRUE);

	Algorithm* test = new Algorithm(pairs);
	vector<Bridge> bridges = test->getBridges();	//�����ű�
	CommunitiesWithMaxEdges tempSet,tempBest,buffer;//�����Ⱥ
	tempSet.InitSetByBridge(pairs,bridges);			//����߳�ʼ��
	//tempSet.InitSetByNumber(pairs,1);				//������������ʼ��

	int i,j,k,ans,imax;
	double tempFit,bufferFit,tempBestFit;
	double oldfitDelta(0),newfitDelta;
	vector<NodeCloseInfo> class_nears;
	vector<int> remain_nodes = nodes;
	vector<double> bestFits;

	state_str = "�������� 0.00%";
	pStatusBar->SetPaneText(0,state_str,TRUE);

	tempBest = tempSet;
	tempBestFit = tempSet.CalModularity();
	for(k=0;k<p_Num_1;k++)
	{
		buffer = tempBest;
		remain_nodes = nodes;
		for(j=0;j<pairs_num;++j)
		{
			imax = remain_nodes[0];
			tempFit = buffer.CalModularity();
			for(i=0;i<remain_nodes.size();++i)
			{
				class_nears.clear();
				buffer.GetCloseInfo(class_nears,remain_nodes[i]);
				ans = buffer.SmallSetChange(remain_nodes[i],class_nears,bridges);//ans=0,1,2
				if(ans==0){
					bufferFit = tempFit;
					continue;
				}else
					bufferFit = buffer.CalModularity();

				newfitDelta = bufferFit-tempFit;
				if(newfitDelta!=0)
				{
					if((oldfitDelta==0)||(newfitDelta>oldfitDelta))
					{
						buffer.AcceptChange(remain_nodes[i],class_nears,ans);
						imax = remain_nodes[i];
						oldfitDelta = newfitDelta;
					}
					else{
						buffer.RefuseChange(remain_nodes[i],ans);
						bufferFit = tempFit;
					}
				}
				else{
					buffer.RefuseChange(remain_nodes[i],ans);
					bufferFit = tempFit;
				}
			}
			remain_nodes.erase(find(remain_nodes.begin(),remain_nodes.end(),imax));
		}
		//һ���غϺ����Ƿ����
		if(bufferFit>tempBestFit)
		{
			tempBest = buffer;
			tempBestFit = bufferFit;
		}
		bestFits.push_back(tempBestFit);
		state_str.Format("%d",100*k/p_Num_1);
		pStatusBar->SetPaneText(0,"�������� "+state_str+"%",TRUE);
	}
	CString str_best = tempBest.GetGroupSetsStr().c_str();//��ѷ�Ⱥ��ʽ
	ShowResult(bestFits,str_best,"��ģ����Ż�����׼ģ��ȣ�");
	if(isExport) tempBest.SaveGroupResult(fileSave);
}

void NetGroupDialog::NACAOptimizing1()//С������Ⱥ�㷨-��׼ģ���
{
	srand(time(NULL));
	//CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();//View�е���
	CMDIFrameWnd* pFrame=(CMDIFrameWnd*)::AfxGetMainWnd();//Dialog�е���
	CMFCStatusBar* pStatusBar = (CMFCStatusBar*)pFrame->GetDescendantWindow(AFX_IDW_STATUS_BAR); 

	int i,j;//------------------------------------------------------------//ѭ������
	int ants;//-----------------------------------------------------------//��Ⱥ��ģ
	int ger(p_Num_1);//---------------------------------------------------//���ߴ���
	vector<double> m_fits;//----------------------------------------------//�洢��Ӧ��
	vector<double> t_fits;//----------------------------------------------//�洢��Ϣ��
	vector<double> B_fits;//----------------------------------------------//�洢�����Ӧ��
	vector<CommunitiesWithExpects> m_sets;//------------------------------//�洢ÿֻ���ϣ�Ⱥ�飩
	vector<NodeCloseInfo> class_nears;//----------------------------------//�ڵ��ھ���Ϣ
	pairs_num>2*p_Num_2 ? ants=p_Num_2 : ants=1+pairs_num/2;//------------//��������趨100ֻ
	Algorithm* test = new Algorithm(pairs);
	vector<Bridge> bridges = test->getBridges();//------------------------//�����ű�

	state_str = "�Ѿ���ʼ�� 0.00%";
	pStatusBar->SetPaneText(0,state_str,TRUE);	
	for(i=0;i<ants;++i)
	{
		CommunitiesWithExpects tempSet;
		tempSet.InitSetByNumber(pairs,i+1);
		m_sets.push_back(tempSet);
		m_fits.push_back(tempSet.CalModularity());
		state_str.Format("%d",100*i/ants);
		pStatusBar->SetPaneText(0,"�Ѿ���ʼ�� "+state_str+"%",TRUE);
	}

	int	ans;//-------------------------------------------------------------//�ƶ���������ֹ�ģ
	int index(0);//--------------------------------------------------------//��Ų���ڵ�
	double count;//--------------------------------------------------------//�����ڵ�
	double deta=(double)pairs_num/ants;//----------------------------------//�ڵ�ѡȡ���
	double P_s=1.0;//------------------------------------------------------//P_s ȫ��ת��ѡ������
	double P_v=p_Prob;//---------------------------------------------------//P_v ��Ϣ������ϵ��
	double F_best,F_wost,T_best;//-----------------------------------------//�����Ӧ�ȡ������Ϣ��
	double Probability;//--------------------------------------------------//���ܸ���
	double new_fit,old_fit;//----------------------------------------------//��ʱ����
	F_best = *max_element(m_fits.begin(),m_fits.end());//------------------//���fit��max_element��
	F_wost = *min_element(m_fits.begin(),m_fits.end());//------------------//��Сfit��min_element��
	for(i=0;i<ants;++i) t_fits.push_back(m_fits[i]-F_wost);//--------------//�Ǹ���
	T_best = *max_element(t_fits.begin(),t_fits.end());//------------------//ȡ�����Ϣ��

	state_str = "�������� 0.00%";
	pStatusBar->SetPaneText(0,state_str,TRUE);
	for(i=0;i<ger;++i)
	{	
		count = i;//------------------------------------------------------//�˴����¼���
		for(j=0;j<ants;++j)
		{
			old_fit = m_fits[j];
			Probability = (T_best-t_fits[j])/T_best;
			if(Probability<P_s)
			{
				class_nears.clear();
				index = ((int)(count+0.5))%pairs_num;//--------------------//��ת�ƽڵ��±�
				m_sets[j].GetCloseInfo(class_nears,index);//---------------//ͳ���ھ���Ϣ
				ans = m_sets[j].SmallSetChange(index,class_nears,bridges);
				if(ans==0)
					new_fit = old_fit;
				else //if(ans==1||ans==2)
					new_fit = m_sets[j].CalModularity();
			}
			else
			{
				int num = 1 + rand()%ants;
				ans = m_sets[j].BigSetChange(num);
				new_fit = m_sets[j].CalModularity();
			}
			if(new_fit>old_fit)//-----------------------------------------//������Ϣ��
			{
				m_sets[j].AcceptChange(index,class_nears,ans);
				m_fits[j] = new_fit;	
				if(new_fit>F_best) F_best=new_fit;	
				t_fits[j] = (1-P_v)*t_fits[j] + F_best - new_fit;
			}
			else{
				m_sets[j].RefuseChange(index,ans);
				t_fits[j] = (1-P_v)*t_fits[j] + F_best - old_fit;
			}
			if(t_fits[j]>T_best) T_best=t_fits[j];
			count += deta;
		}
		B_fits.push_back(F_best);
		state_str.Format("%d",100*i/ger);
		pStatusBar->SetPaneText(0,"�������� "+state_str+"%",TRUE);
	}
	int n = max_element(m_fits.begin(),m_fits.end()) - m_fits.begin();
	CString str_best = m_sets[n].GetGroupSetsStr().c_str();//��ѷ�Ⱥ��ʽ
	vector<double> show_fits;
	count = 0;
	while(count<ger)
	{
		show_fits.push_back(B_fits[count]);
		count += (double)ger/100;
	}
	ShowResult(show_fits,str_best,"��Ⱥ�㷨�Ż�����׼ģ��ȣ�");
	if(isExport) m_sets[n].SaveGroupResult(fileSave);//����
}
void NetGroupDialog::NACAOptimizing2()//��Ⱥ�㷨�Ż�-�Զ����Ⱥָ��
{
	srand(time(NULL));
	//CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();//View�е���
	CMDIFrameWnd* pFrame=(CMDIFrameWnd*)::AfxGetMainWnd();//Dialog�е���
	CMFCStatusBar* pStatusBar = (CMFCStatusBar*)pFrame->GetDescendantWindow(AFX_IDW_STATUS_BAR); 

	int i,j;//------------------------------------------------------------//ѭ������
	int ants;//-----------------------------------------------------------//��Ⱥ��ģ
	int ger(p_Num_1);//---------------------------------------------------//���ߴ���
	vector<double> m_fits;//----------------------------------------------//�洢��Ӧ��
	vector<double> t_fits;//----------------------------------------------//�洢��Ϣ��
	vector<double> B_fits;//----------------------------------------------//�洢�����Ӧ��
	vector<CommunitiesWithMaxEdges> m_sets;//-----------------------------//�洢ÿֻ���ϣ�Ⱥ�飩
	vector<NodeCloseInfo> class_nears;//----------------------------------//�ڵ��ھ���Ϣ
	pairs_num>2*p_Num_2 ? ants=p_Num_2 : ants=1+pairs_num/2;//------------//��������趨100ֻ
	Algorithm* test = new Algorithm(pairs);
	vector<Bridge> bridges = test->getBridges();//------------------------//�����ű�

	state_str = "�Ѿ���ʼ�� 0.00%";
	pStatusBar->SetPaneText(0,state_str,TRUE);	
	for(i=0;i<ants;++i)
	{
		CommunitiesWithMaxEdges tempSet;
		tempSet.InitSetByNumber(pairs,i+1);
		m_sets.push_back(tempSet);
		m_fits.push_back(tempSet.CalModularity());
		state_str.Format("%d",100*i/ants);
		pStatusBar->SetPaneText(0,"�Ѿ���ʼ�� "+state_str+"%",TRUE);
	}

	int	ans;//-------------------------------------------------------------//�ƶ���������ֹ�ģ
	int index(0);//--------------------------------------------------------//��Ų���ڵ�
	double count;//--------------------------------------------------------//�����ڵ�
	double deta=(double)pairs_num/ants;//----------------------------------//�ڵ�ѡȡ���
	double P_s=1.0;//------------------------------------------------------//P_s ȫ��ת��ѡ������
	double P_v=p_Prob;//---------------------------------------------------//P_v ��Ϣ������ϵ��
	double F_best,F_wost,T_best;//-----------------------------------------//�����Ӧ�ȡ������Ϣ��
	double Probability;//--------------------------------------------------//���ܸ���
	double new_fit,old_fit;//----------------------------------------------//��ʱ����
	F_best = *max_element(m_fits.begin(),m_fits.end());//------------------//���fit��max_element��
	F_wost = *min_element(m_fits.begin(),m_fits.end());//------------------//��Сfit��min_element��
	for(i=0;i<ants;++i) t_fits.push_back(m_fits[i]-F_wost);//--------------//�Ǹ���
	T_best = *max_element(t_fits.begin(),t_fits.end());//------------------//ȡ�����Ϣ��

	state_str = "�������� 0.00%";
	pStatusBar->SetPaneText(0,state_str,TRUE);
	for(i=0;i<ger;++i)
	{	
		count = i;//------------------------------------------------------//�˴����¼���
		for(j=0;j<ants;++j)
		{
			old_fit = m_fits[j];
			Probability = (T_best-t_fits[j])/T_best;
			if(Probability<P_s)
			{
				class_nears.clear();
				index = ((int)(count+0.5))%pairs_num;//--------------------//��ת�ƽڵ��±�
				m_sets[j].GetCloseInfo(class_nears,index);//---------------//ͳ���ھ���Ϣ
				ans = m_sets[j].SmallSetChange(index,class_nears,bridges);
				if(ans==0)
					new_fit = old_fit;
				else //if(ans==1||ans==2)
					new_fit = m_sets[j].CalModularity();
			}
			else
			{
				int num = 1 + rand()%ants;
				ans = m_sets[j].BigSetChange(num);
				new_fit = m_sets[j].CalModularity();
			}
			if(new_fit>old_fit)//-----------------------------------------//������Ϣ��
			{
				m_sets[j].AcceptChange(index,class_nears,ans);
				m_fits[j] = new_fit;	
				if(new_fit>F_best) F_best=new_fit;	
				t_fits[j] = (1-P_v)*t_fits[j] + F_best - new_fit;
			}
			else{
				m_sets[j].RefuseChange(index,ans);
				t_fits[j] = (1-P_v)*t_fits[j] + F_best - old_fit;
			}
			if(t_fits[j]>T_best) T_best=t_fits[j];
			count += deta;
		}
		B_fits.push_back(F_best);
		state_str.Format("%d",100*i/ger);
		pStatusBar->SetPaneText(0,"�������� "+state_str+"%",TRUE);
	}
	int n = max_element(m_fits.begin(),m_fits.end()) - m_fits.begin();
	CString str_best = m_sets[n].GetGroupSetsStr().c_str();//��ѷ�Ⱥ��ʽ
	ShowResult(B_fits,str_best,"��Ⱥ�㷨�Ż����Զ���ָ�꣩");
	if(isExport) m_sets[n].SaveGroupResult(fileSave);
}

void NetGroupDialog::MCMCOptimizing1()//ģ���˻��Ż�-��׼ģ���
{
	srand(time(NULL));
	CMDIFrameWnd* pFrame=(CMDIFrameWnd*)::AfxGetMainWnd();//Dialog�е���
	CMFCStatusBar* pStatusBar = (CMFCStatusBar*)pFrame->GetDescendantWindow(AFX_IDW_STATUS_BAR);

	state_str = "���ڳ�ʼ��";
	pStatusBar->SetPaneText(0,state_str,TRUE);

	int i,temperature,border,iterator,node,num_dev,ans;//i,��ʼ�¶�,�¶ȷָ��,��������,��ת�ƽڵ�,��Ŀ�����,�ı���
	double accept_prob,base_prob,count(0),deta;//ԭʼ����,��������,�����ڵ�,�ڵ�ѡȡ���
	double old_fit,new_fit,rnd_pro,acc_pro,delta;//�ϴ�Ŀ��ֵ,��ǰĿ��ֵ,�������,���ܸ���,Ŀ���
	double sum_dev,avg_dev,tmp_dev,sum_squa,curr_dev,stand_dev;//Ŀ����ܺ�,��ֵ,��ʱֵ,����,��ǰ�����,��ʷ�����
	
	vector<int> group_nodes;//ĳȺ�ڵĽڵ㼯��
	vector<double> Energys,delta_fits;//Ŀ�꺯��ֵ,Ŀ�꺯����
	vector<NodeCloseInfo> class_nears;//�ڵ��ڽ���Ϣ
	Algorithm* test = new Algorithm(pairs);//����һ��ͼ���㷨
	vector<Bridge> bridges = test->getBridges();//�����ű�
	CommunitiesWithExpects tempSet;//�����Ⱥ
	tempSet.InitSetByBridge(pairs,bridges);//����߳�ʼ��

	temperature = p_Num_1;
	iterator = p_Num_2;
	accept_prob = p_Prob;
	border = 0.25*temperature;
	base_prob = accept_prob;	
	stand_dev = 1e8;//��ʼ��Ϊ�ܴ�
	deta = (double)pairs_num/iterator;
	new_fit = old_fit = tempSet.CalModularity();

	state_str = "�������� 0.00%";
	pStatusBar->SetPaneText(0,state_str,TRUE);

	while(temperature>0)
	{
		sum_dev = 0;
		delta_fits.clear();
		for(i=0;i<iterator;++i)
		{
			if(temperature>border)//�ߡ�����
			{	//node = rand()%pairs_num;//�����ʽѡ��
				node = ((int)(count+0.5))%pairs_num;//���ȷ�ʽѡ��
				count += deta;
			}else//����
			{
				group_nodes = tempSet.groups[tempSet.node_list[node].group_id].GetNodes();
				node = group_nodes[rand()%group_nodes.size()];//���ѡȡһ��
			}
			class_nears.clear();
			tempSet.GetCloseInfo(class_nears,node);
			ans = tempSet.SmallSetChange(node,class_nears,bridges);
			if(ans==0){
				new_fit = old_fit;
				continue;
			}else{
				new_fit = tempSet.CalModularity();
			}
			if(new_fit>old_fit)
			{
				tempSet.AcceptChange(node,class_nears,ans);
				old_fit = new_fit;
			}else//����仵�̶ȵ������
			{	
				tmp_dev = old_fit-new_fit;
				sum_dev += tmp_dev;
				delta_fits.push_back(tmp_dev);
				delta = tmp_dev / stand_dev;
				acc_pro = base_prob * exp(-pow(delta,2));
				rnd_pro = rand()/(RAND_MAX+1.0);
				if(rnd_pro<acc_pro)
				{
					tempSet.AcceptChange(node,class_nears,ans);
					old_fit = new_fit;
				}else
				{
					tempSet.RefuseChange(node,ans);
				}
			}
			Energys.push_back(old_fit);
		}
		--temperature;//����
		count = p_Num_1-temperature;//ѡȡ�ڵ�ƫ��
		base_prob *= accept_prob; //�����´ν��»�׼���ܸ���
		//����仵�̶ȱ�׼��
		sum_squa = 0;
		num_dev = delta_fits.size();
		avg_dev = sum_dev/num_dev;
		for(i=0;i<num_dev;++i) sum_squa += pow((delta_fits[i]-avg_dev),2);
		curr_dev = pow(sum_squa/num_dev,0.5);
		curr_dev < 0.8*stand_dev ? stand_dev *= 0.8 : stand_dev = curr_dev;
		state_str.Format("%.0f",100*count/p_Num_1);
		pStatusBar->SetPaneText(0,"�������� "+state_str+"%",TRUE);
	}
	CString str_best = tempSet.GetGroupSetsStr().c_str();//��ѷ�Ⱥ��ʽ
	ShowResult(Energys,str_best,"ģ���˻��Ż�����׼ģ��ȣ�");
	if(isExport) tempSet.SaveGroupResult(fileSave);
}
void NetGroupDialog::MCMCOptimizing2()//ģ���˻��Ż�-�Զ����Ⱥָ��
{
	srand(time(NULL));
	CMDIFrameWnd* pFrame=(CMDIFrameWnd*)::AfxGetMainWnd();//Dialog�е���
	CMFCStatusBar* pStatusBar = (CMFCStatusBar*)pFrame->GetDescendantWindow(AFX_IDW_STATUS_BAR);

	state_str = "���ڳ�ʼ��";
	pStatusBar->SetPaneText(0,state_str,TRUE);

	int i,temperature,border,iterator,node,num_dev,ans;//i,��ʼ�¶�,�¶ȷָ��,��������,��ת�ƽڵ�,��Ŀ�����,�ı���
	double accept_prob,base_prob,count(0),deta;//ԭʼ����,��������,�����ڵ�,�ڵ�ѡȡ���
	double old_fit,new_fit,rnd_pro,acc_pro,delta;//�ϴ�Ŀ��ֵ,��ǰĿ��ֵ,�������,���ܸ���,Ŀ���
	double sum_dev,avg_dev,tmp_dev,sum_squa,curr_dev,stand_dev;//Ŀ����ܺ�,��ֵ,��ʱֵ,����,��ǰ�����,��ʷ�����
	
	vector<int> group_nodes;//ĳȺ�ڵĽڵ㼯��
	vector<double> Energys,delta_fits;//Ŀ�꺯��ֵ,Ŀ�꺯����
	vector<NodeCloseInfo> class_nears;//�ڵ��ڽ���Ϣ
	Algorithm* test = new Algorithm(pairs);//����һ��ͼ���㷨
	vector<Bridge> bridges = test->getBridges();//�����ű�
	CommunitiesWithMaxEdges tempSet;//�����Ⱥ
	tempSet.InitSetByBridge(pairs,bridges);//����߳�ʼ��

	temperature = p_Num_1;
	iterator = p_Num_2;
	accept_prob = p_Prob;
	border = 0.25*temperature;
	base_prob = accept_prob;	
	stand_dev = 1e8;//��ʼ��Ϊ�ܴ�
	deta = (double)pairs_num/iterator;
	new_fit = old_fit = tempSet.CalModularity();

	state_str = "�������� 0.00%";
	pStatusBar->SetPaneText(0,state_str,TRUE);

	while(temperature>0)
	{
		sum_dev = 0;
		delta_fits.clear();
		for(i=0;i<iterator;++i)
		{
			if(temperature>border)//�ߡ�����
			{	//node = rand()%pairs_num;//�����ʽѡ��
				node = ((int)(count+0.5))%pairs_num;//���ȷ�ʽѡ��
				count += deta;
			}else//����
			{
				group_nodes = tempSet.groups[tempSet.node_list[node].group_id].GetNodes();
				node = group_nodes[rand()%group_nodes.size()];//���ѡȡһ��
			}
			class_nears.clear();
			tempSet.GetCloseInfo(class_nears,node);
			ans = tempSet.SmallSetChange(node,class_nears,bridges);
			if(ans==0){
				new_fit = old_fit;
				continue;
			}else{
				new_fit = tempSet.CalModularity();
			}
			if(new_fit>old_fit)
			{
				tempSet.AcceptChange(node,class_nears,ans);
				old_fit = new_fit;
			}else//����仵�̶ȵ������
			{	
				tmp_dev = old_fit-new_fit;
				sum_dev += tmp_dev;
				delta_fits.push_back(tmp_dev);
				delta = tmp_dev / stand_dev;
				acc_pro = base_prob * exp(-pow(delta,2));
				rnd_pro = rand()/(RAND_MAX+1.0);
				if(rnd_pro<acc_pro)
				{
					tempSet.AcceptChange(node,class_nears,ans);
					old_fit = new_fit;
				}else
				{
					tempSet.RefuseChange(node,ans);
				}
			}
			Energys.push_back(old_fit);
		}
		--temperature;//����
		count = p_Num_1-temperature;//ѡȡ�ڵ�ƫ��
		base_prob *= accept_prob; //�����´ν��»�׼���ܸ���
		//����仵�̶ȱ�׼��
		sum_squa = 0;
		num_dev = delta_fits.size();
		avg_dev = sum_dev/num_dev;
		for(i=0;i<num_dev;++i) sum_squa += pow((delta_fits[i]-avg_dev),2);
		curr_dev = pow(sum_squa/num_dev,0.5);
		curr_dev < 0.8*stand_dev ? stand_dev *= 0.8 : stand_dev = curr_dev;
		state_str.Format("%.0f",100*count/p_Num_1);
		pStatusBar->SetPaneText(0,"�������� "+state_str+"%",TRUE);
	}
	CString str_best = tempSet.GetGroupSetsStr().c_str();//��ѷ�Ⱥ��ʽ
	ShowResult(Energys,str_best,"ģ���˻��Ż�����׼ģ��ȣ�");
	if(isExport) tempSet.SaveGroupResult(fileSave);
}

void NetGroupDialog::ReadPairsTable(const char* pathname)
{
	CMDIFrameWnd* pFrame=(CMDIFrameWnd*)::AfxGetMainWnd();
	CMFCStatusBar* pStatusBar = (CMFCStatusBar*)pFrame->GetDescendantWindow(AFX_IDW_STATUS_BAR); 
	state_str = "Reading Pairs Data ...";
	pStatusBar->SetPaneText(0,state_str,TRUE);

	ifstream file(pathname);
	if(!file.is_open()) return;
	edges_num = 0;
	edges_max = 0;
	pairs.clear();
	string buffer;//��ȡ��һ�е��ַ�������ȡ���ܵ���
	getline(file, buffer);//��ȥ"Counts"֮����Ӵ�
	string counts = buffer.substr(6);
	pairs_num = atoi(counts.c_str());
	for(int i=0;i<pairs_num;++i) nodes.push_back(i);
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
		//if (ss.fail()) break;
		++edges_num;
	}
	edges_max = pairs_num*(pairs_num-1)/2;
	state_str = "����";
	pStatusBar->SetPaneText(0,state_str,TRUE);
}

void NetGroupDialog::ShowResult(vector<double> bests,CString result,CString title)
{
	CClientDC pDC(this);
	CDC* pdc=(CDC*)&pDC;
	HBITMAP bitmap =DrawNacaResult(bests,result,pdc);
	CBmpDocument* pDoc = new CBmpDocument();
	pDoc->hbitmap = bitmap;
	CChildFrame* pFrm = NULL;
	if(pFrm!=NULL){	pFrm->MDIActivate();return; }
	pFrm=new CChildFrame();
	CCreateContext context;
	context.m_pNewViewClass=RUNTIME_CLASS(MemDCView);
	context.m_pCurrentDoc=pDoc;
	pFrm->LoadFrame(IDR_BmpTYPE,WS_MAXIMIZE|WS_OVERLAPPEDWINDOW,AfxGetApp()->m_pMainWnd,&context);
	pFrm->SetWindowTextA(fileName+title);
	pFrm->ShowWindow(SW_SHOW);
	pFrm->InitialUpdateFrame(NULL,true);
}

HBITMAP NetGroupDialog::DrawNacaResult(vector<double> bests,CString result,CDC* pDC)
{
	int i;
	float x,y;
	//����ͼ�α߿�******************************************************************************/
	CDC			MemDC;   // �豸DCָ��   �ڴ�DC
	CBitmap		Bitmap;
	int pHeight=1000,pWidth=1000;//
	CRect		InfoBmpRect(0, 0, pWidth, pHeight);	// BMP�Ĵ�С
	Bitmap.CreateCompatibleBitmap(pDC,InfoBmpRect.Width(), InfoBmpRect.Height());	// ����һ���뵱ǰ������ݵ�CBitmap����
	MemDC.CreateCompatibleDC(pDC);					// �����뵱ǰ�豸DC����ݵ��ڴ�DC
	CBitmap		*pOldBitmap = MemDC.SelectObject(&Bitmap);	// ��λͼѡȡ���ڴ�DC��
	CBrush		*pWhiteBrush  = new CBrush(RGB(255, 255, 255));// �½���ɫˢ��
	MemDC.FillRect(&InfoBmpRect, pWhiteBrush);
	CPen		*pOldPen    = (CPen *)pDC->SelectStockObject(BLACK_PEN);//�ɻ���
	CPen		*pMyBluePen = new CPen(PS_SOLID,1,RGB(0,0,255));// �½��ҵĻ���	
	CPen		*pMyRedPen  = new CPen(PS_SOLID,1,RGB(255,0,0));// �½��ҵĻ���
	MemDC.SelectObject(pMyBluePen);//���߿�
	CPoint pts[4];
	pts[0].x=5;pts[0].y=5;
	pts[1].x=620;pts[1].y=5;
	pts[2].x=620;pts[2].y=425;
	pts[3].x=5;pts[3].y=425;
	MemDC.Polygon(pts,4);
	MemDC.SelectObject(pOldPen);//������
	MemDC.MoveTo(20,20);
	MemDC.LineTo(25,25);//
	MemDC.MoveTo(20,20);
	MemDC.LineTo(15,25);//
	//��������
	CRect rect(20,450,650,1000);
	MemDC.DrawText(result,&rect, DT_LEFT);
	//�������귶Χ*****************************************************************************/
	double miny = *min_element(bests.begin(),bests.end());
	double maxy = *max_element(bests.begin(),bests.end());
	double dety = (maxy-miny)/4;
	int maxx = bests.size();
	double detx = maxx/10.0;
	//����ͼ������*****************************************************************************/
	for(i=1;i<5;i++)//��y�̶�
	{
		MemDC.MoveTo(20,400-90*i);
		MemDC.LineTo(24,400-90*i);
		CString Str;
		Str.Format("%e",miny+i*dety);
		MemDC.TextOutA(25,400-90*i-8,Str);
	}
	MemDC.TextOutA(25,10,"Modularity");
	MemDC.MoveTo(20,20);
	MemDC.LineTo(20,400);//
	MemDC.MoveTo(20,400);
	MemDC.LineTo(580,400);//
	MemDC.MoveTo(580,400);
	MemDC.LineTo(575,395);//
	MemDC.MoveTo(580,400);
	MemDC.LineTo(575,405);//
	for(i=0;i<11;i++)//��x�̶�
	{
		MemDC.MoveTo(20+55*i,400);
		MemDC.LineTo(20+55*i,404);
		CString Str;
		Str.Format("%.0f",i*detx);
		MemDC.TextOutA(20+55*i-10,405,Str);
	}
	MemDC.TextOutA(550,375,"Times");
	MemDC.SelectObject(pMyRedPen); //ѡ���ɫ����
	x=20;
	y=400-360*(bests[0]-miny)/(maxy-miny);
	MemDC.MoveTo(x,y);
	for(i=1;i<maxx;++i)
	{
		x=20+550*i/maxx;
		y=400-360*(bests[i]-miny)/(maxy-miny);
		MemDC.LineTo(x,y);
		MemDC.MoveTo(x,y);
		MemDC.Ellipse(x-2,y-2,x+2,y+2);
	}
	HBITMAP ShowBitmap = MemToBitmap(MemDC.GetSafeHdc(),0,0,pWidth,pHeight);
	MemDC.SelectObject(pWhiteBrush);
	MemDC.SelectObject(pOldPen);
	// �ͷ��ڴ�	//��ͼ��ɺ������   
	MemDC.DeleteDC();
	Bitmap.DeleteObject();  
	delete pWhiteBrush;
	delete pMyBluePen;
	delete pMyRedPen;
	return ShowBitmap;
}

HBITMAP NetGroupDialog::MemToBitmap(HDC hScrDC,int nX,int nY,int nWidth,int nHeight)//���ڴ汣�浽λͼ�����溯������֮
{
	HDC hMemDC;
	// ��Ļ���ڴ��豸������ 
	HBITMAP hBitmap,hOldBitmap;  
	// λͼ���  
	hMemDC= CreateCompatibleDC( hScrDC );  
	// ����һ������Ļ�豸��������ݵ�λͼ  
	hBitmap = CreateCompatibleBitmap( hScrDC, nWidth, nHeight );  
	// ����λͼѡ���ڴ��豸��������  
	hOldBitmap = ( HBITMAP )SelectObject( hMemDC, hBitmap );  
	// ����Ļ�豸�����������ڴ��豸��������  
	StretchBlt( hMemDC, 0, 0, nWidth, nHeight, hScrDC, nX, nY, nWidth, nHeight, SRCCOPY );
	//�õ���Ļλͼ�ľ��  
	hBitmap = ( HBITMAP )SelectObject( hMemDC, hOldBitmap );  
	//���
	DeleteDC( hMemDC );  
	DeleteObject( hOldBitmap );  
	//����λͼ���  
	return hBitmap;
}

void NetGroupDialog::OnCbnSelchangeComSolution()
{
	switch(Solution.GetCurSel())
	{
	case 0:
		GetDlgItem(IDC_STATIC2)->ShowWindow(SW_NORMAL);
		GetDlgItem(IDC_EDIT2)->ShowWindow(SW_NORMAL);
		GetDlgItem(IDC_STATIC3)->ShowWindow(SW_NORMAL);
		GetDlgItem(IDC_EDIT3)->ShowWindow(SW_NORMAL);
		p_Str_1 = "���ߴ�����";
		p_Str_2 = "����������";
		p_Str_3 = "���ܸ��ʣ�";
		p_Num_1 = 50;
		p_Num_2 = 100;
		p_Prob =0.8;
		UpdateData(FALSE);
		break;
	case 1:
		GetDlgItem(IDC_STATIC2)->ShowWindow(SW_NORMAL);
		GetDlgItem(IDC_EDIT2)->ShowWindow(SW_NORMAL);
		GetDlgItem(IDC_STATIC3)->ShowWindow(SW_NORMAL);
		GetDlgItem(IDC_EDIT3)->ShowWindow(SW_NORMAL);
		p_Str_1 = "��ʼ�¶ȣ�";
		p_Str_2 = "����������";
		p_Str_3 = "���ܸ��ʣ�";
		p_Num_1 = 40;
		p_Num_2 = 1000;
		p_Prob =0.8;
		UpdateData(FALSE);
		break;
	case 2:
		GetDlgItem(IDC_STATIC2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT3)->ShowWindow(SW_HIDE);
		p_Str_1 = "ѭ��������";
		p_Num_1 = 10;
		UpdateData(FALSE);
		break;
	}
}

void NetGroupDialog::OnBnClickedExportAns()
{
	if(!isExport){
		GetDlgItem(IDC_SAVEBTN)->ShowWindow(SW_NORMAL);
		GetDlgItem(IDC_STATICPATH)->ShowWindow(SW_NORMAL);
		SavePath = "Path:��ѡ�񱣴�·��";
		fileSave = "";
		isExport = TRUE;
	}else{
		GetDlgItem(IDC_SAVEBTN)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATICPATH)->ShowWindow(SW_HIDE);
		SavePath = "Path:��ѡ�񱣴�·��";
		fileSave = "";
		isExport = FALSE;
	}
	UpdateData(FALSE);
}

void NetGroupDialog::OnBnClickedSavebtn()
{
	CFileDialog FileSaveDlg(false, ".txt", NULL, NULL, "�ı��ļ�|*.txt||");
	if (FileSaveDlg.DoModal() != IDOK)
	{
		AfxMessageBox("ѡ��ʧ�ܣ�");
		return;
	}
	fileSave = FileSaveDlg.GetPathName();
	SavePath = fileSave;
	UpdateData(FALSE);
}

UINT ThreadFunc(LPVOID lpParam)
{
	NetGroupDialog* p = (NetGroupDialog*)lpParam;
	p->NACAOptimizing1();
	return 0; //�߳���ɱ  
}