// qq_auto_login.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������

#include "LoadConfig.h"
// Cqq_auto_loginApp:
// �йش����ʵ�֣������ qq_auto_login.cpp
//

class Cqq_auto_loginApp : public CWinApp
{
public:
	Cqq_auto_loginApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern Cqq_auto_loginApp theApp;
