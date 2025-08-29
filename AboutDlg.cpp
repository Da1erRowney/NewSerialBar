// AboutDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SerialBar.h"
#include "AboutDlg.h"
#include ".\aboutdlg.h"

#include <WinUser.h>

// CAboutDlg dialog

IMPLEMENT_DYNAMIC(CAboutDlg, CDialog)
CAboutDlg::CAboutDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAboutDlg::IDD, pParent)
	
{

	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));       // zero out structure
	lf.lfHeight = 16;                      // request a 12-pixel-height font
	strcpy(lf.lfFaceName, "Arial");        // request a face name "Arial"
	_urlFont.CreateFontIndirect(&lf);
		
	_handCursor = theApp.LoadCursor(IDC_HAND);

}

CAboutDlg::~CAboutDlg()
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_MSG, _msg);

}


BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)

	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CAboutDlg message handlers

void CAboutDlg::CenterDialog()
{
	CWnd *desktop = GetDesktopWindow();
	CRect deskRect;
	CRect dlgRect;
	desktop->GetWindowRect(&deskRect);
	this->GetWindowRect(&dlgRect);

	this->SetWindowPos(&CWnd::wndBottom,
		(deskRect.Width() - dlgRect.Width() ) / 2,
		(deskRect.Height()- dlgRect.Height() ) / 2 ,
		0,
		0,
		SWP_NOSIZE | SWP_SHOWWINDOW);

}

void CAboutDlg::LoadAboutMessage(CListBox* list)
{
	list->AddString("      SerialBar версия 1.0.0.6, 16.01.2007          ");
	list->AddString("                                                ");
	list->AddString(" Разработана РУП БелЭМН. Используется         ");
	list->AddString("совместно с компонентом SerialServer вер-       ");
	list->AddString("сия 1.0.0.3 для управления и контроля        ");
	list->AddString("COM -порта. ");
	
}

void CAboutDlg::CreateLogoPicture()
{
	_logo = new CBitmapButton();
	_logo->Create(NULL, WS_CHILD|WS_VISIBLE|BS_OWNERDRAW|WS_BORDER, 
		CRect(10,10,110,110), this, 1);
	_logo->LoadBitmaps(IDB_LOGO);
	_logo->EnableWindow(FALSE);
	
}

BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CenterDialog();
	CreateLogoPicture();
	LoadAboutMessage(&_msg);
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CAboutDlg::LoadSite(void)
{
	HKEY hKey;
	DWORD size;
	char browserName[BUFSIZ];
	char browserPath[BUFSIZ];
	char buffer[BUFSIZ];
		
	DWORD lRet = RegOpenKeyEx( HKEY_USERS,
		"S-1-5-21-299502267-583907252-839522115-1003\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\.html\\OpenWithList",
		0, KEY_QUERY_VALUE, &hKey );
	RegQueryValueEx( hKey, "a", NULL, NULL,
		(LPBYTE)browserName, &size);
	RegCloseKey(hKey);
	

	
	sprintf(buffer,"%s","SOFTWARE\\Classes\\Applications\\Opera.exe\\shell\\open\\command");
	lRet = RegOpenKeyEx( HKEY_LOCAL_MACHINE,buffer,
		0, KEY_QUERY_VALUE, &hKey );
	size = 0;
	LONG type;
	lRet = RegQueryValue( hKey, "",(LPTSTR)browserPath, &type);
	
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory( &si, sizeof(si) );
	si.cb = sizeof(si);
	ZeroMemory( &pi, sizeof(pi) );
	strnset(&(browserPath[strlen(browserPath) - 5]),32,10);
	strcat(browserPath," www.belemn.com");
	
#ifdef _DEBUG
	AfxMessageBox(browserPath);
#endif
	if(CreateProcess( NULL, // No module name (use command line). 
		browserPath, // Command line. 
		NULL,             // Process handle not inheritable. 
		NULL,             // Thread handle not inheritable. 
		FALSE,            // Set handle inheritance to FALSE. 
		0,                // No creation flags. 
		NULL,             // Use parent's environment block. 
		NULL,             // Use parent's starting directory. 
		&si,              // Pointer to STARTUPINFO structure.
		&pi )             // Pointer to PROCESS_INFORMATION structure.
		) 
	{
		CloseHandle( pi.hProcess );
		CloseHandle( pi.hThread );	
	}
	

}

void CAboutDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	dc.SetBkMode(TRANSPARENT);
	dc.SelectObject(_urlFont);
	dc.SetTextColor(RGB(0,0,255));
	dc.TextOut(10,120,"www.belemn.com");

}



void CAboutDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	if (point.x >= 10 && point.x <= 110 && point.y <= 130 && point.y >= 120)
	{
		::SetCursor(_handCursor);
	}

	CDialog::OnMouseMove(nFlags, point);
}

void CAboutDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (point.x >= 5 && point.x <= 130 && point.y <= 140 && point.y >= 110)
	{
		LoadSite();
	}

	CDialog::OnLButtonDown(nFlags, point);
}


BOOL CAboutDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	AfxMessageBox("");

	return CDialog::OnNotify(wParam, lParam, pResult);
}
