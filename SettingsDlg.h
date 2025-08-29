#pragma once
#import "../SerialServer/_SerialServer.tlb" raw_interfaces_only, no_namespace, named_guids
#include "Settings.h"
#include "afxwin.h"
#include "afxcmn.h"



class CSettingsDlg : public CDialog
{
	DECLARE_DYNAMIC(CSettingsDlg)
	
public:
	typedef void (CSettingsDlg::*APPLY_ENDHANDLER)(long result);

	struct ApplyThreadParams
	{
		CSettingsDlg *dlg;
		CSettings *settings;
		APPLY_ENDHANDLER *callback;
	} params;

	CSettingsDlg(CWnd* pParent = NULL);   // standard constructor
	CSettingsDlg(IServer *iSrv,IManager *_iManager,CSettings* settings,BYTE portNum = 0);
	virtual ~CSettingsDlg();
//////////////////////////////////////////////////////////////////////////
// Dialog Data

	enum { IDD = IDD_SETTINGS };
private:
	
	//IPort *_iPort;
	IServer *_iSrv;
	IManager *_iManager;
	CSettings *_settings;
	BYTE _currentPortNum;
	SAFEARRAY *_existPorts;
	BYTE _ports[MAX_PORT];
	USHORT _portCnt;
	UINT_PTR  _timer;

	int _baudeRate;
	UINT _timeout;
	UINT _waitByte;
	UINT _waitRequest;
	int _stopBits;
	int _byteSize;
	int _parity;
	DWORD _requestOn;
	DWORD _requestOff;
	CButton _applyButton;
	CButton _okButton;
	CButton _cancelButton;
	CImageList _portImages;
	CListCtrl _portList;	
	BYTE _selectedPort;

	
	
//////////////////////////////////////////////////////////////////////////
//Methods

protected:
	DECLARE_MESSAGE_MAP()

	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	afx_msg void OnCbnSelchangeBaude();
	afx_msg void OnEnChangeWaitbyte();
	afx_msg void OnEnChangeWaitrequest();
	afx_msg void OnEnChangeTimeout();
	afx_msg void OnBnClickedParity();
	afx_msg void OnBnClickedBsize();
	afx_msg void OnBnClickedStopbits();
	afx_msg void OnEnChangeRequeston();
	afx_msg void OnEnChangeRequestoff();
	afx_msg void OnBnClickedApply();
	
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
private:
	void CenterDialog(void);
	void ApplySettings(void);
	void DrawSettings();
	int ToControlByteSize(BYTE byteSize);
	int ToControlBaude(DWORD baudeRate);
	BYTE ToSettingsByteSize(int byteSize);
	DWORD ToSettingsBaude(int baudeRate);
	BOOL InitImageList(void);
	BOOL InitPortList(void);
	void FillPortList(void);
	CSettings* GetSettings(BYTE portNum);
	void AddPortItem(BYTE portNum,BOOL bForceClosed, int clientCnt,char* clientName[32],int namesCnt);
	int GetImageIndex(BOOL bForceClosed, int clientCnt);
	void GetStatusString(BOOL bForceClosed, int clientCnt,char* inStatus);
	void InvertItemState(int iItem);
	ULONG GetClientName(BYTE portNum,char* inClientName[32]);
	static UINT ApplySettingsThread(void *param);
	void SetTitle();
	
public:
	afx_msg void OnNMClickPortlist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkPortlist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClose();
private:
	void SetHotPort(const BYTE portNum);
	void OnApplyEnd(long result);
	bool TurnOnApply();
};
