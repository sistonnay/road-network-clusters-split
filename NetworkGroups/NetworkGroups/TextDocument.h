#pragma once

// CTextDocument �ĵ�
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
	virtual void Serialize(CArchive& ar);   // Ϊ�ĵ� I/O ��д
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
