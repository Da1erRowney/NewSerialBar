#pragma once
#include "afxwin.h"



// CAboutDlg dialog

class CAboutDlg : public CDialog
{
	
	DECLARE_DYNAMIC(CAboutDlg)


public:
	CAboutDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:

	virtual BOOL OnInitDialog();
	void CenterDialog();
	void LoadAboutMessage(CListBox* list);
	void CreateLogoPicture();
	void LoadSite(void);	
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

private:
	
	CBitmapButton* _logo;
	CListBox _msg;
	HCURSOR _handCursor;
	CFont _urlFont;



protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
};
