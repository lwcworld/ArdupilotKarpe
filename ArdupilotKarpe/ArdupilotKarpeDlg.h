
// ArdupilotKarpeDlg.h : header file
//

#pragma once
#include "Agent1Dlg.h"
#include "afxwin.h"
#include "Mycomm.h"

#define NQ (3)
#define time_gap 0.005
#define D2R ((3.14159)/(180))

// CArdupilotKarpeDlg dialog
class CArdupilotKarpeDlg : public CDialogEx
{

// Construction
public:
	CMycomm*     m_comm[NQ];
	LRESULT      OnThreadClosed(WPARAM length, LPARAM lpara);
	LRESULT      OnReceive(WPARAM length, LPARAM lpara);
	void CtrlSend(int QuadNum, double CTRL_roll, double CTRL_pitch, double CTRL_yaw, double CTRL_thrust);
	double xgoal[NQ];
	double ygoal[NQ];
	double zgoal[NQ];

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

	BOOL comport_state[NQ];
	CComboBox m_combo_comport_list1;
	CComboBox m_combo_baudrate_list1;
	CString m_str_comport1;
	CString m_str_baudrate1;
	CComboBox m_combo_comport_list2;
	CComboBox m_combo_baudrate_list2;
	CString m_str_comport2;
	CString m_str_baudrate2;
	CComboBox m_combo_comport_list3;
	CComboBox m_combo_baudrate_list3;
	CString m_str_comport3;
	CString m_str_baudrate3;
	CEdit m_edit_rcv_view1;
	CEdit m_edit_rcv_view2;
	CEdit m_edit_rcv_view3;
	afx_msg void OnBnClickedBtConnect1();
	afx_msg void OnBnClickedBtConnect2();
	afx_msg void OnBnClickedBtConnect3();
	afx_msg void OnCbnSelchangeComboComport1();
	afx_msg void OnCbnSelchangeComboBaudrate1();
	afx_msg void OnCbnSelchangeComboComport2();
	afx_msg void OnCbnSelchangeComboBaudrate2();
	afx_msg void OnCbnSelchangeComboComport3();
	afx_msg void OnCbnSelchangeComboBaudrate3();
	afx_msg void OnBnClickedBtAllarm();
	afx_msg void OnBnClickedBtAlldisarm();
	afx_msg void OnBnClickedBtArm1();
	afx_msg void OnBnClickedBtDisarm1();
	afx_msg void OnBnClickedBtArm2();
	afx_msg void OnBnClickedBtDisarm2();
	afx_msg void OnBnClickedBtArm3();
	afx_msg void OnBnClickedBtDisarm3();
	afx_msg void OnBnClickedBtHovering1();
	afx_msg void OnBnClickedBtLanding1();
	afx_msg void OnBnClickedBtHovering2();
	afx_msg void OnBnClickedBtLanding2();
	afx_msg void OnBnClickedBtHovering3();
	afx_msg void OnBnClickedBtLanding3();
};