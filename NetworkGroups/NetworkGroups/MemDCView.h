#pragma once
#include "BmpDocument.h"


// MemDCView ��ͼ

class MemDCView : public CScrollView
{
	DECLARE_DYNCREATE(MemDCView)

protected:
	MemDCView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~MemDCView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void OnDraw(CDC* pDC);      // ��д�Ի��Ƹ���ͼ
	virtual void OnInitialUpdate();     // �����ĵ�һ��

	DECLARE_MESSAGE_MAP()
};


