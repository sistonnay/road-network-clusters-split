#include "stdafx.h"
#include "NetworkGroups.h"
#include "TextDocument.h"

// CTextDocument
IMPLEMENT_DYNCREATE(CTextDocument, CDocument)

CTextDocument::CTextDocument()
{
	n_Text="";
	pf=NULL;
}

BOOL CTextDocument::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CTextDocument::~CTextDocument(){}

BEGIN_MESSAGE_MAP(CTextDocument, CDocument)
END_MESSAGE_MAP()

// CTextDocument ���
#ifdef _DEBUG
void CTextDocument::AssertValid() const
{
	CDocument::AssertValid();
}

#ifndef _WIN32_WCE
void CTextDocument::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif
#endif //_DEBUG

// CTextDocument ����
BOOL CTextDocument::OnOpenDocument(LPCTSTR lpszPathName)
{
	if(!CDocument::OnOpenDocument(lpszPathName)) 
		return FALSE;
	//TODO:�ڴ������ר�õĴ�������
	return TRUE;
}
#ifndef _WIN32_WCE

// CTextDocument ���л�
void CTextDocument::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{	}
	else
	{
		pf=ar.GetFile();//ͨ��pf�����Ի�ȡ�ļ�������Ϣ
		while(ar.ReadString(m_Text))
		{
			n_Text=n_Text+m_Text+"\r\n";
		}
	}
}
#endif

