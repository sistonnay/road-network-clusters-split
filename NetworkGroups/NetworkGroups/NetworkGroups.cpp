// 定义应用程序的类行为。
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
	// 基于文件的标准文档命令
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// 标准打印设置命令
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
	ON_COMMAND(ID_32776, &CNetworkGroupsApp::OnOverlapGroup)
	ON_COMMAND(ID_32777, &CNetworkGroupsApp::OnConvertPairs)
END_MESSAGE_MAP()

// CNetworkGroupsApp 构造
CNetworkGroupsApp::CNetworkGroupsApp()
{
	m_bHiColorIcons = TRUE;
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// 如果应用程序是利用公共语言运行时支持(/clr)构建的，则:
	//     1) 必须有此附加设置，“重新启动管理器”支持才能正常工作。
	//     2) 在您的项目中，您必须按照生成顺序向 System.Windows.Forms 添加引用。
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif
	//将以下应用程序 ID 字符串替换为唯一的 ID 字符串；建议的字符串格式
	//为 CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("NetworkGroups.AppID.NoVersion"));
	// 将所有重要的初始化放置在 InitInstance 中
}

// 唯一的一个 CNetworkGroupsApp 对象
CNetworkGroupsApp theApp;

// CNetworkGroupsApp 初始化
BOOL CNetworkGroupsApp::InitInstance()
{	//如果一个运行在 Windows XP 上的应用程序清单指定要
	//使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);
	CWinAppEx::InitInstance();
	if (!AfxSocketInit()){
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;}
	// 初始化 OLE 库
	if (!AfxOleInit()){
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;}
	AfxEnableControlContainer();
	EnableTaskbarInteraction();

	// 使用 RichEdit 控件需要  AfxInitRichEdit2()	
	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));
	LoadStdProfileSettings(4);  // 加载标准 INI 文件选项(包括 MRU)
	InitContextMenuManager();
	InitKeyboardManager();
	InitTooltipManager();

	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);
	// 注册应用程序的文档模板。文档模板将用作文档、框架窗口和视图之间的连接
	pDocTemplate = new CMultiDocTemplate(IDR_TextTYPE,
		RUNTIME_CLASS(CTextDocument),
		RUNTIME_CLASS(CChildFrame), //自定义MDI子框架
		RUNTIME_CLASS(CTextView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);
	pBmpTemplate  = new CMultiDocTemplate(IDR_BmpTYPE,
		RUNTIME_CLASS(CBmpDocument),
		RUNTIME_CLASS(CChildFrame), //自定义MDI子框架
		RUNTIME_CLASS(CBmpView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pBmpTemplate);
	// 创建主 MDI 框架窗口
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME)){
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;
	// 仅当具有后缀时才调用 DragAcceptFiles
	// 在 MDI 应用程序中，这应在设置 m_pMainWnd 之后立即发生
	// 分析标准 shell 命令、DDE、打开文件操作的命令行
	CCommandLineInfo cmdInfo;
	//自定义不打开任何文件
	ParseCommandLine(cmdInfo);
	cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	//自定义文件打开的操作
	//if(cmdInfo.m_nShellCommand == CCommandLineInfo::FileNew)
	//{	//加以下这几行代码就行了，pDocTemplateA和pDocTemplate在前面定义过了，一定要非NULL才行
	//	ASSERT(pDocTemplate!= NULL);
	//	ASSERT_KINDOF(CDocTemplate, pDocTemplate);//进行运行时类型识别,确保pDocTemplateA被加入到应用程序的文档模板中
	//	pDocTemplate->OpenDocumentFile(NULL);//打开文档
	//	//如果不想打开2个文档，就不要下面这三行
	//	//ASSERT(pBmpTemplate!= NULL);
	//	//ASSERT_KINDOF(CDocTemplate, pBmpTemplate);//进行运行时类型识别,确保pDocTemplate被加入到应用程序的文档模板中
	//	//pBmpTemplate->OpenDocumentFile(NULL);
	//}else{
	//	//调度在命令行中指定的命令。如果用 /RegServer、/Register、/Unregserver 或 /Unregister 启动应用程序，则返回 FALSE
	//	if (!ProcessShellCommand(cmdInfo))
	//	return FALSE;
	//}
	// 主窗口已初始化，因此显示它并对其进行更新
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();
	return TRUE;
}

int CNetworkGroupsApp::ExitInstance()
{	//TODO: 处理可能已添加的附加资源
	AfxOleTerm(FALSE);
	return CWinAppEx::ExitInstance();
}

// CNetworkGroupsApp 消息处理程序
// 自定义加载/保存方法
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

// CNetworkGroupsApp 消息处理程序

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
