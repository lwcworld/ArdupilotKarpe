#pragma once


// Agent1Dlg dialog

class Agent1Dlg : public CDialogEx
{
	DECLARE_DYNAMIC(Agent1Dlg)

public:
	Agent1Dlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~Agent1Dlg();

// Dialog Data
	enum { IDD = IDD_AGENT1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
