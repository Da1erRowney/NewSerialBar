#import "../SerialServer/_SerialServer.tlb" raw_interfaces_only, no_namespace, named_guids

#include "notifyicon.h"
#include "afxcmn.h"
#include "Settings.h"
#include "SettingsDlg.h"

#pragma once

class CSerialBar : public CWnd
{

public:
	CSerialBar();
	~CSerialBar();
		
private:
	HICON m_hIcon;
	IServer *_iSrv;
	IManager *_iManager;
	CNotifyIcon *_trayIcon;
	CSettings *_settings;
	BYTE _selectedPort;
	
private:
	void InitTray();	
	void InitCOM();
	
protected:
	afx_msg void OnExit();
	afx_msg void OnPorts();
	afx_msg void OnAbout();

	DECLARE_MESSAGE_MAP()		
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
public:
	void ShowSettingsDlg(void);
};
