// BmpView.cpp : ʵ���ļ�

#include "stdafx.h"
#include "NetworkGroups.h"
#include "Dib.h"
#include "BmpView.h"
#include "BmpDocument.h"

// CBmpView
IMPLEMENT_DYNCREATE(CBmpView, CScrollView)

CBmpView::CBmpView(){}
CBmpView::~CBmpView(){}

BEGIN_MESSAGE_MAP(CBmpView, CScrollView)
END_MESSAGE_MAP()

// CBmpView ���
#ifdef _DEBUG
void CBmpView::AssertValid() const{
	CScrollView::AssertValid();
}

#ifndef _WIN32_WCE
void CBmpView::Dump(CDumpContext& dc) const{
	CScrollView::Dump(dc);
}
#endif
#endif //_DEBUG

// CBmpView ��Ϣ�������
void CBmpView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
	CSize sizeTotal;
	sizeTotal.cx = sizeTotal.cy = 2000;
	SetScrollSizes(MM_TEXT, sizeTotal);
}
// CBmpView ��ͼ
void CBmpView::OnDraw(CDC* pDC)
{
	CBmpDocument* pDoc = (CBmpDocument*)GetDocument();
	CDib dib;
	dib.LoadFromFile(pDoc->GetPathName());
	dib.MakePalette();
	CSize size=dib.GetDimension();
	dib.Draw(pDC,(0,0),size);
}