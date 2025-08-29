// ClientsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SerialBar.h"
#include "ClientsDlg.h"
#include ".\clientsdlg.h"


// CClientsDlg dialog

IMPLEMENT_DYNAMIC(CClientsDlg, CDialog)
CClientsDlg::CClientsDlg(CWnd* pParent /*=NULL*/,BYTE portNum,SAFEARRAY *clientNames)
	: CDialog(CClientsDlg::IDD, pParent)
{
	_caption.Format("Клиенты порта N%d",portNum);
	_clientNames = clientNames;
}

CClientsDlg::~CClientsDlg()
{
}

void CClientsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CLIENT, _clientsList);
}


BEGIN_MESSAGE_MAP(CClientsDlg, CDialog)
END_MESSAGE_MAP()


// CClientsDlg message handlers

BOOL CClientsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetWindowText(_caption);
	char* names[MAX_CLIENT];
	for (int i = 0; i < MAX_CLIENT; i++)
	{
		names[i] = new char[BUFSIZ];
	}

	int clientNamesCnt = get_ClientNames(_clientNames,names);

	for (int j = 0; j < clientNamesCnt; j++)
	{
		_clientsList.AddString(names[j]);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE


}
int CClientsDlg::get_ClientNames(SAFEARRAY* clientNames,char* ret[MAX_CLIENT])
{

	USES_CONVERSION;
	if (clientNames)
	{
		for (UINT i = 0; i < clientNames->rgsabound[0].cElements; i++)
		{
			CComBSTR clientNameOLE;
			SafeArrayGetElement(clientNames,(LONG*)&i,&clientNameOLE);
			char* clientNameText = OLE2A(clientNameOLE.Detach());
			strcpy(ret[i],clientNameText);
		}
	}

	return clientNames ? clientNames->rgsabound[0].cElements : 0;
}



