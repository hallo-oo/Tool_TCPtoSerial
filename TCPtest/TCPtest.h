
// TCPtest.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CTCPtestApp:
// �йش����ʵ�֣������ TCPtest.cpp
//

class CTCPtestApp : public CWinApp
{
public:
	CTCPtestApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CTCPtestApp theApp;