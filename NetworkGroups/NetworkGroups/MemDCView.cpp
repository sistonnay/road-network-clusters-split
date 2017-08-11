// MemDCView.cpp : 实现文件
#include "stdafx.h"
#include "NetworkGroups.h"
#include "MemDCView.h"

// MemDCView
IMPLEMENT_DYNCREATE(MemDCView, CScrollView)

MemDCView::MemDCView(){}
MemDCView::~MemDCView(){}

BEGIN_MESSAGE_MAP(MemDCView, CScrollView)
END_MESSAGE_MAP()

// MemDCView 绘图
void MemDCView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
	CSize sizeTotal;
	// TODO: 计算此视图的合计大小
	sizeTotal.cx = sizeTotal.cy = 1000;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

void MemDCView::OnDraw(CDC* pDC)
{
	//获取当前视图对应文档的一种方式
	CBmpDocument* pDoc =(CBmpDocument*)GetDocument();
	// TODO: 在此添加绘制代码
	CBitmap hbmp;
	HBITMAP hbitmap=pDoc->hbitmap;
	//装载图片文件MM.bmp
	//hbitmap=(HBITMAP)::LoadImage(::AfxGetInstanceHandle(),"MM.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION);
	hbmp.Attach(hbitmap);
	//获取图片格式
	BITMAP bm;
	hbmp.GetBitmap(&bm);
	CDC MemDC;
	MemDC.CreateCompatibleDC(GetDC());
	CBitmap * poldBitmap=(CBitmap*)MemDC.SelectObject(hbmp);
	//CRect lRect;//获取客户区尺寸
	//GetClientRect(&lRect);
	//lRect.NormalizeRect();
	//显示位图
	pDC->BitBlt(0,0,bm.bmWidth,bm.bmHeight,&MemDC,0,0,SRCCOPY);//
	MemDC.SelectObject(&poldBitmap);
	hbmp.Detach();//很重要,不然bitmap会跟着销毁
}


// MemDCView 诊断
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

// MemDCView 消息处理程序