#include "stdafx.h"
#include "NetworkGroups.h"
#include "TextDocument.h"

// CTextDocument
IMPLEMENT_DYNCREATE(CTextDocument, CDocument)

CTextDocument::CTextDocument()
{
	n_Text="";
	pf=NULL;
}

BOOL CTextDocument::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CTextDocument::~CTextDocument(){}

BEGIN_MESSAGE_MAP(CTextDocument, CDocument)
END_MESSAGE_MAP()

// CTextDocument 诊断
#ifdef _DEBUG
void CTextDocument::AssertValid() const
{
	CDocument::AssertValid();
}

#ifndef _WIN32_WCE
void CTextDocument::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif
#endif //_DEBUG

// CTextDocument 命令
BOOL CTextDocument::OnOpenDocument(LPCTSTR lpszPathName)
{
	if(!CDocument::OnOpenDocument(lpszPathName)) 
		return FALSE;
	//TODO:在此添加您专用的创建代码
	return TRUE;
}
#ifndef _WIN32_WCE

// CTextDocument 序列化
void CTextDocument::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{	}
	else
	{
		pf=ar.GetFile();//通过pf还可以获取文件其他信息
		while(ar.ReadString(m_Text))
		{
			n_Text=n_Text+m_Text+"\r\n";
		}
	}
}
#endif

