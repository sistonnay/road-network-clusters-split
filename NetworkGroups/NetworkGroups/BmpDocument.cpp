// BmpDocument.cpp : ʵ���ļ�
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


// CBmpDocument ���
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
// CBmpDocument ���л�

void CBmpDocument::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}
#endif


// CBmpDocument ����
