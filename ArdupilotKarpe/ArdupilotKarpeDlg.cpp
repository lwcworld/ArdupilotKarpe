
// ArdupilotKarpeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ArdupilotKarpe.h"
#include "ArdupilotKarpeDlg.h"
#include "afxdialogex.h"
#include "Sdk2Example2.h"

// 우철 include입니다.

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 우철 함수입니다. 
void Wait(DWORD dwMillisecond)
{
	MSG msg;
	DWORD dwStart;

	dwStart = GetTickCount();

	while (GetTickCount() - dwStart < dwMillisecond)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}

void CArdupilotKarpeDlg::OnTimer(UINT nIDEvent)
{
	switch (nIDEvent)
	{
	case 1: // 타이머로 제어 입력
		break;
	}
}

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CArdupilotKarpeDlg dialog



CArdupilotKarpeDlg::CArdupilotKarpeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CArdupilotKarpeDlg::IDD, pParent)
	, m_str_comport1(_T(""))
	, m_str_baudrate1(_T(""))
	, m_str_comport2(_T(""))
	, m_str_baudrate2(_T(""))
	, m_str_comport3(_T(""))
	, m_str_baudrate3(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CArdupilotKarpeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_COMPORT1, m_combo_comport_list1);
	DDX_Control(pDX, IDC_COMBO_BAUDRATE1, m_combo_baudrate_list);
	DDX_CBString(pDX, IDC_COMBO_COMPORT1, m_str_comport1);
	DDX_CBString(pDX, IDC_COMBO_BAUDRATE1, m_str_baudrate1);
	DDX_Control(pDX, IDC_COMBO_COMPORT2, m_combo_comport_list2);
	DDX_Control(pDX, IDC_COMBO_BAUDRATE2, m_combo_baudrate_list2);
	DDX_CBString(pDX, IDC_COMBO_COMPORT2, m_str_comport2);
	DDX_CBString(pDX, IDC_COMBO_BAUDRATE2, m_str_baudrate2);
	DDX_Control(pDX, IDC_COMBO_COMPORT3, m_combo_comport_list3);
	DDX_Control(pDX, IDC_COMBO_BAUDRATE3, m_combo_baudrate3);
	DDX_CBString(pDX, IDC_COMBO_COMPORT3, m_str_comport3);
	DDX_CBString(pDX, IDC_COMBO_BAUDRATE3, m_str_baudrate3);
}

BEGIN_MESSAGE_MAP(CArdupilotKarpeDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BT_KARPECONNECT, &CArdupilotKarpeDlg::OnBnClickedBtKarpeconnect)
	ON_BN_CLICKED(IDC_BT_KARPEDISCONNECT, &CArdupilotKarpeDlg::OnBnClickedBtKarpedisconnect)
	ON_BN_CLICKED(IDC_BT_AGENT1, &CArdupilotKarpeDlg::OnBnClickedBtAgent1)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CArdupilotKarpeDlg message handlers

BOOL CArdupilotKarpeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	SetTimer(1, 20, NULL);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CArdupilotKarpeDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CArdupilotKarpeDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CArdupilotKarpeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CArdupilotKarpeDlg::OnBnClickedBtKarpeconnect()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	//Console::Allocate(); // 콘솔창 띄우는 사제 코드임. Console.cpp / console.h 필요함. 
	// 일단은 stdafx.cpp 에 #pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console") 추가해서 쓰겠음. 
	// 뭐가 나은지는 잘 모르겠음.

	MySdk2Client sdkClient;

	printf("*** SDK2 Sample Program modified by J1\n");
	printf("*** Program Version: %s\n", APP_VERSION);
	printf("*** SDK2 Version   : %s\n\n", EvartSdk2Interface::VersionString().c_str());

	char str[3][15] = { "10.1.1.199", "10.1.1.199", "200000" }; //IP address, time(ms)
	printf("%s\n%s\n%s\n\n", str[0], str[1], str[2]);

	// Try to connect to the given EVaRT SDK2 server
	//if (sdkClient.Connect(argv[1], argv[2], atoi(argv[3])))
	if (sdkClient.Connect(str[0], str[1], atoi(str[2])))
	{
		//printf("Streaming data from EVaRT SDK2 server %s...\n\n", argv[1]);
		printf("Streaming data from EVaRT SDK2 server %s...\n\n", str[0]);


		// Just wait until we've received the required number of frames
		while (sdkClient.IsFinished() == false)
		{
			Wait(1000);
		}
		sdkClient.Disconnect();

		printf("\n\nFinished\n");
	}
	else
	{
		//printf("Could not connect to EVaRT SDK2 at %s\n", argv[1]);
		printf("Could not connect to EVaRT SDK2 at %s\n", str[0]);
	}
	Wait(2000);
}


void CArdupilotKarpeDlg::OnBnClickedBtKarpedisconnect()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	MySdk2Client sdkClient;

	sdkClient.Disconnect();
	printf("\n\nFinished\n");
}


void CArdupilotKarpeDlg::OnBnClickedBtAgent1()
{
	// TODO: Add your control notification handler code here
	m_Agent1 = new CAgent1Dlg;
	m_Agent1->Create(IDD_AGENT1);
	m_Agent1->ShowWindow(SW_SHOW);
}