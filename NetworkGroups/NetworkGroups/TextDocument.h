#pragma once

// CTextDocument 文档
class CTextDocument : public CDocument
{
	DECLARE_DYNCREATE(CTextDocument)
public:
	CString n_Text,m_Text;
	CFile* pf;
public:
	CTextDocument();
	virtual ~CTextDocument();
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
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
};
