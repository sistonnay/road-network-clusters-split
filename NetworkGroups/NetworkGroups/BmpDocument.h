#pragma once

// CBmpDocument 文档

class CBmpDocument : public CDocument
{
	DECLARE_DYNCREATE(CBmpDocument)
public:
	HBITMAP hbitmap;

public:
	CBmpDocument();
	virtual ~CBmpDocument();
#ifndef _WIN32_WCE
	virtual void Serialize(CArchive& ar);   // 为文档 I/O 重写
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual BOOL OnNewDocument();
	DECLARE_MESSAGE_MAP()
};
