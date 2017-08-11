//NetworkGroups 应用程序的主头文件
#pragma once
#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif
#include "resource.h"//主符号
// CNetworkGroupsApp:
class CNetworkGroupsApp:public CWinAppEx
{
public:
	CNetworkGroupsApp();
	CMultiDocTemplate* pDocTemplate;
	CMultiDocTemplate* pBmpTemplate;
// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
// 实现
	BOOL  m_bHiColorIcons;
	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnOverlapGroup();
	afx_msg void OnConvertPairs();
};

extern CNetworkGroupsApp theApp;