//NetworkGroups Ӧ�ó������ͷ�ļ�
#pragma once
#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif
#include "resource.h"//������
// CNetworkGroupsApp:
class CNetworkGroupsApp:public CWinAppEx
{
public:
	CNetworkGroupsApp();
	CMultiDocTemplate* pDocTemplate;
	CMultiDocTemplate* pBmpTemplate;
// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
// ʵ��
	BOOL  m_bHiColorIcons;
	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnOverlapGroup();
	afx_msg void OnConvertPairs();
};

extern CNetworkGroupsApp theApp;