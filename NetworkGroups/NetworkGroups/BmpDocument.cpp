// BmpDocument.cpp : 实现文件
//

#include "stdafx.h"
#include "NetworkGroups.h"
#include "BmpDocument.h"


// CBmpDocument

IMPLEMENT_DYNCREATE(CBmpDocument, CDocument)

CBmpDocument::CBmpDocument()
{
	hbitmap=NULL;
}

BOOL CBmpDocument::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CBmpDocument::~CBmpDocument(){}

BEGIN_MESSAGE_MAP(CBmpDocument, CDocument)
END_MESSAGE_MAP()


// CBmpDocument 诊断
#ifdef _DEBUG
void CBmpDocument::AssertValid() const
{
	CDocument::AssertValid();
}

#ifndef _WIN32_WCE
void CBmpDocument::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif
#endif //_DEBUG

#ifndef _WIN32_WCE
// CBmpDocument 序列化

void CBmpDocument::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}
#endif


// CBmpDocument 命令
