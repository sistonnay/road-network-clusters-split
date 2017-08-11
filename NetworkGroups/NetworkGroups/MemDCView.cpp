// MemDCView.cpp : ʵ���ļ�
#include "stdafx.h"
#include "NetworkGroups.h"
#include "MemDCView.h"

// MemDCView
IMPLEMENT_DYNCREATE(MemDCView, CScrollView)

MemDCView::MemDCView(){}
MemDCView::~MemDCView(){}

BEGIN_MESSAGE_MAP(MemDCView, CScrollView)
END_MESSAGE_MAP()

// MemDCView ��ͼ
void MemDCView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
	CSize sizeTotal;
	// TODO: �������ͼ�ĺϼƴ�С
	sizeTotal.cx = sizeTotal.cy = 1000;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

void MemDCView::OnDraw(CDC* pDC)
{
	//��ȡ��ǰ��ͼ��Ӧ�ĵ���һ�ַ�ʽ
	CBmpDocument* pDoc =(CBmpDocument*)GetDocument();
	// TODO: �ڴ���ӻ��ƴ���
	CBitmap hbmp;
	HBITMAP hbitmap=pDoc->hbitmap;
	//װ��ͼƬ�ļ�MM.bmp
	//hbitmap=(HBITMAP)::LoadImage(::AfxGetInstanceHandle(),"MM.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION);
	hbmp.Attach(hbitmap);
	//��ȡͼƬ��ʽ
	BITMAP bm;
	hbmp.GetBitmap(&bm);
	CDC MemDC;
	MemDC.CreateCompatibleDC(GetDC());
	CBitmap * poldBitmap=(CBitmap*)MemDC.SelectObject(hbmp);
	//CRect lRect;//��ȡ�ͻ����ߴ�
	//GetClientRect(&lRect);
	//lRect.NormalizeRect();
	//��ʾλͼ
	pDC->BitBlt(0,0,bm.bmWidth,bm.bmHeight,&MemDC,0,0,SRCCOPY);//
	MemDC.SelectObject(&poldBitmap);
	hbmp.Detach();//����Ҫ,��Ȼbitmap���������
}


// MemDCView ���
#ifdef _DEBUG
void MemDCView::AssertValid() const
{
	CScrollView::AssertValid();
}

#ifndef _WIN32_WCE
void MemDCView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif
#endif //_DEBUG

// MemDCView ��Ϣ�������