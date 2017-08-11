#pragma once
#include <vector>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "MainFrm.h"
#include "TextDocument.h"
using namespace std;
// CTextView 视图
class CTextView : public CEditView
{
	DECLARE_DYNCREATE(CTextView)
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

private://成员变量

	CTextDocument* pDoc;

public://成员函数

	CTextView();
	virtual ~CTextView();

protected:

	virtual void OnUpdate(CView*, LPARAM, CObject*);
	afx_msg void OnNetGroup();
	DECLARE_MESSAGE_MAP()

};

