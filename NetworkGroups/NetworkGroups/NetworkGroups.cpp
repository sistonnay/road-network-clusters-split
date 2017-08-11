// ����Ӧ�ó��������Ϊ��
#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "NetworkGroups.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "TextDocument.h"
#include "TextView.h"
#include "BmpDocument.h"
#include "BmpView.h"
#include "NetOverLapGroupDialog.h"
#include "ConvertPairs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CNetworkGroupsApp
BEGIN_MESSAGE_MAP(CNetworkGroupsApp, CWinAppEx)
	// �����ļ��ı�׼�ĵ�����
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// ��׼��ӡ��������
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
	ON_COMMAND(ID_32776, &CNetworkGroupsApp::OnOverlapGroup)
	ON_COMMAND(ID_32777, &CNetworkGroupsApp::OnConvertPairs)
END_MESSAGE_MAP()

// CNetworkGroupsApp ����
CNetworkGroupsApp::CNetworkGroupsApp()
{
	m_bHiColorIcons = TRUE;
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// ���Ӧ�ó��������ù�����������ʱ֧��(/clr)�����ģ���:
	//     1) �����д˸������ã�������������������֧�ֲ�������������
	//     2) ��������Ŀ�У������밴������˳���� System.Windows.Forms ������á�
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif
	//������Ӧ�ó��� ID �ַ����滻ΪΨһ�� ID �ַ�����������ַ�����ʽ
	//Ϊ CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("NetworkGroups.AppID.NoVersion"));
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}

// Ψһ��һ�� CNetworkGroupsApp ����
CNetworkGroupsApp theApp;

// CNetworkGroupsApp ��ʼ��
BOOL CNetworkGroupsApp::InitInstance()
{	//���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	//ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õĹ����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);
	CWinAppEx::InitInstance();
	if (!AfxSocketInit()){
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;}
	// ��ʼ�� OLE ��
	if (!AfxOleInit()){
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;}
	AfxEnableControlContainer();
	EnableTaskbarInteraction();

	// ʹ�� RichEdit �ؼ���Ҫ  AfxInitRichEdit2()	
	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));
	LoadStdProfileSettings(4);  // ���ر�׼ INI �ļ�ѡ��(���� MRU)
	InitContextMenuManager();
	InitKeyboardManager();
	InitTooltipManager();

	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);
	// ע��Ӧ�ó�����ĵ�ģ�塣�ĵ�ģ�彫�����ĵ�����ܴ��ں���ͼ֮�������
	pDocTemplate = new CMultiDocTemplate(IDR_TextTYPE,
		RUNTIME_CLASS(CTextDocument),
		RUNTIME_CLASS(CChildFrame), //�Զ���MDI�ӿ��
		RUNTIME_CLASS(CTextView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);
	pBmpTemplate  = new CMultiDocTemplate(IDR_BmpTYPE,
		RUNTIME_CLASS(CBmpDocument),
		RUNTIME_CLASS(CChildFrame), //�Զ���MDI�ӿ��
		RUNTIME_CLASS(CBmpView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pBmpTemplate);
	// ������ MDI ��ܴ���
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME)){
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;
	// �������к�׺ʱ�ŵ��� DragAcceptFiles
	// �� MDI Ӧ�ó����У���Ӧ������ m_pMainWnd ֮����������
	// ������׼ shell ���DDE�����ļ�������������
	CCommandLineInfo cmdInfo;
	//�Զ��岻���κ��ļ�
	ParseCommandLine(cmdInfo);
	cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	//�Զ����ļ��򿪵Ĳ���
	//if(cmdInfo.m_nShellCommand == CCommandLineInfo::FileNew)
	//{	//�������⼸�д�������ˣ�pDocTemplateA��pDocTemplate��ǰ�涨����ˣ�һ��Ҫ��NULL����
	//	ASSERT(pDocTemplate!= NULL);
	//	ASSERT_KINDOF(CDocTemplate, pDocTemplate);//��������ʱ����ʶ��,ȷ��pDocTemplateA�����뵽Ӧ�ó�����ĵ�ģ����
	//	pDocTemplate->OpenDocumentFile(NULL);//���ĵ�
	//	//��������2���ĵ����Ͳ�Ҫ����������
	//	//ASSERT(pBmpTemplate!= NULL);
	//	//ASSERT_KINDOF(CDocTemplate, pBmpTemplate);//��������ʱ����ʶ��,ȷ��pDocTemplate�����뵽Ӧ�ó�����ĵ�ģ����
	//	//pBmpTemplate->OpenDocumentFile(NULL);
	//}else{
	//	//��������������ָ������������ /RegServer��/Register��/Unregserver �� /Unregister ����Ӧ�ó����򷵻� FALSE
	//	if (!ProcessShellCommand(cmdInfo))
	//	return FALSE;
	//}
	// �������ѳ�ʼ���������ʾ����������и���
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();
	return TRUE;
}

int CNetworkGroupsApp::ExitInstance()
{	//TODO: �����������ӵĸ�����Դ
	AfxOleTerm(FALSE);
	return CWinAppEx::ExitInstance();
}

// CNetworkGroupsApp ��Ϣ�������
// �Զ������/���淽��
void CNetworkGroupsApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}

void CNetworkGroupsApp::LoadCustomState(){}

void CNetworkGroupsApp::SaveCustomState(){}

// CNetworkGroupsApp ��Ϣ�������

void CNetworkGroupsApp::OnOverlapGroup()
{
	NetOverLapGroupDialog NetOverlapGroupSetting;
	NetOverlapGroupSetting.DoModal();
	//Invalidate();
}


void CNetworkGroupsApp::OnConvertPairs()
{
	ConvertPairs mConvertPairs;
	mConvertPairs.DoModal();
}
