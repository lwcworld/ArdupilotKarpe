
// ArdupilotKarpeDlg.h : header file
//

#pragma once
#include "Agent1Dlg.h"
#include "afxwin.h"

// CArdupilotKarpeDlg dialog
class CArdupilotKarpeDlg : public CDialogEx
{

// Construction
public:
	CArdupilotKarpeDlg(CWnd* pParent = NULL);	// standard constructor

	CAgent1Dlg *m_Agent1;

// Dialog Data
	enum { IDD = IDD_ARDUPILOTKARPE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtKarpeconnect();
	afx_msg void OnBnClickedBtKarpedisconnect();
	afx_msg void OnBnClickedBtAgent1();
	afx_msg void OnTimer(UINT nIDEvent);
	CComboBox m_combo_comport_list1;
	CComboBox m_combo_baudrate_list;
	CString m_str_comport1;
	CString m_str_baudrate1;
	CComboBox m_combo_comport_list2;
	CComboBox m_combo_baudrate_list2;
	CString m_str_comport2;
	CString m_str_baudrate2;
	CComboBox m_combo_comport_list3;
	CComboBox m_combo_baudrate3;
	CString m_str_comport3;
	CString m_str_baudrate3;
};