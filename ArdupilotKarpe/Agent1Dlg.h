#pragma once


// CAgent1Dlg dialog

class CAgent1Dlg : public CDialog
{
	DECLARE_DYNAMIC(CAgent1Dlg)

public:
	CAgent1Dlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAgent1Dlg();

// Dialog Data
	enum { IDD = IDD_AGENT1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
