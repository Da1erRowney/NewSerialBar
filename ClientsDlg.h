#pragma once
#include "afxwin.h"


// CClientsDlg dialog

class CClientsDlg : public CDialog
{
	DECLARE_DYNAMIC(CClientsDlg)

public:
	CClientsDlg(CWnd* pParent = NULL,BYTE portNum = 0,SAFEARRAY* clientNames = NULL);   // standard constructor
	
	virtual ~CClientsDlg();

// Dialog Data
	enum { IDD = IDD_CLIENTS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	enum{MAX_CLIENT = 32};
	DECLARE_MESSAGE_MAP()
private:
	CListBox _clientsList;
	CString _caption;
	SAFEARRAY *_clientNames;
	int get_ClientNames(SAFEARRAY* clientNames,char* ret[MAX_CLIENT]);
	

public:
	virtual BOOL OnInitDialog();
};
