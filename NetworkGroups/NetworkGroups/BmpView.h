#pragma once


// CBmpView ��ͼ
class CBmpView : public CScrollView
{
	DECLARE_DYNCREATE(CBmpView)

protected:
	CBmpView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CBmpView();
	DECLARE_MESSAGE_MAP();

#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif
	
public:
	virtual void OnDraw(CDC* pDC); // ��д�Ի��Ƹ���ͼ
	virtual void OnInitialUpdate();
};


