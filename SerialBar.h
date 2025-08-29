
#pragma once

#ifndef __AFXWIN_H__
	#error 
#endif

#include "resource.h"		


class CSerialBarApp : public CWinApp
{
public:
	CSerialBarApp();
	BOOL IsSettingsDlgVisible();
	void ShowSettingsDlg(void);
	int IsApplicationRunned(const char* appName);	
	virtual BOOL InitInstance();

private:
	DECLARE_MESSAGE_MAP()
	


};

extern CSerialBarApp theApp;
