
// MessageManager.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMessageManagerApp:
// �йش����ʵ�֣������ MessageManager.cpp
//

class CMessageManagerApp : public CWinAppEx
{
public:
	CMessageManagerApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMessageManagerApp theApp;