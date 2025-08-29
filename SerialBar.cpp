#include "stdafx.h"
#include "SerialBar.h"
#include "SerialBarDlg.h"
#include "Tlhelp32.h"
#include ".\serialbar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CSerialBarApp

BEGIN_MESSAGE_MAP(CSerialBarApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()



CSerialBarApp::CSerialBarApp()
{

	
}


CSerialBarApp theApp;



/////////////////////////////////////////////////Z/////////////////////////
//Проверяет открыт ли диалог настройки порта, если да то центрируем его.
BOOL CSerialBarApp::IsSettingsDlgVisible()
{
	BOOL ret = FALSE;

	CWnd* pDlg = CWnd::FindWindow("#32770","Список портов");
	if (pDlg)
	{
		pDlg->SetForegroundWindow();
		pDlg->BringWindowToTop();
		pDlg->ShowWindow(SW_RESTORE);
		ret = TRUE;
	}
	return ret;
}


BOOL CSerialBarApp::InitInstance()
{
	WriteLog("InitInstance");
	
	AfxEnableControlContainer();
	InitCommonControls();
	CWinApp::InitInstance();

	if (1 == IsApplicationRunned("SerialBar.exe")) 
	{
		if (IsApplicationRunned("SerialServer.exe"))
		{
	
		}

		CSerialBar mainWnd;         //Запускаем...
		m_pMainWnd = &mainWnd;
		this->Run();
	}
	else {
		IsSettingsDlgVisible();
	} 
	
		
	
	
	return TRUE;
}

void CSerialBarApp::ShowSettingsDlg(void)
{
	m_pMainWnd ? ((CSerialBar*)m_pMainWnd)->ShowSettingsDlg() : FALSE;
}

//////////////////////////////////////////////////////////////////////////
//Проверяет сколько экземпляров программы запущено
int CSerialBarApp::IsApplicationRunned(const char* appName)
{

	int prcsCnt = 0;
	PROCESSENTRY32 prcs;

	HANDLE  shapshot = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, NULL);
	prcs.dwSize=sizeof(PROCESSENTRY32);
	Process32First(shapshot,&prcs);

	while(Process32Next(shapshot,&prcs))
	{	
		if (!strcmp(appName,prcs.szExeFile))
		{
			prcsCnt++;
		}
	}

	
	return prcsCnt;
}
