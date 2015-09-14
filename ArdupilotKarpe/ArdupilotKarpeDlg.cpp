
// ArdupilotKarpeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ArdupilotKarpe.h"
#include "ArdupilotKarpeDlg.h"
#include "afxdialogex.h"
#include "Sdk2Example2.h"
#include "Mycomm.h"
#include "mavlink\common\mavlink.h"
#include "DataSet.h"

// 우철 include입니다.

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//우철 선언입니다.
DataSet data;

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

void CArdupilotKarpeDlg::CtrlSend(int QuadNum, double CTRL_roll, double CTRL_pitch, double CTRL_yaw, double CTRL_thrust)
{
	uint16_t rawdata[8];
	static uint8_t buffer[200];
	mavlink_message_t message;
	rawdata[0] = int(CTRL_roll); //CH1 roll
	rawdata[1] = int(CTRL_pitch); //CH2 pitch
	rawdata[2] = int(CTRL_thrust); //CH3 throttle
	rawdata[3] = int(CTRL_yaw); //CH4 yaw
	rawdata[4] = 0;
	rawdata[5] = 0;
	rawdata[6] = 0;
	rawdata[7] = 0;
	mavlink_msg_rc_channels_override_pack_chan(255, 0, 1, &message, 1, 0, rawdata[0], rawdata[1], rawdata[2], rawdata[3], rawdata[4], rawdata[5], rawdata[6], rawdata[7]);
	int len = mavlink_msg_to_send_buffer(buffer, &message);
	m_comm[QuadNum]->Send((const wchar_t*)buffer, len);
}

void CArdupilotKarpeDlg::OnTimer(UINT nIDEvent)
{
	switch (nIDEvent)
	{
	case 1: // 타이머로 제어 입력

		for (int QuadNum = 0; QuadNum < NQ; QuadNum++)
		{
			if (comport_state[QuadNum] == true) // 통신포트가 열려있는 쿼드로터에 대해서만 컨트롤 인풋 줌.
			{
				switch (data.Mode[QuadNum]) // 1: hovering / 2: landing / 3: moving / 4: failsafe
				{
				case 1:
					data.StateCalc(QuadNum);
					data.CtrlCalc(QuadNum);
					CtrlSend(QuadNum, data.CTRL_command[QuadNum][0], data.CTRL_command[QuadNum][1], data.CTRL_command[QuadNum][2], data.CTRL_command[QuadNum][3]);
					break;

				case 2:
					data.StateCalc(QuadNum);
					data.CtrlCalc(QuadNum);
					CtrlSend(QuadNum, data.CTRL_command[QuadNum][0], data.CTRL_command[QuadNum][1], data.CTRL_command[QuadNum][2], data.CTRL_command[QuadNum][3]);
					break;

				case 3:
					data.StateCalc(QuadNum);
					data.CtrlCalc(QuadNum);
					CtrlSend(QuadNum, data.CTRL_command[QuadNum][0], data.CTRL_command[QuadNum][1], data.CTRL_command[QuadNum][2], data.CTRL_command[QuadNum][3]);
					break;

				case 4:

					break;
				}


			}
		}
		break;

	case 2:
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
	DDX_Control(pDX, IDC_COMBO_BAUDRATE1, m_combo_baudrate_list1);
	DDX_CBString(pDX, IDC_COMBO_COMPORT1, m_str_comport1);
	DDX_CBString(pDX, IDC_COMBO_BAUDRATE1, m_str_baudrate1);
	DDX_Control(pDX, IDC_COMBO_COMPORT2, m_combo_comport_list2);
	DDX_Control(pDX, IDC_COMBO_BAUDRATE2, m_combo_baudrate_list2);
	DDX_CBString(pDX, IDC_COMBO_COMPORT2, m_str_comport2);
	DDX_CBString(pDX, IDC_COMBO_BAUDRATE2, m_str_baudrate2);
	DDX_Control(pDX, IDC_COMBO_COMPORT3, m_combo_comport_list3);
	DDX_Control(pDX, IDC_COMBO_BAUDRATE3, m_combo_baudrate_list3);
	DDX_CBString(pDX, IDC_COMBO_COMPORT3, m_str_comport3);
	DDX_CBString(pDX, IDC_COMBO_BAUDRATE3, m_str_baudrate3);
	DDX_Control(pDX, IDC_EDIT_RCV_VIEW1, m_edit_rcv_view1);
	DDX_Control(pDX, IDC_EDIT_RCV_VIEW2, m_edit_rcv_view2);
	DDX_Control(pDX, IDC_EDIT_REV_VIEW3, m_edit_rcv_view3);
}

BEGIN_MESSAGE_MAP(CArdupilotKarpeDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()

	ON_MESSAGE(WM_MYCLOSE, &CArdupilotKarpeDlg::OnThreadClosed)
	ON_MESSAGE(WM_MYRECEIVE, &CArdupilotKarpeDlg::OnReceive)

	ON_BN_CLICKED(IDC_BT_KARPECONNECT, &CArdupilotKarpeDlg::OnBnClickedBtKarpeconnect)
	ON_BN_CLICKED(IDC_BT_KARPEDISCONNECT, &CArdupilotKarpeDlg::OnBnClickedBtKarpedisconnect)
	ON_BN_CLICKED(IDC_BT_AGENT1, &CArdupilotKarpeDlg::OnBnClickedBtAgent1)
	ON_WM_TIMER()

	ON_BN_CLICKED(IDC_BT_CONNECT1, &CArdupilotKarpeDlg::OnBnClickedBtConnect1)
	ON_BN_CLICKED(IDC_BT_CONNECT2, &CArdupilotKarpeDlg::OnBnClickedBtConnect2)
	ON_BN_CLICKED(IDC_BT_CONNECT3, &CArdupilotKarpeDlg::OnBnClickedBtConnect3)
	ON_CBN_SELCHANGE(IDC_COMBO_COMPORT1, &CArdupilotKarpeDlg::OnCbnSelchangeComboComport1)
	ON_CBN_SELCHANGE(IDC_COMBO_BAUDRATE1, &CArdupilotKarpeDlg::OnCbnSelchangeComboBaudrate1)
	ON_CBN_SELCHANGE(IDC_COMBO_COMPORT2, &CArdupilotKarpeDlg::OnCbnSelchangeComboComport2)
	ON_CBN_SELCHANGE(IDC_COMBO_BAUDRATE2, &CArdupilotKarpeDlg::OnCbnSelchangeComboBaudrate2)
	ON_CBN_SELCHANGE(IDC_COMBO_COMPORT3, &CArdupilotKarpeDlg::OnCbnSelchangeComboComport3)
	ON_CBN_SELCHANGE(IDC_COMBO_BAUDRATE3, &CArdupilotKarpeDlg::OnCbnSelchangeComboBaudrate3)
	ON_BN_CLICKED(IDC_BT_ALLARM, &CArdupilotKarpeDlg::OnBnClickedBtAllarm)
	ON_BN_CLICKED(IDC_BT_ALLDISARM, &CArdupilotKarpeDlg::OnBnClickedBtAlldisarm)
	ON_BN_CLICKED(IDC_BT_ARM1, &CArdupilotKarpeDlg::OnBnClickedBtArm1)
	ON_BN_CLICKED(IDC_BT_DISARM1, &CArdupilotKarpeDlg::OnBnClickedBtDisarm1)
	ON_BN_CLICKED(IDC_BT_ARM2, &CArdupilotKarpeDlg::OnBnClickedBtArm2)
	ON_BN_CLICKED(IDC_BT_DISARM2, &CArdupilotKarpeDlg::OnBnClickedBtDisarm2)
	ON_BN_CLICKED(IDC_BT_ARM3, &CArdupilotKarpeDlg::OnBnClickedBtArm3)
	ON_BN_CLICKED(IDC_BT_DISARM3, &CArdupilotKarpeDlg::OnBnClickedBtDisarm3)
	ON_BN_CLICKED(IDC_BT_HOVERING1, &CArdupilotKarpeDlg::OnBnClickedBtHovering1)
	ON_BN_CLICKED(IDC_BT_LANDING1, &CArdupilotKarpeDlg::OnBnClickedBtLanding1)
	ON_BN_CLICKED(IDC_BT_HOVERING2, &CArdupilotKarpeDlg::OnBnClickedBtHovering2)
	ON_BN_CLICKED(IDC_BT_LANDING2, &CArdupilotKarpeDlg::OnBnClickedBtLanding2)
	ON_BN_CLICKED(IDC_BT_HOVERING3, &CArdupilotKarpeDlg::OnBnClickedBtHovering3)
	ON_BN_CLICKED(IDC_BT_LANDING3, &CArdupilotKarpeDlg::OnBnClickedBtLanding3)
	ON_BN_CLICKED(IDC_BT_NEXTPOINT1, &CArdupilotKarpeDlg::OnBnClickedBtNextpoint1)
	ON_BN_CLICKED(IDC_BT_UP1, &CArdupilotKarpeDlg::OnBnClickedBtUp1)
	ON_BN_CLICKED(IDC_BT_DOWN1, &CArdupilotKarpeDlg::OnBnClickedBtDown1)
END_MESSAGE_MAP()

LRESULT CArdupilotKarpeDlg::OnThreadClosed(WPARAM length, LPARAM lpara)
{
	//overlapped i/o 핸들을닫는다.
	((CMycomm*)lpara)->HandleClose();
	delete ((CMycomm*)lpara);

	return 0;
}

LRESULT CArdupilotKarpeDlg::OnReceive(WPARAM length, LPARAM lpara)
{
	CString str;
	char data1[20000];
	char data2[20000];
	char data3[20000];

	if (m_comm[0])
	{
		m_comm[0]->Receive(data1, length); //length 길이만큼데이터를받는다.
		data1[length] = _T('\0');
		str += _T("\r\n");
		for (int i = 0; i<length; i++)
		{
			str += data1[i];
		}
		m_edit_rcv_view1.ReplaceSel(str); //에디트박스에표시하기위함
		str = "";
		//UpdateData(FALSE);
		m_edit_rcv_view1.LineScroll(m_edit_rcv_view1.GetLineCount());
	}

	if (m_comm[1])
	{
		m_comm[1]->Receive(data2, length); //length 길이만큼데이터를받는다.
		data2[length] = _T('\0');
		str += _T("\r\n");
		for (int i = 0; i<length; i++)
		{
			str += data2[i];
		}
		m_edit_rcv_view2.ReplaceSel(str); //에디트박스에표시하기위함
		str = "";
		//UpdateData(FALSE);
		m_edit_rcv_view2.LineScroll(m_edit_rcv_view2.GetLineCount());
	}

	if (m_comm[2])
	{
		m_comm[2]->Receive(data3, length); //length 길이만큼데이터를받는다.
		data3[length] = _T('\0');
		str += _T("\r\n");
		for (int i = 0; i<length; i++)
		{
			str += data3[i];
		}
		m_edit_rcv_view3.ReplaceSel(str); //에디트박스에표시하기위함
		str = "";
		//UpdateData(FALSE);
		m_edit_rcv_view3.LineScroll(m_edit_rcv_view3.GetLineCount());
	}

	return 0;
}


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
	m_combo_comport_list1.AddString(_T("COM1"));
	m_combo_comport_list1.AddString(_T("COM2"));
	m_combo_comport_list1.AddString(_T("COM3"));
	m_combo_comport_list1.AddString(_T("COM4"));
	m_combo_comport_list1.AddString(_T("COM5"));
	m_combo_comport_list1.AddString(_T("COM6"));
	m_combo_comport_list1.AddString(_T("COM7"));
	m_combo_comport_list1.AddString(_T("COM8"));
	m_combo_comport_list1.AddString(_T("COM9"));
	m_combo_comport_list1.AddString(_T("COM10"));
	m_combo_comport_list1.AddString(_T("COM11"));
	m_combo_comport_list1.AddString(_T("COM12"));
	m_combo_comport_list1.AddString(_T("COM13"));
	m_combo_comport_list1.AddString(_T("COM14"));
	m_combo_comport_list1.AddString(_T("COM15"));

	m_combo_comport_list2.AddString(_T("COM1"));
	m_combo_comport_list2.AddString(_T("COM2"));
	m_combo_comport_list2.AddString(_T("COM3"));
	m_combo_comport_list2.AddString(_T("COM4"));
	m_combo_comport_list2.AddString(_T("COM5"));
	m_combo_comport_list2.AddString(_T("COM6"));
	m_combo_comport_list2.AddString(_T("COM7"));
	m_combo_comport_list2.AddString(_T("COM8"));
	m_combo_comport_list2.AddString(_T("COM9"));
	m_combo_comport_list2.AddString(_T("COM10"));
	m_combo_comport_list2.AddString(_T("COM11"));
	m_combo_comport_list2.AddString(_T("COM12"));
	m_combo_comport_list2.AddString(_T("COM13"));
	m_combo_comport_list2.AddString(_T("COM14"));
	m_combo_comport_list2.AddString(_T("COM15"));

	m_combo_comport_list3.AddString(_T("COM1"));
	m_combo_comport_list3.AddString(_T("COM2"));
	m_combo_comport_list3.AddString(_T("COM3"));
	m_combo_comport_list3.AddString(_T("COM4"));
	m_combo_comport_list3.AddString(_T("COM5"));
	m_combo_comport_list3.AddString(_T("COM6"));
	m_combo_comport_list3.AddString(_T("COM7"));
	m_combo_comport_list3.AddString(_T("COM8"));
	m_combo_comport_list3.AddString(_T("COM9"));
	m_combo_comport_list3.AddString(_T("COM10"));
	m_combo_comport_list3.AddString(_T("COM11"));
	m_combo_comport_list3.AddString(_T("COM12"));
	m_combo_comport_list3.AddString(_T("COM13"));
	m_combo_comport_list3.AddString(_T("COM14"));
	m_combo_comport_list3.AddString(_T("COM15"));

	m_combo_baudrate_list1.AddString(_T("57600"));
	m_combo_baudrate_list2.AddString(_T("57600"));
	m_combo_baudrate_list3.AddString(_T("57600"));  // 잘못 만듬... ;;; ㅠㅠㅠ

	for (int i = 0; i < NQ; i++)
	{
		comport_state[i] = false;
	}

	GetDlgItem(IDC_BT_CONNECT1)->SetWindowText(_T("OPEN1"));
	GetDlgItem(IDC_BT_CONNECT2)->SetWindowText(_T("OPEN2"));
	GetDlgItem(IDC_BT_CONNECT3)->SetWindowText(_T("OPEN3"));

	m_str_comport1 = _T("COM1");
	m_str_baudrate1 = _T("57600");

	m_str_comport2 = _T("COM2");
	m_str_baudrate2 = _T("57600");

	m_str_comport3 = _T("COM3");
	m_str_baudrate3 = _T("57600");

	UpdateData(FALSE);

	SetTimer(1, 20, NULL);
	SetTimer(2, 100, NULL); // gnuplot

	
	
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

void CArdupilotKarpeDlg::OnBnClickedBtConnect1()
{
	// TODO: Add your control notification handler code here
	if (comport_state[0])
	{
		if (m_comm[0])        //컴포트가존재하면
		{
			m_comm[0]->Close();
			m_comm[0] = NULL;
			AfxMessageBox(_T("COM 포트닫힘"));
			comport_state[0] = false;
			GetDlgItem(IDC_BT_CONNECT1)->SetWindowText(_T("OPEN"));
			//GetDlgItem(IDC_BT_SEND1)->EnableWindow(false);
		}
	}
	else
	{
		m_comm[0] = new CMycomm(_T("\\\\.\\") + m_str_comport1, m_str_baudrate1, _T("None"), _T("8 Bit"), _T("1 Bit"));         // initial Comm port
		if (m_comm[0]->Create(GetSafeHwnd()) != 0) //통신포트를열고윈도우의핸들을넘긴다.
		{
			AfxMessageBox(_T("COM 포트열림"));
			comport_state[0] = true;
			GetDlgItem(IDC_BT_CONNECT1)->SetWindowText(_T("CLOSE"));
			//GetDlgItem(IDC_BT_SEND)->EnableWindow(true);
		}
		else
		{
			AfxMessageBox(_T("ERROR!"));
		}

	}
}


void CArdupilotKarpeDlg::OnBnClickedBtConnect2()
{
	// TODO: Add your control notification handler code here
	if (comport_state[1])
	{
		if (m_comm[1])        //컴포트가존재하면
		{
			m_comm[1]->Close();
			m_comm[1] = NULL;
			AfxMessageBox(_T("COM 포트닫힘"));
			comport_state[1] = false;
			GetDlgItem(IDC_BT_CONNECT2)->SetWindowText(_T("OPEN"));
			//GetDlgItem(IDC_BT_SEND)->EnableWindow(false);
		}
	}
	else
	{
		m_comm[1] = new CMycomm(_T("\\\\.\\") + m_str_comport2, m_str_baudrate2, _T("None"), _T("8 Bit"), _T("1 Bit"));         // initial Comm port
		if (m_comm[1]->Create(GetSafeHwnd()) != 0) //통신포트를열고윈도우의핸들을넘긴다.
		{
			AfxMessageBox(_T("COM 포트열림"));
			comport_state[1] = true;
			GetDlgItem(IDC_BT_CONNECT2)->SetWindowText(_T("CLOSE"));
			//GetDlgItem(IDC_BT_SEND)->EnableWindow(true);
		}
		else
		{
			AfxMessageBox(_T("ERROR!"));
		}

	}
}


void CArdupilotKarpeDlg::OnBnClickedBtConnect3()
{
	// TODO: Add your control notification handler code here
	if (comport_state[2])
	{
		if (m_comm[2])        //컴포트가존재하면
		{
			m_comm[2]->Close();
			m_comm[2] = NULL;
			AfxMessageBox(_T("COM 포트닫힘"));
			comport_state[2] = false;
			GetDlgItem(IDC_BT_CONNECT3)->SetWindowText(_T("OPEN"));
			//GetDlgItem(IDC_BT_SEND)->EnableWindow(false);
		}
	}
	else
	{
		m_comm[2] = new CMycomm(_T("\\\\.\\") + m_str_comport3, m_str_baudrate3, _T("None"), _T("8 Bit"), _T("1 Bit"));         // initial Comm port
		if (m_comm[2]->Create(GetSafeHwnd()) != 0) //통신포트를열고윈도우의핸들을넘긴다.
		{
			AfxMessageBox(_T("COM 포트열림"));
			comport_state[2] = true;
			GetDlgItem(IDC_BT_CONNECT3)->SetWindowText(_T("CLOSE"));
			//GetDlgItem(IDC_BT_SEND)->EnableWindow(true);
		}
		else
		{
			AfxMessageBox(_T("ERROR!"));
		}

	}
}


void CArdupilotKarpeDlg::OnCbnSelchangeComboComport1()
{
	// TODO: Add your control notification handler code here
	UpdateData();
}


void CArdupilotKarpeDlg::OnCbnSelchangeComboBaudrate1()
{
	// TODO: Add your control notification handler code here
	UpdateData();
}


void CArdupilotKarpeDlg::OnCbnSelchangeComboComport2()
{
	// TODO: Add your control notification handler code here
	UpdateData();
}


void CArdupilotKarpeDlg::OnCbnSelchangeComboBaudrate2()
{
	// TODO: Add your control notification handler code here
	UpdateData();
}


void CArdupilotKarpeDlg::OnCbnSelchangeComboComport3()
{
	// TODO: Add your control notification handler code here
	UpdateData();
}


void CArdupilotKarpeDlg::OnCbnSelchangeComboBaudrate3()
{
	// TODO: Add your control notification handler code here
	UpdateData();
}


void CArdupilotKarpeDlg::OnBnClickedBtAllarm()
{
	// TODO: Add your control notification handler code here
	//------------------------------[arm]----------------------------------------------------
	// Create buffer
	static uint8_t buffer[150];
	mavlink_message_t message;
	//mavlink_msg_rc_channels_override_pack_chan(255, 0, 1, &message, 1, 0, rawdata[0], rawdata[1], rawdata[2], rawdata[3], rawdata[4], rawdata[5], rawdata[6], rawdata[7]);

	mavlink_msg_command_long_pack(255, 0, &message, 1, 250, MAV_CMD_COMPONENT_ARM_DISARM, 0, 1, 0, 0, 0, 0, 0, 0);
	//mavlink_msg_command_long_pack(system_id, component id, msg, target sys, target component, command, confirmation, par1, par2, par3, par4, par5, par6, par7);

	int len = mavlink_msg_to_send_buffer(buffer, &message);

	for (int i = 0; i < NQ; i++)
	{
		if (comport_state[i] == true) m_comm[i]->Send((const wchar_t*)buffer, len);
	}
	//---------------------------------------------------------------------------------------
}

void CArdupilotKarpeDlg::OnBnClickedBtAlldisarm()
{
	// TODO: Add your control notification handler code here
	//---------------------------[disarm]----------------------------------------------------
	// Create buffer
	static uint8_t buffer[200];
	mavlink_message_t message;

	mavlink_msg_command_long_pack(255, 0, &message, 1, 250, MAV_CMD_COMPONENT_ARM_DISARM, 0, 0, 0, 0, 0, 0, 0, 0);
	//mavlink_msg_command_long_pack(system_id, component id, msg, target sys, target component, command, confirmation, par1, par2, par3, par4, par5, par6, par7);
	int len = mavlink_msg_to_send_buffer(buffer, &message);

	for (int i = 0; i < NQ; i++)
	{
		if (comport_state[i] == true) m_comm[i]->Send((const wchar_t*)buffer, len);
	}
	//---------------------------------------------------------------------------------------
}

void CArdupilotKarpeDlg::OnBnClickedBtArm1()
{
	// TODO: Add your control notification handler code here
	//------------------------------[arm]----------------------------------------------------
	// Create buffer
	static uint8_t buffer[150];
	mavlink_message_t message;
	//mavlink_msg_rc_channels_override_pack_chan(255, 0, 1, &message, 1, 0, rawdata[0], rawdata[1], rawdata[2], rawdata[3], rawdata[4], rawdata[5], rawdata[6], rawdata[7]);

	mavlink_msg_command_long_pack(255, 0, &message, 1, 250, MAV_CMD_COMPONENT_ARM_DISARM, 0, 1, 0, 0, 0, 0, 0, 0);
	//mavlink_msg_command_long_pack(system_id, component id, msg, target sys, target component, command, confirmation, par1, par2, par3, par4, par5, par6, par7);

	int len = mavlink_msg_to_send_buffer(buffer, &message);

	m_comm[0]->Send((const wchar_t*)buffer, len);
	//---------------------------------------------------------------------------------------
}


void CArdupilotKarpeDlg::OnBnClickedBtDisarm1()
{
	// TODO: Add your control notification handler code here
	//---------------------------[disarm]----------------------------------------------------
	// Create buffer
	static uint8_t buffer[200];
	mavlink_message_t message;

	mavlink_msg_command_long_pack(255, 0, &message, 1, 250, MAV_CMD_COMPONENT_ARM_DISARM, 0, 0, 0, 0, 0, 0, 0, 0);
	//mavlink_msg_command_long_pack(system_id, component id, msg, target sys, target component, command, confirmation, par1, par2, par3, par4, par5, par6, par7);
	int len = mavlink_msg_to_send_buffer(buffer, &message);

	m_comm[0]->Send((const wchar_t*)buffer, len);
	//---------------------------------------------------------------------------------------
}


void CArdupilotKarpeDlg::OnBnClickedBtArm2()
{
	// TODO: Add your control notification handler code here
	//------------------------------[arm]----------------------------------------------------
	// Create buffer
	static uint8_t buffer[150];
	mavlink_message_t message;
	//mavlink_msg_rc_channels_override_pack_chan(255, 0, 1, &message, 1, 0, rawdata[0], rawdata[1], rawdata[2], rawdata[3], rawdata[4], rawdata[5], rawdata[6], rawdata[7]);

	mavlink_msg_command_long_pack(255, 0, &message, 1, 250, MAV_CMD_COMPONENT_ARM_DISARM, 0, 1, 0, 0, 0, 0, 0, 0);
	//mavlink_msg_command_long_pack(system_id, component id, msg, target sys, target component, command, confirmation, par1, par2, par3, par4, par5, par6, par7);

	int len = mavlink_msg_to_send_buffer(buffer, &message);

	m_comm[1]->Send((const wchar_t*)buffer, len);
	//---------------------------------------------------------------------------------------
}


void CArdupilotKarpeDlg::OnBnClickedBtDisarm2()
{
	// TODO: Add your control notification handler code here
	//---------------------------[disarm]----------------------------------------------------
	// Create buffer
	static uint8_t buffer[200];
	mavlink_message_t message;

	mavlink_msg_command_long_pack(255, 0, &message, 1, 250, MAV_CMD_COMPONENT_ARM_DISARM, 0, 0, 0, 0, 0, 0, 0, 0);
	//mavlink_msg_command_long_pack(system_id, component id, msg, target sys, target component, command, confirmation, par1, par2, par3, par4, par5, par6, par7);
	int len = mavlink_msg_to_send_buffer(buffer, &message);

	m_comm[1]->Send((const wchar_t*)buffer, len);
	//---------------------------------------------------------------------------------------
}


void CArdupilotKarpeDlg::OnBnClickedBtArm3()
{
	// TODO: Add your control notification handler code here
	//------------------------------[arm]----------------------------------------------------
	// Create buffer
	static uint8_t buffer[150];
	mavlink_message_t message;
	//mavlink_msg_rc_channels_override_pack_chan(255, 0, 1, &message, 1, 0, rawdata[0], rawdata[1], rawdata[2], rawdata[3], rawdata[4], rawdata[5], rawdata[6], rawdata[7]);

	mavlink_msg_command_long_pack(255, 0, &message, 1, 250, MAV_CMD_COMPONENT_ARM_DISARM, 0, 1, 0, 0, 0, 0, 0, 0);
	//mavlink_msg_command_long_pack(system_id, component id, msg, target sys, target component, command, confirmation, par1, par2, par3, par4, par5, par6, par7);

	int len = mavlink_msg_to_send_buffer(buffer, &message);

	m_comm[2]->Send((const wchar_t*)buffer, len);
	//---------------------------------------------------------------------------------------
}


void CArdupilotKarpeDlg::OnBnClickedBtDisarm3()
{
	// TODO: Add your control notification handler code here
	//---------------------------[disarm]----------------------------------------------------
	// Create buffer
	static uint8_t buffer[200];
	mavlink_message_t message;

	mavlink_msg_command_long_pack(255, 0, &message, 1, 250, MAV_CMD_COMPONENT_ARM_DISARM, 0, 0, 0, 0, 0, 0, 0, 0);
	//mavlink_msg_command_long_pack(system_id, component id, msg, target sys, target component, command, confirmation, par1, par2, par3, par4, par5, par6, par7);
	int len = mavlink_msg_to_send_buffer(buffer, &message);

	m_comm[2]->Send((const wchar_t*)buffer, len);
	//---------------------------------------------------------------------------------------
}

void CArdupilotKarpeDlg::OnBnClickedBtHovering1()
{
	// TODO: Add your control notification handler code here
	xgoal[0] = data.q[0][0];
	ygoal[0] = data.q[0][1];
	zgoal[0] = 600;

	data.SetGoal(0, xgoal[0], ygoal[0], zgoal[0]);
	data.Mode[0] = 1; // 1: hovering / 2: landing / 3: moving / 4: failsafe
}


void CArdupilotKarpeDlg::OnBnClickedBtLanding1()
{
	// TODO: Add your control notification handler code here
	xgoal[0] = data.q[0][0];
	ygoal[0] = data.q[0][1];
	zgoal[0] = 250;

	data.SetGoal(0, xgoal[0], ygoal[0], zgoal[0]);
	data.Mode[0] = 2; // 1: hovering / 2: landing / 3: moving / 4: failsafe
}


void CArdupilotKarpeDlg::OnBnClickedBtHovering2()
{
	// TODO: Add your control notification handler code here
	xgoal[1] = data.q[1][0];
	ygoal[1] = data.q[1][1];
	zgoal[1] = 600; 

	data.SetGoal(1, xgoal[0], ygoal[0], zgoal[0]);
	data.Mode[1] = 1; // 1: hovering / 2: landing / 3: moving / 4: failsafe
}


void CArdupilotKarpeDlg::OnBnClickedBtLanding2()
{
	// TODO: Add your control notification handler code here
	xgoal[1] = data.q[1][0];
	ygoal[1] = data.q[1][1];
	zgoal[1] = 250;

	data.SetGoal(1, xgoal[0], ygoal[0], zgoal[0]);
	data.Mode[1] = 2; // 1: hovering / 2: landing / 3: moving / 4: failsafe
}


void CArdupilotKarpeDlg::OnBnClickedBtHovering3()
{
	// TODO: Add your control notification handler code here
	xgoal[2] = data.q[2][0];
	ygoal[2] = data.q[2][1];
	zgoal[2] = 600;

	data.SetGoal(2, xgoal[0], ygoal[0], zgoal[0]);
	data.Mode[2] = 1; // 1: hovering / 2: landing / 3: moving / 4: failsafe
}


void CArdupilotKarpeDlg::OnBnClickedBtLanding3()
{
	// TODO: Add your control notification handler code here
	xgoal[2] = data.q[2][0];
	ygoal[2] = data.q[2][1];
	zgoal[2] = 250;

	data.SetGoal(2, xgoal[0], ygoal[0], zgoal[0]);
	data.Mode[2] = 2; // 1: hovering / 2: landing / 3: moving / 4: failsafe
}


void CArdupilotKarpeDlg::OnBnClickedBtNextpoint1()
{
	// TODO: Add your control notification handler code here

	static int step = 1;

	switch (step)
	{
	case 1:
		xgoal[0] = 700;
		ygoal[0] = 700;

		step++;
		break;
	case 2:
		xgoal[0] = -700;
		ygoal[0] = 700;

		step++;
		break;
	case 3:
		xgoal[0] = -700;
		ygoal[0] = -700;

		step++;
		break;
	case 4:
		xgoal[0] = 700;
		ygoal[0] = -700;

		step++;
		break;
	case 5:
		xgoal[0] = 700;
		ygoal[0] = 700;

		step = 1;
		break;
	}

	data.SetGoal(0, xgoal[0], ygoal[0], zgoal[0]);
	data.Mode[0] = 3; // 1: hovering / 2: landing / 3: moving / 4: failsafe
}




void CArdupilotKarpeDlg::OnBnClickedBtUp1()
{
	// TODO: Add your control notification handler code here
	if (zgoal[0] < 1000) zgoal[0] = zgoal[0] + 100;

	data.SetGoal(0, xgoal[0], ygoal[0], zgoal[0]);
}


void CArdupilotKarpeDlg::OnBnClickedBtDown1()
{
	// TODO: Add your control notification handler code here
	if (zgoal[0] > 300) zgoal[0] = zgoal[0] - 100;

	data.SetGoal(0, xgoal[0], ygoal[0], zgoal[0]);
}
