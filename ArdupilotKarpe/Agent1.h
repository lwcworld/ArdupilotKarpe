#pragma once


// Agent1 dialog

class Agent1 : public CDialog
{
	DECLARE_DYNAMIC(Agent1)

public:
	Agent1(CWnd* pParent = NULL);   // standard constructor
	virtual ~Agent1();

// Dialog Data
	enum { IDD = IDD_AGENT1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
