// WebEnviromentCheck.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error PCH���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����Ͻʽÿ�.
#endif

#include "resource.h"		// �� ��ȣ


// CWebEnviromentCheckApp:
// �� Ŭ������ ������ ���ؼ��� WebEnviromentCheck.cpp�� �����Ͻʽÿ�.
//

class CWebEnviromentCheckApp : public CWinApp
{
public:
	CWebEnviromentCheckApp();

// ������
	public:
	virtual BOOL InitInstance();

// ����

	DECLARE_MESSAGE_MAP()
};

extern CWebEnviromentCheckApp theApp;
