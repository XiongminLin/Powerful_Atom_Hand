
// whu_MFC_HandDlg.cpp : implementation file
//

#include "stdafx.h"
#include "whu_MFC_Hand.h"
#include "whu_MFC_HandDlg.h"
#include "afxdialogex.h"
#include "CvvImage.h"
#include <Afxwin.h>

#define  OPENNI_CONTEXT_ERROR 0
#define  OPENNI_CHECK_KINECT 1
#define  OPENNI_GENERATOR_ERROR 2
#define  OPENNI_USERREGISTER_ERROR 3
#define  OPENNI_OK 4
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define  WHU_GLOBALS
#include "whu_Golobal.h"

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


// Cwhu_MFC_HandDlg dialog




Cwhu_MFC_HandDlg::Cwhu_MFC_HandDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(Cwhu_MFC_HandDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_Language = 0;

}
Cwhu_MFC_HandDlg::~Cwhu_MFC_HandDlg()
{
	cvReleaseImage(&whu_Coverimg);
	cvReleaseImage(&whu_RecvImg);
	GpioClose(m_pIObj);
	cimg.Destroy();
	ReleaseDC(pDC); 
	//AfxMyHand.whu_closehand();
	//GpioClose(m_pIObj); 

}
void Cwhu_MFC_HandDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RCHINESE, m_Language);
	DDX_Control(pDX, IDC_IPADDRESS, whu_IpAddress);
	DDX_Control(pDX, IDC_SLIDER2, m_Slider1);
	DDX_Control(pDX, IDC_SLIDER3, m_Slider2);
	DDX_Control(pDX, IDC_SLIDER4, m_Slider3);
	DDX_Control(pDX, IDC_SLIDER5, m_Slider4);
	DDX_Control(pDX, IDC_SLIDER6, m_Slider5);
}

BEGIN_MESSAGE_MAP(Cwhu_MFC_HandDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &Cwhu_MFC_HandDlg::OnBnClickedOk)
	ON_ACN_START(IDC_ANIMATE1, &Cwhu_MFC_HandDlg::OnAcnStartAnimate1)
	ON_BN_CLICKED(IDC_BSTART, &Cwhu_MFC_HandDlg::OnBnClickedBstart)
	ON_BN_CLICKED(IDC_BSTOP, &Cwhu_MFC_HandDlg::OnBnClickedBstop)
	ON_BN_CLICKED(IDC_BSET, &Cwhu_MFC_HandDlg::OnBnClickedBset)

	ON_BN_CLICKED(IDC_BSETOK, &Cwhu_MFC_HandDlg::OnBnClickedBsetok)
	ON_BN_CLICKED(IDC_BSETCANCLE, &Cwhu_MFC_HandDlg::OnBnClickedBsetcancle)
	ON_BN_CLICKED(IDC_RCHINESE, &Cwhu_MFC_HandDlg::OnBnClickedRchinese)
	ON_BN_CLICKED(IDC_RENGLISH, &Cwhu_MFC_HandDlg::OnBnClickedRenglish)
	ON_BN_CLICKED(IDC_LOCAL, &Cwhu_MFC_HandDlg::OnBnClickedLocal)
	ON_BN_CLICKED(IDC_SERIALPORT, &Cwhu_MFC_HandDlg::OnBnClickedSerialport)
	ON_BN_CLICKED(IDC_SOCKET, &Cwhu_MFC_HandDlg::OnBnClickedSocket)
	ON_BN_CLICKED(IDC_PICSTART, &Cwhu_MFC_HandDlg::OnBnClickedPicstart)
	ON_MESSAGE(WM_DISPLAYANGLE,DisplayAngle)
	ON_MESSAGE(WM_DISPLAYVIDEO,DisplayVideo)

	ON_BN_CLICKED(IDC_SWITCH, &Cwhu_MFC_HandDlg::OnBnClickedSwitch)
	ON_BN_CLICKED(IDC_FULLSCREEN, &Cwhu_MFC_HandDlg::OnBnClickedFullscreen)
	ON_STN_DBLCLK(IDC_PIC, &Cwhu_MFC_HandDlg::OnStnDblclickPic)
	ON_BN_CLICKED(IDC_CLIENTMODE, &Cwhu_MFC_HandDlg::OnBnClickedClientmode)
	ON_BN_CLICKED(IDC_LOCALMODE, &Cwhu_MFC_HandDlg::OnBnClickedLocalmode)
	ON_BN_CLICKED(IDC_NOBODY, &Cwhu_MFC_HandDlg::OnBnClickedNobody)
	ON_BN_CLICKED(IDC_BODY, &Cwhu_MFC_HandDlg::OnBnClickedBody)
END_MESSAGE_MAP()


// Cwhu_MFC_HandDlg message handlers

BOOL Cwhu_MFC_HandDlg::OnInitDialog()
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

	

	whu_Run=true;
	CvSize m_size;
	m_size.height=480; //zzzz
	m_size.width=640; //zzzzzz
	whu_RecvImg=cvCreateImage(m_size,IPL_DEPTH_8U,3);


	//char m_ip[20]="127.0.0.1";
	unsigned char m_ip[4]={192,168,16,3};
	whu_IpAddress.SetAddress(m_ip[0],m_ip[1],m_ip[2],m_ip[3]);
	int port=4;
	GetDlgItem(IDC_PORT)->SetWindowTextA(_T("4"));
	GetDlgItem(IDC_BAUDRATE)->SetWindowTextA("9600");
	GetDlgItem(IDC_REMOTEPORT)->SetWindowTextA("4600");
	GetDlgItem(IDC_LOCALPORT)->SetWindowTextA("1700");


	for (int i=0;i<5;i++)
	{
		whu_angle[i] = 90;
	}

	m_PauseControl = true;
	m_FullScreen = false;
	SystemMode = 0;
	m_bVideo = false;
	InitDone[0] = false;
	InitDone[1] = false;
	InitDone[2] = false;



	DWORD dwErr = GpioCreate(&m_pIObj,0);

	pDC = GetDlgItem(IDC_PIC)->GetDC();

	//【设置窗口隐藏】//
	CRect SpeLeRec;
	GetWindowRect(&m_large);
	GetDlgItem(IDC_LINE1)->GetWindowRect(&SpeLeRec);
	m_small.bottom = m_large.bottom;
	m_small.left = m_large.left;
	m_small.top = m_large.top;
	m_small.right = SpeLeRec.right;
	SetWindowPos(NULL,0,0,m_small.Width(),m_small.Height(),SWP_NOMOVE|SWP_NOZORDER);
	int nFullWidth=GetSystemMetrics(SM_CXSCREEN);
	int nFullHeight=GetSystemMetrics(SM_CYSCREEN);
	m_FullScreenRect.left = 0;
	m_FullScreenRect.top = 0;
	m_FullScreenRect.bottom = nFullHeight;
	m_FullScreenRect.right = nFullWidth;
	GetDlgItem(IDC_PIC)->GetWindowRect(m_NormalScreenRect);  //非全屏模式的矩形框

	whu_HideAllItem();
	GetDlgItem(IDC_ALL)->ShowWindow(true);
	GetDlgItem(IDC_WARNING)->ShowWindow(true);
	GetDlgItem(IDC_BSTART)->ShowWindow(true);
	GetDlgItem(IDC_BSET)->ShowWindow(true);
	GetDlgItem(IDC_CLIENTMODE)->ShowWindow(true);
	GetDlgItem(IDC_LOCALMODE)->ShowWindow(true);
	GetDlgItem(IDC_TIME)->ShowWindow(true);
	GetDlgItem(IDC_STATICTIME)->ShowWindow(true);
	m_Slider1.SetRange(0,180);
	m_Slider1.SetTicFreq(1);
	m_Slider2.SetRange(0,180);
	m_Slider2.SetTicFreq(1);
	m_Slider3.SetRange(0,180);
	m_Slider3.SetTicFreq(1);
	m_Slider4.SetRange(0,180);
	m_Slider4.SetTicFreq(1);
	m_Slider5.SetRange(0,180);
	m_Slider5.SetTicFreq(1);
	m_Slider1.SetPos(90);
	m_Slider2.SetPos(90);
	m_Slider3.SetPos(90);
	m_Slider4.SetPos(90);
	m_Slider5.SetPos(90);

	return TRUE;  // return TRUE  unless you set the focus to a control

}

void Cwhu_MFC_HandDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void Cwhu_MFC_HandDlg::OnPaint()
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
	//whu_RecvImg= cvLoadImage("图书馆.jpg");

	if (m_bVideo == false)
	{
		whu_Coverimg= cvLoadImage("E:\\樱顶.jpg");
		DrawPicToHDC(whu_Coverimg,IDC_PIC);
	}

}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR Cwhu_MFC_HandDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void Cwhu_MFC_HandDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}


void Cwhu_MFC_HandDlg::OnAcnStartAnimate1()
{
	// TODO: Add your control notification handler code here
}


void Cwhu_MFC_HandDlg::OnEnChangeEdit1()
{

}

bool Cwhu_MFC_HandDlg::whu_Start()
{
	return true;
}



void Cwhu_MFC_HandDlg::OnBnClickedBstop()
{
	// TODO: Add your control notification handler code here
		whu_Run=false;
		CDialogEx::OnOK();
}


void Cwhu_MFC_HandDlg::OnBnClickedBset()
{
	CString m_s;
	if (GetDlgItem(IDC_BSET)->GetWindowTextA(m_s),m_s=="设置")
	{
		GetDlgItem(IDC_BSET)->SetWindowTextA("隐藏");
		SetWindowPos(NULL,0,0,m_large.Width(),m_large.Height(),SWP_NOMOVE|SWP_NOZORDER);
		GetDlgItem(IDC_LANGUAGE)->ShowWindow(true);
		GetDlgItem(IDC_RCHINESE)->ShowWindow(true);
		GetDlgItem(IDC_RENGLISH)->ShowWindow(true);
		GetDlgItem(IDC_STATIC44)->ShowWindow(true);
		GetDlgItem(IDC_STATICPORT)->ShowWindow(true);
		GetDlgItem(IDC_PORT)->ShowWindow(true);
		GetDlgItem(IDC_STATICBOTE)->ShowWindow(true);
		GetDlgItem(IDC_BAUDRATE)->ShowWindow(true);
		GetDlgItem(IDC_STATICNET)->ShowWindow(true);
		GetDlgItem(IDC_STATICIP)->ShowWindow(true);
		GetDlgItem(IDC_IPADDRESS)->ShowWindow(true);
		GetDlgItem(IDC_STATIC33)->ShowWindow(true);
		GetDlgItem(IDC_REMOTEPORT)->ShowWindow(true);
		GetDlgItem(IDC_STATIC22)->ShowWindow(true);	
		GetDlgItem(IDC_LOCALPORT)->ShowWindow(true);
		GetDlgItem(IDC_BSETCANCLE)->ShowWindow(true);
		GetDlgItem(IDC_BSETOK)->ShowWindow(true);
		GetDlgItem(IDC_LINE1)->ShowWindow(true);	
		GetDlgItem(IDC_LINE2)->ShowWindow(true);
	}
	else
	{
		GetDlgItem(IDC_BSET)->SetWindowTextA("设置");
		SetWindowPos(NULL,0,0,m_small.Width(),m_small.Height(),SWP_NOMOVE|SWP_NOZORDER);
	}
	// TODO: Add your control notification handler code here
}


void Cwhu_MFC_HandDlg::OnEnChangeEdit6()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

void Cwhu_MFC_HandDlg::whu_ClearAngleShake(double *TempAngle,double *FinalAngle,int MaxChange)
{
	int m_value = 10;
	static bool n_first=true;
	if(n_first){	          //第一次读取仅赋值不比较
		for(int i=0;i<5;i++){
			FinalAngle[i] = TempAngle[i];    	
		}
		n_first = false;
	}
	else    //第二次之后开始比较，满足要求后才赋值//
	{
		//【舵机1】//
		if (TempAngle[0]-FinalAngle[0]>=m_value&&TempAngle[0]-FinalAngle[0]<=90)
		{
			FinalAngle[0]+=MaxChange;   //如果电脑运行很快，那么这个值就取小一点//

		} 
		else if(FinalAngle[0]-TempAngle[0]>=m_value&&FinalAngle[0]-TempAngle[0]<=90)
		{
			FinalAngle[0]-=MaxChange;

		}
		//【舵机2】
		if ((TempAngle[1]-FinalAngle[1]>=m_value&&TempAngle[1]-FinalAngle[1]<=90))
		{
			FinalAngle[1]+=MaxChange;
		} 
		else if((FinalAngle[1]-TempAngle[1]>=m_value&&FinalAngle[1]-TempAngle[1]<=90))
		{
			FinalAngle[1]-=MaxChange;
		}
		//【舵机3】
		if ((TempAngle[2]-FinalAngle[2]>=m_value&&TempAngle[2]-FinalAngle[2]<=90))
		{
			FinalAngle[2]+=MaxChange;
		} 
		else if((FinalAngle[2]-TempAngle[2]>=m_value&&FinalAngle[2]-TempAngle[2]<=90))
		{
			FinalAngle[2]-=MaxChange; 
		}
		//【舵机4 控制平衡舵机】
		if (TempAngle[3]-FinalAngle[3]>=m_value&&TempAngle[3]-FinalAngle[3]<=90)
		{
			FinalAngle[3]+=MaxChange;
		} 
		else if(FinalAngle[3]-TempAngle[3]>=m_value&&FinalAngle[3]-TempAngle[3]<=90)
		{
			FinalAngle[3]-=MaxChange;
		}
		////【舵机5 控制手掌闭合】
		if (TempAngle[4]-FinalAngle[4]>=15) //不可取=
		{
			FinalAngle[4]+=15;
		} 
		else if(FinalAngle[4]-TempAngle[4]>=15)
		{
			FinalAngle[4]-=15;
		}
		for (int j=0;j<5;j++)
		{
			if(FinalAngle[j]>=175) FinalAngle[j]=175;
			if (FinalAngle[j]<5) FinalAngle[j]= 5;
		}
	}
}

void Cwhu_MFC_HandDlg::OnBnClickedBsetok()
{

	SetWindowPos(NULL,0,0,m_small.Width(),m_small.Height(),SWP_NOMOVE|SWP_NOZORDER);
	GetDlgItem(IDC_BSET)->SetWindowTextA("设置");

	// TODO: Add your control notification handler code here
}
void Cwhu_MFC_HandDlg::OnBnClickedBsetcancle()
{
	
	SetWindowPos(NULL,0,0,m_small.Width(),m_small.Height(),SWP_NOMOVE|SWP_NOZORDER);
	GetDlgItem(IDC_BSET)->SetWindowTextA("设置");
	// TODO: Add your control notification handler code here
}
void Cwhu_MFC_HandDlg::OnBnClickedRchinese()
{
	// TODO: Add your control notification handler code here
	m_Language=0;
}
void Cwhu_MFC_HandDlg::OnBnClickedRenglish()
{
	// TODO: Add your control notification handler code here
	m_Language=1;
}
void Cwhu_MFC_HandDlg::OnBnClickedLocal()
{
	// TODO: Add your control notification handler code here

}
void Cwhu_MFC_HandDlg::OnBnClickedSerialport()
{
	// TODO: Add your control notification handler code here

}
void Cwhu_MFC_HandDlg::OnBnClickedSocket()
{
	// TODO: Add your control notification handler code here

}
void Cwhu_MFC_HandDlg::OnBnClickedPicstart()
{

}


void Cwhu_MFC_HandDlg::whu_ShowAngles(double *angle)
{
	char angleshow[5][10];
	int m_angle[5];
	for (int i=0;i<5;i++)
	{
		m_angle[i]=(int)angle[i];
		itoa(m_angle[i],angleshow[i],10);
	}
	GetDlgItem(IDC_ANGLE1)->SetWindowTextA(angleshow[0]);
	GetDlgItem(IDC_ANGLE2)->SetWindowTextA(angleshow[1]);
	GetDlgItem(IDC_ANGLE3)->SetWindowTextA(angleshow[2]);
	GetDlgItem(IDC_ANGLE4)->SetWindowTextA(angleshow[3]);
	GetDlgItem(IDC_ANGLE5)->SetWindowTextA(angleshow[4]);
	m_Slider1.SetPos(m_angle[0]);
	m_Slider2.SetPos(m_angle[1]);
	m_Slider3.SetPos(m_angle[2]);
	m_Slider4.SetPos(m_angle[3]);
	m_Slider5.SetPos(m_angle[4]);
	m_Slider1.SetPos(m_angle[0]);
	m_Slider2.SetPos(m_angle[1]);
	m_Slider3.SetPos(m_angle[2]);
	m_Slider4.SetPos(m_angle[3]);
	m_Slider5.SetPos(m_angle[4]);
}


void Cwhu_MFC_HandDlg::whu_ShowOpenNiError(int message)
{
	switch (message)
	{
	case OPENNI_CONTEXT_ERROR :
		GetDlgItem(IDC_WARNING)->SetWindowTextA("OpenNi Context Initialize Error");
		break;
	case OPENNI_CHECK_KINECT :
		GetDlgItem(IDC_WARNING)->SetWindowTextA("OpenNi Error:Please Check Your Kinect");
		break;
	case OPENNI_GENERATOR_ERROR :
		GetDlgItem(IDC_WARNING)->SetWindowTextA("OpenNi Generator Create Error");
		break;
	case OPENNI_USERREGISTER_ERROR:
		GetDlgItem(IDC_WARNING)->SetWindowTextA("OpenNi User Register Error");
		break;
	case OPENNI_OK:
		GetDlgItem(IDC_WARNING)->SetWindowTextA("OpenNi Initialize Success");
		break;
	default:	
		GetDlgItem(IDC_WARNING)->SetWindowTextA("OpenNi Error Not Register");
		break;
	}
}



void Cwhu_MFC_HandDlg::OnBnClickedSwitch()
{
	CString m_str;
	GetDlgItemText(IDC_SWITCH,m_str);
	if (m_str == "切至本地监控")
	{
		SetDlgItemText(IDC_SWITCH,"切至远程监控");
		
	} 
	else if(m_str =="切至远程监控")
	{
		SetDlgItemText(IDC_SWITCH,"切至本地监控");
	}

	if (m_gKinectVideo)
	{
		m_gKinectVideo = false;
	} 
	else
	{
		m_gKinectVideo = true;
	}

	if (m_gCamVideo)
	{
		m_gCamVideo = false;
	} 
	else
	{
		m_gCamVideo = true;
	}

	if (m_gRecvVideo)
	{
		m_gRecvVideo = false;
	} 
	else
	{
		m_gRecvVideo = true;
	}
	// TODO: Add your control notification handler code here
}

void Cwhu_MFC_HandDlg::OnStnDblclickPic()//双击屏幕全屏
{
	CString str;
	GetDlgItemText(IDC_FULLSCREEN,str);
	if (str=="全屏")
	{
		GetDlgItem(IDC_FULLSCREEN)->SetWindowTextA("退出全屏");
		//GetDlgItem(IDC_FULLSCREEN)->MoveWindow(m_FullScreenRect.right-200,m_FullScreenRect.bottom-200,100,100);
		m_FullScreen = TRUE;
		whu_SetScreen();
	} 
	else
	{
		GetDlgItem(IDC_FULLSCREEN)->SetWindowTextA("全屏");
		m_FullScreen = false;
		whu_SetScreen();
	}
	// TODO: Add your control notification handler code here
}

void Cwhu_MFC_HandDlg::OnBnClickedFullscreen()
{
	
	CString str;
	GetDlgItemText(IDC_FULLSCREEN,str);
	if (str=="全屏")
	{
		GetDlgItem(IDC_FULLSCREEN)->SetWindowTextA("退出全屏");
		//GetDlgItem(IDC_FULLSCREEN)->MoveWindow(m_FullScreenRect.right-200,m_FullScreenRect.bottom-200,100,100);
		m_FullScreen = TRUE;
		whu_SetScreen();
	} 
	else
	{
		GetDlgItem(IDC_FULLSCREEN)->SetWindowTextA("全屏");
		m_FullScreen = false;
		whu_SetScreen();
	}
	
	// TODO: Add your control notification handler code here
}
void Cwhu_MFC_HandDlg::whu_SetScreen()
{
	static bool LastScreenMode = false;
	if (LastScreenMode != m_FullScreen)
	{
		if (m_FullScreen)
		{
			whu_HideAllItem();
			GetDlgItem(IDC_PIC)->SetWindowPos(NULL,0,0,m_FullScreenRect.Width(),m_FullScreenRect.Height(),
				SWP_NOMOVE|SWP_NOZORDER);
			MoveWindow(0,0,m_FullScreenRect.Width(),m_FullScreenRect.Height());
			SetWindowPos(NULL,0,0,m_FullScreenRect.Width(),m_FullScreenRect.Height(),
						SWP_NOMOVE|SWP_NOZORDER);		
		} 
		else
		{
			whu_ShowAllItem();
			GetDlgItem(IDC_PIC)->SetWindowPos(NULL,0,0,m_NormalScreenRect.Width(),
												m_NormalScreenRect.Height(),SWP_NOMOVE|SWP_NOZORDER);
			SetWindowPos(NULL,0,0,m_small.Width(),m_small.Height(),
							SWP_NOMOVE|SWP_NOZORDER);
		}
		LastScreenMode = m_FullScreen;//保证不用每次都变
	}

}
void Cwhu_MFC_HandDlg::whu_ShowAllItem()
{
	if (InitDone[0]||InitDone[1]||InitDone[2])
	{
			GetDlgItem(IDC_NO1)->ShowWindow(true);
			GetDlgItem(IDC_NO2)->ShowWindow(true);
			GetDlgItem(IDC_NO3)->ShowWindow(true);
			GetDlgItem(IDC_NO4)->ShowWindow(true);
			GetDlgItem(IDC_NO5)->ShowWindow(true);
			GetDlgItem(IDC_ANGLE1)->ShowWindow(true);
			GetDlgItem(IDC_ANGLE2)->ShowWindow(true);
			GetDlgItem(IDC_ANGLE3)->ShowWindow(true);
			GetDlgItem(IDC_ANGLE4)->ShowWindow(true);
			GetDlgItem(IDC_ANGLE5)->ShowWindow(true);

			GetDlgItem(IDC_LR)->ShowWindow(true);
			GetDlgItem(IDC_FB)->ShowWindow(true);
			GetDlgItem(IDC_DU)->ShowWindow(true);
			GetDlgItem(IDC_PINGHENG)->ShowWindow(true);
			GetDlgItem(IDC_OPEN)->ShowWindow(true);
			GetDlgItem(IDC_SLIDER2)->ShowWindow(true);
			GetDlgItem(IDC_SLIDER3)->ShowWindow(true);
			GetDlgItem(IDC_SLIDER4)->ShowWindow(true);
			GetDlgItem(IDC_SLIDER5)->ShowWindow(true);
			GetDlgItem(IDC_SLIDER6)->ShowWindow(true);

		//GetDlgItem(IDC_DUOJI)->ShowWindow(true);
		if(SystemMode == 0 ){
			GetDlgItem(IDC_SWITCH)->ShowWindow(true);//
			GetDlgItem(IDC_BSTOP)->ShowWindow(true);//
			GetDlgItem(IDC_FULLSCREEN)->ShowWindow(true);//
		}
		if(SystemMode == 1 ){
			GetDlgItem(IDC_BSTOP)->ShowWindow(true);//
			GetDlgItem(IDC_FULLSCREEN)->ShowWindow(true);//
		}
		if(SystemMode == 2 ){

			GetDlgItem(IDC_BSTOP)->ShowWindow(true);//
			GetDlgItem(IDC_FULLSCREEN)->ShowWindow(true);//
		}

	} 
	else
	{
		GetDlgItem(IDC_BSET)->ShowWindow(TRUE);
		GetDlgItem(IDC_BSTART)->ShowWindow(true);//
		GetDlgItem(IDC_LOCALMODE)->ShowWindow(true);//
		GetDlgItem(IDC_CLIENTMODE)->ShowWindow(true);//

	}
	

	GetDlgItem(IDC_WARNING)->ShowWindow(true);

	//GetDlgItem(IDC_CONTROL)->ShowWindow(true);

	GetDlgItem(IDC_LANGUAGE)->ShowWindow(true);
	GetDlgItem(IDC_RCHINESE)->ShowWindow(true);
	GetDlgItem(IDC_RENGLISH)->ShowWindow(true);
	GetDlgItem(IDC_STATIC44)->ShowWindow(true);
	GetDlgItem(IDC_STATICPORT)->ShowWindow(true);
	GetDlgItem(IDC_PORT)->ShowWindow(true);
	GetDlgItem(IDC_STATICBOTE)->ShowWindow(true);
	GetDlgItem(IDC_BAUDRATE)->ShowWindow(true);
	GetDlgItem(IDC_STATICNET)->ShowWindow(true);
	GetDlgItem(IDC_STATICIP)->ShowWindow(true);
	GetDlgItem(IDC_IPADDRESS)->ShowWindow(true);
	GetDlgItem(IDC_STATIC33)->ShowWindow(true);
	GetDlgItem(IDC_REMOTEPORT)->ShowWindow(true);
	GetDlgItem(IDC_STATIC22)->ShowWindow(true);	
	GetDlgItem(IDC_LOCALPORT)->ShowWindow(true);
	GetDlgItem(IDC_BSETCANCLE)->ShowWindow(true);
	GetDlgItem(IDC_BSETOK)->ShowWindow(true);
	GetDlgItem(IDC_LINE1)->ShowWindow(true);	
	GetDlgItem(IDC_LINE2)->ShowWindow(true);
	GetDlgItem(IDC_STATICTIME)->ShowWindow(true);
	GetDlgItem(IDC_TIME)->ShowWindow(true);
}
void Cwhu_MFC_HandDlg::whu_HideAllItem()
{
	GetDlgItem(IDC_ANGLE1)->ShowWindow(false);
	GetDlgItem(IDC_ANGLE2)->ShowWindow(false);
	GetDlgItem(IDC_ANGLE3)->ShowWindow(false);
	GetDlgItem(IDC_ANGLE4)->ShowWindow(false);
	GetDlgItem(IDC_ANGLE5)->ShowWindow(false);
	GetDlgItem(IDC_NO1)->ShowWindow(false);		
	GetDlgItem(IDC_NO2)->ShowWindow(false);	
	GetDlgItem(IDC_NO3)->ShowWindow(false);
	GetDlgItem(IDC_NO4)->ShowWindow(false);	
	GetDlgItem(IDC_NO5)->ShowWindow(false);
	GetDlgItem(IDC_FULLSCREEN)->ShowWindow(false);
	GetDlgItem(IDC_SWITCH)->ShowWindow(false);
	GetDlgItem(IDC_WARNING)->ShowWindow(false);
	//GetDlgItem(IDC_DUOJI)->ShowWindow(false);
	//GetDlgItem(IDC_CONTROL)->ShowWindow(false);
	GetDlgItem(IDC_CLIENTMODE)->ShowWindow(false);
	GetDlgItem(IDC_LOCALMODE)->ShowWindow(false);
	GetDlgItem(IDC_BSTART)->ShowWindow(false);
	GetDlgItem(IDC_BSET)->ShowWindow(false);
	GetDlgItem(IDC_SWITCH)->ShowWindow(false);
	GetDlgItem(IDC_BSTOP)->ShowWindow(false);
	GetDlgItem(IDC_FULLSCREEN)->ShowWindow(false);
	GetDlgItem(IDC_LANGUAGE)->ShowWindow(false);
	GetDlgItem(IDC_RCHINESE)->ShowWindow(false);
	GetDlgItem(IDC_RENGLISH)->ShowWindow(false);
	GetDlgItem(IDC_STATIC44)->ShowWindow(false);
	GetDlgItem(IDC_STATICPORT)->ShowWindow(false);
	GetDlgItem(IDC_PORT)->ShowWindow(false);
	GetDlgItem(IDC_STATICBOTE)->ShowWindow(false);
	GetDlgItem(IDC_BAUDRATE)->ShowWindow(false);
	GetDlgItem(IDC_STATICNET)->ShowWindow(false);
	GetDlgItem(IDC_STATICIP)->ShowWindow(false);
	GetDlgItem(IDC_IPADDRESS)->ShowWindow(false);
	GetDlgItem(IDC_STATIC33)->ShowWindow(false);
	GetDlgItem(IDC_REMOTEPORT)->ShowWindow(false);
	GetDlgItem(IDC_STATIC22)->ShowWindow(false);	
	GetDlgItem(IDC_LOCALPORT)->ShowWindow(false);
	GetDlgItem(IDC_BSETCANCLE)->ShowWindow(false);
	GetDlgItem(IDC_BSETOK)->ShowWindow(false);
	GetDlgItem(IDC_LINE1)->ShowWindow(false);	
	GetDlgItem(IDC_LINE2)->ShowWindow(false);
	GetDlgItem(IDC_NOBODY)->ShowWindow(false);
	GetDlgItem(IDC_BODY)->ShowWindow(false);
	GetDlgItem(IDC_SLIDER2)->ShowWindow(false);
	GetDlgItem(IDC_SLIDER3)->ShowWindow(false);
	GetDlgItem(IDC_SLIDER4)->ShowWindow(false);
	GetDlgItem(IDC_SLIDER5)->ShowWindow(false);
	GetDlgItem(IDC_SLIDER6)->ShowWindow(false);
	GetDlgItem(IDC_ALL)->ShowWindow(false);
	GetDlgItem(IDC_LR)->ShowWindow(false);
	GetDlgItem(IDC_FB)->ShowWindow(false);
	GetDlgItem(IDC_DU)->ShowWindow(false);
	GetDlgItem(IDC_PINGHENG)->ShowWindow(false);
	GetDlgItem(IDC_OPEN)->ShowWindow(false);
	GetDlgItem(IDC_STATICTIME)->ShowWindow(false);
	GetDlgItem(IDC_TIME)->ShowWindow(false);
	
}


void Cwhu_MFC_HandDlg::OnBnClickedNobody()
{
}
void Cwhu_MFC_HandDlg::OnBnClickedBody()
{
	// TODO: Add your control notification handler code here
}
bool Cwhu_MFC_HandDlg::whu_GetNobodyAngle(double* angle)
{
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////
void Cwhu_MFC_HandDlg::OnBnClickedBstart()//服务器端模式
{

	GetDlgItem(IDC_BSTART)->ShowWindow(FALSE);
	GetDlgItem(IDC_BSET)->ShowWindow(false);
	GetDlgItem(IDC_NO1)->ShowWindow(true);
	GetDlgItem(IDC_NO2)->ShowWindow(true);
	GetDlgItem(IDC_NO3)->ShowWindow(true);
	GetDlgItem(IDC_NO4)->ShowWindow(true);
	GetDlgItem(IDC_NO5)->ShowWindow(true);
	GetDlgItem(IDC_ANGLE1)->ShowWindow(true);
	GetDlgItem(IDC_ANGLE2)->ShowWindow(true);
	GetDlgItem(IDC_ANGLE3)->ShowWindow(true);
	GetDlgItem(IDC_ANGLE4)->ShowWindow(true);
	GetDlgItem(IDC_ANGLE5)->ShowWindow(true);

	GetDlgItem(IDC_LR)->ShowWindow(true);
	GetDlgItem(IDC_FB)->ShowWindow(true);
	GetDlgItem(IDC_DU)->ShowWindow(true);
	GetDlgItem(IDC_PINGHENG)->ShowWindow(true);
	GetDlgItem(IDC_OPEN)->ShowWindow(true);
	GetDlgItem(IDC_SLIDER2)->ShowWindow(true);
	GetDlgItem(IDC_SLIDER3)->ShowWindow(true);
	GetDlgItem(IDC_SLIDER4)->ShowWindow(true);
	GetDlgItem(IDC_SLIDER5)->ShowWindow(true);
	GetDlgItem(IDC_SLIDER6)->ShowWindow(true);



	//GetDlgItem(IDC_DUOJI)->ShowWindow(true);
	GetDlgItem(IDC_CLIENTMODE)->ShowWindow(FALSE);
	GetDlgItem(IDC_LOCALMODE)->ShowWindow(FALSE);
	GetDlgItem(IDC_BSTOP)->ShowWindow(true);
	GetDlgItem(IDC_SWITCH)->ShowWindow(TRUE);
	GetDlgItem(IDC_FULLSCREEN)->ShowWindow(TRUE);

	GetDlgItem(IDC_WARNING)->SetWindowText("The Application of Server Mode has Started ,Please Wait For 10 Seconds");
	SystemMode = 0;
	whu_IpAddress.GetAddress(m_GRemoteIpAddRess[0],m_GRemoteIpAddRess[1],m_GRemoteIpAddRess[2],m_GRemoteIpAddRess[3]);
	m_GRemotePort = GetDlgItemInt(IDC_REMOTEPORT);
	m_GLocalPort = GetDlgItemInt(IDC_LOCALPORT);
	//m_LocalVideo = true;
	m_gKinectVideo = true;
	m_gRecvVideo = false;
	m_gStopArm = false;
	InitDone[0]=true;
	if (InitDone[0])
	{

		m_KinThread=(CKinectThread*)AfxBeginThread(RUNTIME_CLASS(CKinectThread));
		m_KinThread->PostThreadMessage(WM_GETANGLE,NULL,NULL);
		Sleep(10);	
		m_RtpSendAngleThread = (CRecvPicSendAngle*)AfxBeginThread(RUNTIME_CLASS(CRecvPicSendAngle));
		m_RtpSendAngleThread->PostThreadMessageA(WM_SENDANGLE,NULL,NULL);
		Sleep(10);
	}
	// TODO: Add your control notification handler code here
}

///////////////////////////////////////////////////////////////////////////////////////////
void Cwhu_MFC_HandDlg::OnBnClickedClientmode()  //客户端模式
{
	// TODO: Add your control notification handler code here


	GetDlgItem(IDC_BSTART)->ShowWindow(FALSE);
	GetDlgItem(IDC_CLIENTMODE)->ShowWindow(false);
	GetDlgItem(IDC_LOCALMODE)->ShowWindow(false);
	GetDlgItem(IDC_BSET)->ShowWindow(false);
	GetDlgItem(IDC_BSTOP)->ShowWindow(true);
	GetDlgItem(IDC_FULLSCREEN)->ShowWindow(true);
	GetDlgItem(IDC_NO1)->ShowWindow(true);
	GetDlgItem(IDC_NO2)->ShowWindow(true);
	GetDlgItem(IDC_NO3)->ShowWindow(true);
	GetDlgItem(IDC_NO4)->ShowWindow(true);
	GetDlgItem(IDC_NO5)->ShowWindow(true);
	GetDlgItem(IDC_ANGLE1)->ShowWindow(true);
	GetDlgItem(IDC_ANGLE2)->ShowWindow(true);
	GetDlgItem(IDC_ANGLE3)->ShowWindow(true);
	GetDlgItem(IDC_ANGLE4)->ShowWindow(true);
	GetDlgItem(IDC_ANGLE5)->ShowWindow(true);

	GetDlgItem(IDC_LR)->ShowWindow(true);
	GetDlgItem(IDC_FB)->ShowWindow(true);
	GetDlgItem(IDC_DU)->ShowWindow(true);
	GetDlgItem(IDC_PINGHENG)->ShowWindow(true);
	GetDlgItem(IDC_OPEN)->ShowWindow(true);
	GetDlgItem(IDC_SLIDER2)->ShowWindow(true);
	GetDlgItem(IDC_SLIDER3)->ShowWindow(true);
	GetDlgItem(IDC_SLIDER4)->ShowWindow(true);
	GetDlgItem(IDC_SLIDER5)->ShowWindow(true);
	GetDlgItem(IDC_SLIDER6)->ShowWindow(true);


	//GetDlgItem(IDC_DUOJI)->ShowWindow(true);

	SystemMode = 1;
	GetDlgItem(IDC_WARNING)->SetWindowText("The Application of Client Mode has Started ,Please Wait For 10 Seconds");
	whu_IpAddress.GetAddress(m_GRemoteIpAddRess[0],m_GRemoteIpAddRess[1],m_GRemoteIpAddRess[2],m_GRemoteIpAddRess[3]);
	m_GRemotePort = GetDlgItemInt(IDC_REMOTEPORT);
	m_GLocalPort = GetDlgItemInt(IDC_LOCALPORT);
	m_gCamVideo = true;
	m_gStopArm = false;
	//DWORD dwErr = GpioCreate(&m_pIObj,0);
	InitDone[1]=true;
	if (InitDone[1])
	{
		m_RtpRecvAngleThread = (CRecvAngleSendPicThread*)AfxBeginThread(RUNTIME_CLASS(CRecvAngleSendPicThread));
		m_RtpRecvAngleThread->PostThreadMessage(WM_RECVANGLE,NULL,NULL);
		Sleep(10);

		m_fpgathread = (CFPGAThread*)AfxBeginThread(RUNTIME_CLASS(CFPGAThread));
		m_fpgathread->PostThreadMessage(WM_SENDFPGAANGLE,NULL,NULL);
		Sleep(10);
	}

}

///////////////////////////////////////////////////////////////////////////////////////////

void Cwhu_MFC_HandDlg::OnBnClickedLocalmode()//单机模式//
{
	// TODO: Add your control notification handler code here
	SystemMode = 2;
	//bool init=false;
	GetDlgItem(IDC_BSTART)->ShowWindow(FALSE);
	GetDlgItem(IDC_CLIENTMODE)->ShowWindow(FALSE);
	GetDlgItem(IDC_LOCALMODE)->ShowWindow(FALSE);
	GetDlgItem(IDC_BSET)->ShowWindow(false);
	GetDlgItem(IDC_BSTOP)->ShowWindow(true);
	GetDlgItem(IDC_FULLSCREEN)->ShowWindow(true);
	GetDlgItem(IDC_NO1)->ShowWindow(true);
	GetDlgItem(IDC_NO2)->ShowWindow(true);
	GetDlgItem(IDC_NO3)->ShowWindow(true);
	GetDlgItem(IDC_NO4)->ShowWindow(true);
	GetDlgItem(IDC_NO5)->ShowWindow(true);
	GetDlgItem(IDC_ANGLE1)->ShowWindow(true);
	GetDlgItem(IDC_ANGLE2)->ShowWindow(true);
	GetDlgItem(IDC_ANGLE3)->ShowWindow(true);
	GetDlgItem(IDC_ANGLE4)->ShowWindow(true);
	GetDlgItem(IDC_ANGLE5)->ShowWindow(true);

	GetDlgItem(IDC_LR)->ShowWindow(true);
	GetDlgItem(IDC_FB)->ShowWindow(true);
	GetDlgItem(IDC_DU)->ShowWindow(true);
	GetDlgItem(IDC_PINGHENG)->ShowWindow(true);
	GetDlgItem(IDC_OPEN)->ShowWindow(true);
	GetDlgItem(IDC_SLIDER2)->ShowWindow(true);
	GetDlgItem(IDC_SLIDER3)->ShowWindow(true);
	GetDlgItem(IDC_SLIDER4)->ShowWindow(true);
	GetDlgItem(IDC_SLIDER5)->ShowWindow(true);
	GetDlgItem(IDC_SLIDER6)->ShowWindow(true);


	//GetDlgItem(IDC_DUOJI)->ShowWindow(true);
	GetDlgItem(IDC_WARNING)->SetWindowText("The Application of Local Mode  has Started ,Please Wait For 10 Seconds");
	//whu_IpAddress.GetAddress(m_GRemoteIpAddRess[0],m_GRemoteIpAddRess[1],m_GRemoteIpAddRess[2],m_GRemoteIpAddRess[3]);
	//m_GRemotePort = GetDlgItemInt(IDC_REMOTEPORT);
	//m_GLocalPort = GetDlgItemInt(IDC_LOCALPORT);

	m_gKinectVideo = true;
	m_gCamVideo = false;
	m_gStopArm = false;
	InitDone[2]=true;
	if (InitDone[2])
	{
		m_KinThread=(CKinectThread*)AfxBeginThread(RUNTIME_CLASS(CKinectThread));
		m_KinThread->PostThreadMessage(WM_GETANGLE,NULL,NULL);
		Sleep(10);

		m_GetCamVideoThread = (CGetCamVideoThread*)AfxBeginThread(RUNTIME_CLASS(CGetCamVideoThread));
		m_GetCamVideoThread->PostThreadMessage(WM_GETCAMVIDEO,NULL,NULL);
		Sleep(10);

		m_fpgathread = (CFPGAThread*)AfxBeginThread(RUNTIME_CLASS(CFPGAThread));
		m_fpgathread->PostThreadMessage(WM_SENDFPGAANGLE,NULL,NULL);
		Sleep(10);


	}
	// TODO: Add your control notification handler code here
}


//////////////////////////////////////////////////////////////////////////////////////

LRESULT Cwhu_MFC_HandDlg::DisplayAngle(WPARAM wParam,LPARAM lParam)
{
	int time = (int)m_runtime;
	SetDlgItemInt(IDC_TIME,time);
	double *angle = (double*)wParam;
	//whu_angle = (double*)wParam;
	//static bool  first=true;
	//if (first)
	//{
		//for (int i=0;i<5;i++)
		//{
		//	angle[i]=90;
		//}
	//	first=false;
	//}
	//else{
		for (int i=0;i<5;i++)
		{
			whu_angle[i] = angle[i];
		}
		if(m_gStopArm){
			GetDlgItem(IDC_WARNING)->SetWindowText("手掌握合判定,仿生机械手抓取中......");
		}
		else{
			GetDlgItem(IDC_WARNING)->SetWindowText(" >>>>>>>>>>>>>>>>>>>>>>手势动作判定,仿生机械手移动中......");
		}
		
	//}
	 //memcpy(whu_angle,angle,40);

	 whu_ShowAngles(whu_angle);

	////memcpy(m_gFPGAdata,angle,40);//m_gFPGAdata
	//if (SystemMode==0)
	//{
	//	whu_ClearAngleShake(whu_angle,m_ShowAngle,1);//
	//	whu_ShowAngles(m_ShowAngle);
	//	//if (m_gKinectVideo==true&&m_gRecvVideo==false)
	//	//{
	//	//	whu_angle[5] = CLIENTNOSENDVIDEO;
	//	//} 
	//	//else if(m_gKinectVideo==false&&m_gRecvVideo==true)
	//	//{
	//	//	whu_angle[5] = CLIENTSENDVIDEO;
	//	//}
	//	//m_RtpSendAngleThread->PostThreadMessage(WM_SENDANGLE,(WPARAM)whu_angle,NULL); //不在这里发
	//	//Sleep(5);
	//}
	//else
	//{
	//	whu_ShowAngles(whu_angle);
	//}
	return 0;
}
bool Cwhu_MFC_HandDlg::whu_SendLEDData(double *angle)
{
	//IGpioObject* m_pIObj;
	//DWORD dwErr = GpioCreate(&m_pIObj,0);
	bool state=false;
	DWORD m_angle[6]={0};
	if(m_pIObj)
	{
		m_pIObj->SetLEDState(m_angle[0]);//同步	//
		for (int i=1;i<6;i++)
		{
			m_angle[i] = (DWORD)angle[i-1];
			m_pIObj->SetLEDState(m_angle[i]);
			Sleep(1);
		}
		state=true;
	}
	/*GpioClose(m_pIObj);*/
	return state;
}



///////////////////////////////////////////////////////////////////////////////////
LRESULT Cwhu_MFC_HandDlg::DisplayVideo(WPARAM wParam,LPARAM lParam)
{
	static bool first=true;
	if (first)
	{
		if (SystemMode==1||SystemMode==3)
		{
			SetDlgItemTextA(IDC_WARNING,"Please Hand Up Both Your Hands And Wait For Detection.....");
		}
		m_bVideo = true;
		first=false;
	}
	whu_SetScreen();
	IplImage* m_img = (IplImage*)wParam;
	DrawPicToHDC(m_img,IDC_PIC);
	return 0;
}
void Cwhu_MFC_HandDlg::DrawPicToHDC(IplImage *img, UINT ID )
{
	CDC *pDC = GetDlgItem(ID)->GetDC();
	HDC hDC= pDC->GetSafeHdc(); 
	CRect rect; 
	GetDlgItem(ID)->GetClientRect(&rect); 
	CvvImage cimg; 
	cimg.CopyOf(img); 
	cimg.DrawToHDC(hDC,&rect); 
	cimg.Destroy();
	ReleaseDC(pDC); 


	////CDC *pDC = GetDlgItem(ID)->GetDC(); 
	//HDC hDC= pDC->GetSafeHdc(); 
	//CRect rect; 
	//GetDlgItem(ID)->GetClientRect(&rect); 
	////CvvImage cimg; 
	//cimg.CopyOf(img); 
	//cimg.DrawToHDC(hDC,&rect); 
	////cimg.Destroy();
	////ReleaseDC(pDC); 
	////cimg.Destroy();


}


