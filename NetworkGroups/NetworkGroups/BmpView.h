#pragma once


// CBmpView 视图
class CBmpView : public CScrollView
{
	DECLARE_DYNCREATE(CBmpView)

protected:
	CBmpView();           // 动态创建所使用的受保护的构造函数
	virtual ~CBmpView();
	DECLARE_MESSAGE_MAP();

#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif
	
public:
	virtual void OnDraw(CDC* pDC); // 重写以绘制该视图
	virtual void OnInitialUpdate();
};


