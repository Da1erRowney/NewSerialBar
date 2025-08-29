// SettingsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SerialBar.h"
#include "SettingsDlg.h"
#include ".\settingsdlg.h"

#define SUCCESS 0
#define MEMORYALLOCATION_FAIL 1
#define BUSY 2
#define TIMEOUT 3

// CSettingsDlg dialog



IMPLEMENT_DYNAMIC(CSettingsDlg, CDialog)
CSettingsDlg::CSettingsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSettingsDlg::IDD, pParent)
	, _baudeRate(0)
	, _timeout(0)
	, _waitByte(0)
	, _waitRequest(0)
	, _stopBits(0)
	, _byteSize(0)
	, _parity(0)
	, _requestOn(0)
	, _requestOff(0)


{
}

CSettingsDlg::CSettingsDlg(IServer *iSrv,IManager *iManager,CSettings* settings,BYTE portNum) : CDialog(CSettingsDlg::IDD, NULL)
, _baudeRate(0)
, _timeout(0)
, _waitByte(0)
, _waitRequest(0)
, _stopBits(0)
, _byteSize(0)
, _parity(0)
, _requestOn(0)
, _requestOff(0)

{	
	
	_timer = NULL;
	this->_iSrv = iSrv;
	_settings = settings;
	_selectedPort = portNum;
	_iManager = iManager;

	try
	{
		_portCnt = (BYTE)CSettings::GetExistPorts(_ports);
	
	}


	catch (HRESULT e)
	{
		if (E_CONFIGURE == e)
		{
			AfxMessageBox("Файл с настройками повережден.");
			return;
		}
	}

		

	if (_portCnt > 0)
	{
		_currentPortNum = _ports[0];
	}
	
	
	
}


CSettingsDlg::~CSettingsDlg()
{

}

void CSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_CBIndex(pDX, IDC_BAUDE, _baudeRate);
	DDX_Text(pDX, IDC_TIMEOUT, _timeout);
	DDX_Text(pDX, IDC_WAITBYTE, _waitByte);
	DDX_Text(pDX, IDC_WAITREQUEST, _waitRequest);
	DDX_Radio(pDX,IDC_STOPBITS_1,_stopBits);
	DDX_Radio(pDX,IDC_PARITY_NO,_parity);
	DDX_Radio(pDX,IDC_BSIZE_4,_byteSize);

	DDX_Text(pDX, IDC_REQUESTON, _requestOn);
	DDX_Text(pDX, IDC_REQUESTOFF, _requestOff);
	DDX_Control(pDX, ID_APPLY, _applyButton);
	DDX_Control(pDX, IDOK, _okButton);
	DDX_Control(pDX, IDCANCEL, _cancelButton);

	DDX_Control(pDX, IDC_PORTLIST, _portList);
}


BEGIN_MESSAGE_MAP(CSettingsDlg, CDialog)
	ON_CBN_SELCHANGE(IDC_BAUDE, OnCbnSelchangeBaude)
	ON_EN_CHANGE(IDC_WAITBYTE, OnEnChangeWaitbyte)
	ON_EN_CHANGE(IDC_WAITREQUEST, OnEnChangeWaitrequest)
	ON_EN_CHANGE(IDC_TIMEOUT, OnEnChangeTimeout)
	ON_BN_CLICKED(IDC_PARITY_NO, OnBnClickedParity)
	ON_BN_CLICKED(IDC_PARITY_TRUE, OnBnClickedParity)
	ON_BN_CLICKED(IDC_PARITY_FALSE, OnBnClickedParity)
	ON_BN_CLICKED(IDC_PARITY_MARKER, OnBnClickedParity)
	ON_BN_CLICKED(IDC_PARITY_SPACE, OnBnClickedParity)
	ON_BN_CLICKED(IDC_BSIZE_4, OnBnClickedBsize)
	ON_BN_CLICKED(IDC_BSIZE_5, OnBnClickedBsize)
	ON_BN_CLICKED(IDC_BSIZE_6, OnBnClickedBsize)
	ON_BN_CLICKED(IDC_BSIZE_7, OnBnClickedBsize)
	ON_BN_CLICKED(IDC_BSIZE_8, OnBnClickedBsize)
	ON_BN_CLICKED(IDC_STOPBITS_1, OnBnClickedStopbits)
	ON_BN_CLICKED(IDC_STOPBITS_15, OnBnClickedStopbits)
	ON_BN_CLICKED(IDC_STOPBITS_2, OnBnClickedStopbits)
	ON_EN_CHANGE(IDC_REQUESTON, OnEnChangeRequeston)
	ON_EN_CHANGE(IDC_REQUESTOFF, OnEnChangeRequestoff)
	ON_BN_CLICKED(ID_APPLY, OnBnClickedApply)

	ON_NOTIFY(NM_CLICK, IDC_PORTLIST, OnNMClickPortlist)
	ON_NOTIFY(NM_DBLCLK, IDC_PORTLIST, OnNMDblclkPortlist)
	ON_WM_TIMER()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CSettingsDlg message handlers

BOOL CSettingsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	try
	{
		CSettings::LoadAll(NULL,&_settings);
	}
	catch (HRESULT e)
	{
		//Порт не открылся
		if (2 == HRESULT_SEVERITY(e))
		{
			AfxMessageBox("Порт");
		}
		else
		{
			AfxMessageBox("Файл настроек поврежден");
		}
	
	}
	
	_applyButton.SetWindowText("Принять");
	_applyButton.EnableWindow(FALSE);
	_okButton.SetWindowText("Ввод");
	_cancelButton.SetWindowText("Отмена");
	CenterDialog();

	InitImageList();
	InitPortList();
	FillPortList();

	if (_portCnt > 0)
	{
		_selectedPort ? SetHotPort(_selectedPort) : SetHotPort(_currentPortNum = _ports[0]);
		
		DrawSettings();

	}
	
	
	_timer = this->SetTimer(1,10000,0);
	return TRUE;  // return TRUE unless you set the focus to a control

}

BOOL CSettingsDlg::InitImageList(void)
{
	BOOL ret = TRUE;

	_portImages.Create(32,32,ILC_COLOR16,0,3);
	try
	{

		HICON hIcon = AfxGetApp()->LoadIcon(IDI_PORT);
		_portImages.Add(hIcon);
		hIcon = AfxGetApp()->LoadIcon(IDI_PORTSTOPPED);
		_portImages.Add(hIcon);
		hIcon = AfxGetApp()->LoadIcon(IDI_PORTRUNNED);
		_portImages.Add(hIcon);

	}
	catch (...)
	{
		ret = FALSE;
	}


	return ret;
}


BOOL CSettingsDlg::InitPortList(void)
{
	enum{MAX_COLUMN = 4};
	char* _columnNames[MAX_COLUMN] = {"Порт","Статус","Клиентов","Имена клиентов"};
	int  _columnWidths[MAX_COLUMN] = {_portList.GetStringWidth(_columnNames[0]) + 44,
										(int)(_portList.GetStringWidth(_columnNames[1]) * 2.5),
										(int)(_portList.GetStringWidth(_columnNames[2]) * 1.5 ),	
										(int)(_portList.GetStringWidth(_columnNames[3]) + 157)};

	BOOL ret = TRUE;
	try
	{
		_portList.SetImageList(&_portImages,LVSIL_SMALL);
		
		for (int i = 0; i < MAX_COLUMN; i++)
		{
			_portList.InsertColumn(i,_columnNames[i],LVCFMT_LEFT,_columnWidths[i]);
		}
		
	}
	catch (...) 
	{
		ret = FALSE;
	}

	_portList.SetExtendedStyle(LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT );
	return ret;
}






// [8/3/2006]---------------------------------------------------------------------------------------

void CSettingsDlg::FillPortList(void)
{
	int j = 0;
	/*	_portCnt = 4;
		_ports[1] = 2;
		_ports[2] = 3;
		_ports[3] = 4;*/
	for (ULONG i = 0;i < _portCnt; i++)
	{
		char* clientName[32];
		for (j = 0; j < 32;j++)
		{
			clientName[j] = new char[BUFSIZ];
		}
		
		int namesCnt = GetClientName(_ports[i],clientName);
		WriteLog("SerialBar: _portCnt = %d",_portCnt);
		if (_settings)
		{
			AddPortItem(_ports[i],_settings[_ports[i]]._bForceClosed,namesCnt,clientName,namesCnt);
		}

		for (j = 0; j < 32;j++)
		{
			delete clientName[j];
		}

	}

	
}

ULONG CSettingsDlg::GetClientName(BYTE portNum,char* inClientName[32])
{
	SAFEARRAY* cnSafe = NULL;
	theApp.BeginWaitCursor();
	HRCALL(_iSrv->GetClientNames(portNum,&cnSafe),"GetClientNames FAILED");

	
	theApp.EndWaitCursor();
	
	USES_CONVERSION;
	if (cnSafe)
	{
		for (UINT i = 0; i < cnSafe->rgsabound[0].cElements; i++)
		{
			CComBSTR clientNameOLE;
			SafeArrayGetElement(cnSafe,(LONG*)&i,&clientNameOLE);
			char* clientNameText = OLE2A(clientNameOLE.Detach());
			strcpy(inClientName[i],clientNameText);
		}
	}

	return cnSafe ? cnSafe->rgsabound[0].cElements : 0;
	
}


void CSettingsDlg::AddPortItem(BYTE portNum,BOOL bForceClosed, int clientCnt,char* clientName[32],int namesCnt)
{
	char buf[BUFSIZ];
	int iItem = _portList.GetItemCount();

	LVITEM item;
	item.iItem = iItem;
	item.iSubItem = 0;
	item.mask = LVIF_TEXT | LVIF_IMAGE;
	sprintf(buf,"COM%d",portNum);
	item.pszText = buf;
	item.iImage = GetImageIndex(bForceClosed,clientCnt);
	_portList.InsertItem(&item);

	item.iSubItem = 1;
	item.mask = LVIF_TEXT;
	GetStatusString(bForceClosed,clientCnt,buf);
	item.pszText = buf;
	_portList.SetItem(&item);

	item.iSubItem = 2;
	item.mask = LVIF_TEXT;
	sprintf(buf,"%d",clientCnt);
	item.pszText = buf;
	_portList.SetItem(&item);

	ZeroMemory(buf,sizeof(buf));
	for (int i = 0; i < namesCnt; i++)
	{
		char tmp[BUFSIZ];
		sprintf(tmp,"%s; ",clientName[i]);
		strcat(buf,tmp);
	}

	item.iSubItem = 3;
	item.mask = LVIF_TEXT;
	item.pszText = buf;
	_portList.SetItem(&item);

	if (namesCnt)
	{
		_portList.SetColumnWidth(3, _portList.GetStringWidth(buf) + 30);
	}
	else
	{
		_portList.SetColumnWidth(3,_portList.GetStringWidth("Имена клиентов") + 157);
	}
	


	DWORD itemData = portNum;
	_portList.SetItemData(iItem,itemData);
}

int CSettingsDlg::GetImageIndex(BOOL bForceClosed, int clientCnt)
{
	int ret = -1;
	if (bForceClosed)
	{
		ret = 1;
	} else if (clientCnt)
	{
		ret = 2;
	} else
	{
		ret = 0;
	}

	return ret;
}

void CSettingsDlg::GetStatusString(BOOL bForceClosed, int clientCnt,char* inStatus)
{

	if (bForceClosed)
	{
		strcpy(inStatus,"Закрыт");
	} 
	else if (clientCnt)
	{
		strcpy(inStatus,"Используется");
	} else
	{
		strcpy(inStatus,"");
	}

}


CSettings* CSettingsDlg::GetSettings(BYTE portNum)
{
	CSettings* ret = NULL;
	for (int i = 0; i < _portCnt; i++)
	{
		if (_settings[i]._portNum == portNum)
		{
			ret = &_settings[i];
		}
	}
	return ret;
}


//[8/3/2006]-----------------------------------------------------------------------------------------

void CSettingsDlg::OnOK()
{
	
	this->ApplySettings();
	CDialog::OnOK();
}

//[8/3/2006]-----------------------------------------------------------------------------------------

void CSettingsDlg::DrawSettings( )
{
	
	_baudeRate = ToControlBaude(_settings[_currentPortNum]._baudeRate);
	_byteSize  = ToControlByteSize(_settings[_currentPortNum]._byteSize);
	_parity    = _settings[_currentPortNum]._parity;
	_stopBits  = _settings[_currentPortNum]._stopBits;
	_timeout   = _settings[_currentPortNum]._timeout;
	_waitByte  = _settings[_currentPortNum]._waitByte;
	_waitRequest = _settings[_currentPortNum]._waitRequest;
	_requestOff = _settings[_currentPortNum]._responseOff;
	_requestOn = _settings[_currentPortNum]._responseOn;

	UpdateData(FALSE);
	
	
}

// [8/3/2006] ------------------------------------------------------------------------------------------

int CSettingsDlg::ToControlBaude(DWORD baudeRate)
{
	int ret = 0;
	switch(baudeRate)
	{
		case 1200:
			ret = 0;
			break;
		case 2400:
			ret = 1;
			break;
		case 4800:
			ret = 2;
			break;
		case 9600:
			ret = 3;
			break;
		case 19200:
			ret = 4;
			break;
		case 38400:
			ret = 5;
			break;
		case 57600:
			ret = 6;
			break;
		case 115200:
			ret = 7;
			break;
		case 115200 * 2:
			ret = 8;
			break;
		case 115200 * 4:
			ret = 9;
			break;
		case 115200 * 8:
			ret = 10;
			break;
		default:
			ret = 0;
	}	
	return ret;
}




int CSettingsDlg::ToControlByteSize(BYTE byteSize)
{
	int ret = 0;
	switch(byteSize) {
		case 4:
			ret = 0;
			break;
		case 5:
			ret = 1;
			break;
		case 6:
			ret = 2;
			break;
		case 7:
			ret = 3;
			break;
		case 8:
			ret = 4;
			break;
		default:
			ret = 0;
			break;
	}
	return ret;
}

DWORD CSettingsDlg::ToSettingsBaude(int baudeRate)
{
	DWORD ret = 115200;
	switch(baudeRate)
	{
	case 0:
		ret = 1200;
		break;
	case 1:
		ret = 2400;
		break;
	case 2:
		ret = 4800;
		break;
	case 3:
		ret = 9600;
		break;
	case 4:
		ret = 19200;
		break;
	case 5:
		ret = 38400;
		break;
	case 6:
		ret = 57600;
		break;
	case 7:
		ret = 115200;
		break;
	case 8:
		ret = 115200*2;
		break;
	case 9:
		ret = 115200*4;
		break;
	case 10:
		ret = 115200*8;
		break;
	default:
		ret = 115200;
	}	
	return ret;

}

BYTE CSettingsDlg::ToSettingsByteSize(int byteSize)
{
	BYTE ret = 4;
	switch(byteSize) {
		case 0:
			ret = 4;
			break;
		case 1:
			ret = 5;
			break;
		case 2:
			ret = 6;
			break;
		case 3:
			ret = 7;
			break;
		case 4:
			ret = 8;
			break;
		default:
			ret = 4;
			break;
	}
	return ret;
}

bool CSettingsDlg::TurnOnApply()
{
	bool turnOn = (0 != _waitByte && 0 != _waitRequest);
	_okButton.EnableWindow(turnOn);
	_applyButton.EnableWindow(turnOn);
	turnOn ? SetWindowText("Список портов") : SetWindowText("Ожидание ответа и ожидание байта должны быть неравны 0") ;
	return turnOn;
}


void CSettingsDlg::OnCbnSelchangeBaude()
{
	UpdateData(TRUE);
    _settings[_currentPortNum]._baudeRate = ToSettingsBaude(_baudeRate);
}

void CSettingsDlg::OnEnChangeWaitbyte()
{
	UpdateData(TRUE);
	TurnOnApply() ? _settings[_currentPortNum]._waitByte = _waitByte : FALSE;
	//_settings[_currentPortNum]._waitByte = _waitByte;
	
}

void CSettingsDlg::OnEnChangeWaitrequest()
{
	UpdateData(TRUE);
	TurnOnApply() ? _settings[_currentPortNum]._waitRequest = _waitRequest : FALSE;
	//_settings[_currentPortNum]._waitRequest = _waitRequest;
}

void CSettingsDlg::OnEnChangeTimeout()
{
	UpdateData(TRUE);
	_settings[_currentPortNum]._timeout = _timeout;
}

void CSettingsDlg::OnBnClickedParity()
{
	UpdateData(TRUE);
	_settings[_currentPortNum]._parity = _parity;
}

void CSettingsDlg::OnBnClickedBsize()
{
	UpdateData(TRUE);
	_settings[_currentPortNum]._byteSize  = ToSettingsByteSize(_byteSize);
}

void CSettingsDlg::OnBnClickedStopbits()
{
	UpdateData(TRUE);
	_settings[_currentPortNum]._stopBits = _stopBits; 
}

void CSettingsDlg::OnEnChangeRequeston()
{
	UpdateData(TRUE);
	_settings[_currentPortNum]._responseOn = _requestOn;
}

void CSettingsDlg::OnEnChangeRequestoff()
{
	UpdateData(TRUE);
	_settings[_currentPortNum]._responseOff = _requestOff;
}

void CSettingsDlg::OnBnClickedApply()
{
	_applyButton.EnableWindow(FALSE);
	
	try
	{
		ApplySettings();
	}
	catch (HRESULT e)
	{
		if (E_CONFIGURE == e)
		{
			AfxMessageBox("Настройки невозможны. Перенастройте порт");
		}

	}
	
	
	
}


void CSettingsDlg::CenterDialog(void)
{
	CWnd *desktop = GetDesktopWindow();
	CRect deskRect;
	CRect dlgRect;
	desktop->GetWindowRect(&deskRect);
	this->GetWindowRect(&dlgRect);
	this->SetWindowPos(&CWnd::wndBottom,
		deskRect.Width() - dlgRect.Width(),
		deskRect.Height()- dlgRect.Height() - 100,
		0,
		0,
		SWP_NOSIZE | SWP_SHOWWINDOW);
}




void CSettingsDlg::ApplySettings()
{	
	theApp.BeginWaitCursor();
	this->SetWindowText("Порт переоткрывается. Пожалуйста, подождите");

	try
	{
		CSettings::SaveAll(NULL,_settings,FALSE);
	}
	catch (HRESULT e)
	{
		throw e;
	}

	//params.dlg = this;
	//params.settings = _settings;
	//params.callback = CSettingsDlg::OnApplyEnd(long result);
		
	//CWinThread* applyThread = AfxBeginThread(ApplySettingsThread,((void*)&params));
	
	this->SetWindowText("Список портов");
	//theApp.EndWaitCursor();
}

void CSettingsDlg::OnApplyEnd(long result)
{
	AfxMessageBox("callback");
}


/*UINT CSettingsDlg::ApplySettingsThread(void *param)
{
	ApplyThreadParams currentParam = *(ApplyThreadParams*)param;

	currentParam.dlg->SetWindowText("Порт переоткрывается. Пожалуйста, подождите");
	

	if(!IsWindow(currentParam.dlg->m_hWnd))
	{
		return TRUE;
	}
	HANDLE hMutex = OpenMutex( MUTEX_ALL_ACCESS,NULL,"SerialSettingsMutex");

	if(NULL == hMutex)
	{
		hMutex = CreateMutex(NULL,FALSE,"SerialSettingsMutex");
		if(NULL == hMutex)
		{
			AfxMessageBox("Внутренняя ошибка выделения памяти");
			//currentParam.(*callback)(MEMORYALLOCATION_FAIL);
			return -1;
		}
	}

	unsigned long operationResult = WaitForSingleObject(hMutex,5000);

	switch(operationResult)
	{
		case WAIT_OBJECT_0 :
			try
			{
				CSettings::SaveAll("serial.xml",currentParam.settings,FALSE);
			}
			catch(HRESULT e)
			{
				if(!ReleaseMutex(hMutex))
				{
					CString msg;
					//currentParam.callback(-1);
					msg.Format("Internal error %d",GetLastError());
					AfxMessageBox(msg);
					return FALSE;
				}
			}
			
			if(!ReleaseMutex(hMutex))
			{
				CString msg;
				//currentParam.callback(-1);
				msg.Format("Internal error %d",GetLastError());
				AfxMessageBox(msg);
				return FALSE;
			}
			
			//currentParam.callback(SUCCESS);
			currentParam.dlg->SetWindowText("Настройки изменены");
			break;
		case WAIT_TIMEOUT:
			//currentParam.callback(TIMEOUT);
			return FALSE;
		case WAIT_ABANDONED:
			//currentParam.callback(BUSY);
			currentParam.dlg->SetWindowText("Предыдущее дествие не завершено - изменение настроек");
			return FALSE;
			
	}

	
	return TRUE;
}
*/
BOOL CSettingsDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	if( (HIWORD(wParam) == EN_SETFOCUS) ||
		(HIWORD(wParam) == BN_CLICKED)  ||
		(HIWORD(wParam) == CBN_DROPDOWN)  )
	{
		_applyButton.EnableWindow(TRUE);
	}

	return CDialog::OnCommand(wParam, lParam);
}


void CSettingsDlg::OnNMClickPortlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	int iItem = _portList.GetNextItem(-1,LVIS_SELECTED);
		
	for (int i = 0; i < _portList.GetItemCount(); i++)
	{
		_portList.SetItemState(i,0,LVIS_DROPHILITED);
	}

	if (iItem >= 0)
	{
		BYTE portNum = (BYTE)_portList.GetItemData(iItem);
		_currentPortNum = portNum;
		DrawSettings();
		char buffer[BUFSIZ];
		sprintf(buffer,"Список портов. Текущий порт N%d",portNum);
		this->SetWindowText(buffer);
	}
	*pResult = 0;
}



void CSettingsDlg::InvertItemState(int iItem)
{
	CString portStr;
	BYTE portNum = (BYTE)_portList.GetItemData(iItem);
	CSettings *settings = GetSettings(portNum);
	if (settings)
	{
		if ("Закрыт" == _portList.GetItemText(iItem,1))
		{
			_timer = this->SetTimer(1,10000,0);
			_portList.SetItemText(iItem,1,"");
			portStr = _portList.GetItemText(iItem,0);
			_portList.SetItem(iItem,0,LVIF_TEXT | LVIF_IMAGE,portStr,0,0,0,0);
			_portList.SetItemText(iItem,2,"0");
			_portList.SetItemText(iItem,3,"");
			
			settings[_currentPortNum]._bForceClosed = FALSE;

		} else 	if ("Используется" == _portList.GetItemText(iItem,1) || "" == _portList.GetItemText(iItem,1))
		{
			KillTimer(_timer);
			_portList.SetItemText(iItem,1,"Закрыт");
			portStr = _portList.GetItemText(iItem,0);
			_portList.SetItem(iItem,0,LVIF_TEXT | LVIF_IMAGE,portStr,1,0,0,0);
			_portList.SetItemText(iItem,2,"0");
			_portList.SetItemText(iItem,3,"");

			settings[_currentPortNum]._bForceClosed = TRUE;
		}

	}else
	{
		AfxMessageBox("Настройки для этого порта не найдены. При повторении ошибки перезапустите программу.");
	}

}



void CSettingsDlg::OnNMDblclkPortlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	
	INT iItem = _portList.GetNextItem(-1,LVIS_SELECTED);
	if (iItem >= 0)
	{
		_currentPortNum = (BYTE)_portList.GetItemData(iItem);
		InvertItemState(iItem);
		_applyButton.EnableWindow(TRUE);

	}
	
	*pResult = 0;
}



void CSettingsDlg::OnTimer(UINT nIDEvent)
{
	int selItem = _portList.GetNextItem(-1,LVIS_DROPHILITED);

	if (selItem < 0)
	{
		selItem = _portList.GetNextItem(-1,LVIS_SELECTED);
	}

	_portList.DeleteAllItems();
	FillPortList();
	
	for (int i = 0; i < _portList.GetItemCount(); i++)
	{
		_portList.SetItemState(i,0,LVIS_DROPHILITED);
	}

	if (selItem <= _portList.GetItemCount() && selItem >= 0)
	{
		_portList.SetItemState(selItem,LVIS_DROPHILITED,LVIS_DROPHILITED);
	}
	
	CDialog::OnTimer(nIDEvent);
}

void CSettingsDlg::OnClose()
{
	_timer ? KillTimer(_timer) : FALSE;

	CDialog::OnClose();
}

void CSettingsDlg::SetHotPort(const BYTE portNum)
{
	int iItem = -1;
	for(int i = 0; i < _portList.GetItemCount();i++)
	{
		if (portNum == _portList.GetItemData(i))
		{
			iItem = i;
		}
	}
	
	if (iItem >= 0)
	{		
		_portList.SetItemState(iItem,LVIS_DROPHILITED,LVIS_DROPHILITED);
		_currentPortNum = portNum;
		
	}
}


