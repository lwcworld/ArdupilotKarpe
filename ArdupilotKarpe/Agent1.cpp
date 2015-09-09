// Agent1.cpp : implementation file
//

#include "stdafx.h"
#include "ArdupilotKarpe.h"
#include "Agent1.h"
#include "afxdialogex.h"


// Agent1 dialog

IMPLEMENT_DYNAMIC(Agent1, CDialog)

Agent1::Agent1(CWnd* pParent /*=NULL*/)
	: CDialog(Agent1::IDD, pParent)
{

}

Agent1::~Agent1()
{
}

void Agent1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Agent1, CDialog)
END_MESSAGE_MAP()


// Agent1 message handlers
