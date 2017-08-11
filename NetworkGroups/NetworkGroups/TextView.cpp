//TextView.cpp : 实现文件
#include "stdafx.h"
#include "TextView.h"
#include "NetworkGroups.h"
#include "NetGroupDialog.h"

// CTextView
IMPLEMENT_DYNCREATE(CTextView, CEditView)

CTextView::CTextView(){}
CTextView::~CTextView(){}

BEGIN_MESSAGE_MAP(CTextView, CEditView)
	ON_COMMAND(ID_32772, &CTextView::OnNetGroup)
END_MESSAGE_MAP()

#ifdef _DEBUG
void CTextView::AssertValid() const{
	CEditView::AssertValid();
}

#ifndef _WIN32_WCE
void CTextView::Dump(CDumpContext& dc) const{
	CEditView::Dump(dc);
}
#endif
#endif //_DEBUG

void CTextView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	pDoc=(CTextDocument*) GetDocument();
	this->SetWindowTextA(pDoc->n_Text);
	Invalidate(TRUE);
}

void CTextView::OnNetGroup()
{
	NetGroupDialog NetGroupSetting;
	NetGroupSetting.DoModal();
	Invalidate();
}