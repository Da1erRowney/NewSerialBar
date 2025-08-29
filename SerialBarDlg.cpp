#include "stdafx.h"
#include "SerialBar.h"
#include "SerialBarDlg.h"
#include "SettingsDlg.h"
#include "AboutDlg.h"
#include "log.h"
#include ".\serialbardlg.h"

 
#define WM_NOTIFYICON WM_USER + 0x100
#define WM_SHOWPORTSETTINGS WM_USER + 0x101


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CSerialBar::CSerialBar()

{	
	WriteLog("CSerialBar");
	InitCOM();	
	InitTray();

	_settings = NULL;
	_selectedPort = 0;
	
	try
	{
	
		CSettings::LoadAll(NULL,&_settings);
		CSettings::SaveAll(NULL,_settings,TRUE);
		
	}
	catch (HRESULT e)
	{
		AfxMessageBox("Файл настроек поврежден");
		ExitProcess(- 1);
	}
	
	
	
	

}

void CSerialBar::InitCOM()
{
		WriteLog("InitCOM");
	_iManager = NULL;
	_iSrv = NULL;
	
	CoInitializeEx(NULL,COINIT_MULTITHREADED);				
	HRCALL(CoCreateInstance(__uuidof(CServer),NULL,CLSCTX_ALL,__uuidof(IServer),(void**)&_iSrv),"Получить ISrv");
	HRCALL(_iSrv->QueryInterface(__uuidof(_iManager),(void**)&_iManager),"Получить IManager");

}


	 

void CSerialBar::InitTray()
{
	_trayIcon = NULL;
	WriteLog("InitTray");
	LPCSTR cc = AfxRegisterWndClass(0);
	CreateEx(WS_EX_TOOLWINDOW,AfxRegisterWndClass(0),NULL,!WS_VISIBLE | WS_CHILD,CRect(0,0,1,1),CWnd::GetDesktopWindow(),1983,NULL);
	_trayIcon = new CNotifyIcon(this->m_hWnd,IDI_PORT,WM_NOTIFYICON);
	_trayIcon->SetMenu(NULL,IDR_MAINMENU);
	_trayIcon->SetTip("Настройка COM порта");
	this->SetWindowText("SERIALBAR");
	ShowWindow(SW_SHOWNORMAL);
	UpdateWindow();
}
 
 
CSerialBar::~CSerialBar()
{ 
	
	if (_iSrv)
	{
		while (_iSrv->Release())
		{
		}
	}
	
	//_iManager ? _iManager->Release() : FALSE;
	//_iSrv ? _iSrv->Release() : FALSE;
	CoUninitialize();
	_trayIcon ? delete _trayIcon : FALSE;
}


BEGIN_MESSAGE_MAP(CSerialBar, CWnd)
	
	//}}AFX_MSG_MAP
	
	ON_COMMAND(ID_EXIT, OnExit)
	ON_COMMAND(ID_PORTS, OnPorts)
	ON_COMMAND(ID_ABOUT,OnAbout)
END_MESSAGE_MAP()


void CSerialBar::OnExit()
{
	
	this->DestroyWindow();
	/*if(IDYES == MessageBox("Принудительное завершение SerialServer приведет к аварийному завершению клиентов. Продолжить ?","SerialBar - ВНИМАНИЕ",MB_YESNO | MB_DEFBUTTON2))
	{
		
		_trayIcon->Remove();
		HRCALL(_iManager->Kill(),"Завершить работу сервера");
	
		
		this->DestroyWindow();
		
	}*/
	
 }

void CSerialBar::OnAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


void CSerialBar::OnPorts()
{
	if (!theApp.IsSettingsDlgVisible())
	{
		ShowSettingsDlg();
		_selectedPort = 0;
		
	}
	
}

BOOL CSerialBar::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	_trayIcon ? _trayIcon->Dispatch(this->m_hWnd,message,wParam,lParam) : FALSE;

	if (WM_SHOWPORTSETTINGS == message)
	{
		_selectedPort = (BYTE)wParam;
		OnPorts();
		
	}
	return CWnd::OnWndMsg(message, wParam, lParam, pResult);
}

void CSerialBar::ShowSettingsDlg(void)
{
	CSettingsDlg dlg(_iSrv,_iManager,_settings,_selectedPort);
	dlg.DoModal();
	
}
