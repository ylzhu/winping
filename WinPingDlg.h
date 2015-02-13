
// WinPingDlg.h : header file
//

#pragma once
#include "afxwin.h"

#include "AutoCompleteComboBox.h"

static UINT g_stPingStatus;
// CWinPingDlg dialog
class CWinPingDlg : public CDialog
{
// Construction
public:
	CWinPingDlg(CWnd* pParent = NULL);	// standard constructor
// Dialog Data
	enum { IDD = IDD_WINPING_DIALOG };
    
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

private:
	static UINT __cdecl PingThreadProc(LPVOID lParam);
	static CString m_strResult;
	void AddStringToComboBox();

    //CNEWBMP m_TestStatusbmp;
// Implementation
protected:
	HICON m_hIcon;
	CAutoCompleteComboBox m_combo;
	CEdit m_edit;
    
	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedOk();
	afx_msg LRESULT OnPingMsg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPingFin(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
    
};
