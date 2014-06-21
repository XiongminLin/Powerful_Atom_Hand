
// whu_MFC_HandDlg.h : header file
//

#pragma once
#include <winsock2.h> //这句要放到开头//
#include <cv.h>
#include <highgui.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <string.h>
#include <Windows.h>
#include <XnCppWrapper.h>
#include <fstream>
#include <time.h>
#include "whu_MyHand.h"
#include "KinectThread.h"         //线程1 捕获Kinect信息
#include "RecvPicSendAngle.h"    //线程2 rtp发送角度接受视频
#include "RecvAngleSendPicThread.h" //线程3 rtp接受角度发送视频
#include "GetCamVideoThread.h"  ///////线程4 获取本地摄像头视频//////
#include  "FPGAThread.h"
#include "CvvImage.h"
#include "whu_SockRecv.h"
#include "whu_SockSend.h"
#include "whu_SerialPort.h"
#include "ArCorelib.h"  //平台LED
#include "afxcmn.h"
#pragma comment (lib,"WS2_32.lib")
using namespace std;
using namespace cv;


#define WM_DISPLAYANGLE WM_USER+2 
#define WM_DISPLAYVIDEO WM_USER+3
#define  CLIENTNOSENDVIDEO 1700
#define  CLIENTSENDVIDEO 4600

struct KinectData{
	double angle[5];
	bool m_OtherInfo[3];
	//bool LeftWave;
	//bool FullScreen;
};
struct RtpInitData{
	unsigned char ip[4];
	int RemotePort;
	int LocalPort;
};

// Cwhu_MFC_HandDlg dialog
class Cwhu_MFC_HandDlg : public CDialogEx
{
// Construction
public:
	Cwhu_MFC_HandDlg(CWnd* pParent = NULL);	// standard constructor
	~Cwhu_MFC_HandDlg();
// Dialog Data
	enum { IDD = IDD_WHU_MFC_HAND_DIALOG };

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
	afx_msg void OnBnClickedOk();
	afx_msg void OnAcnStartAnimate1();
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnBnClickedBstart();
	afx_msg void OnBnClickedBstop();
	afx_msg void OnBnClickedBset();
	afx_msg void OnEnChangeEdit6();
private:	
		
		char whu_ip[20];
		bool whu_Run;
		IplImage *whu_RecvImg;
		IplImage *whu_Coverimg;
		clock_t m_start;
		clock_t m_end;
		//IGpioObject* m_pIObj;
		//CDC *pDC; 
		//HDC hDC; 
		double whu_angle[5];
		double m_ShowAngle[5];
		//bool whu_SendAngle(double* angle);
		//void whu_SendLEDData(double *angle);
		//void whu_SockSendAngle(double *Angle);
		//void whu_SeriPortSend(double *Angle);
		bool whu_SendModeInit();
		void whu_SetScreen();
		bool whu_GetNobodyAngle(double* angle);
		int SystemMode;
		bool m_bVideo;

		LRESULT DisplayAngle(WPARAM wParam,LPARAM lParam);  //收到信息时显示角度//
		LRESULT DisplayVideo(WPARAM wParam,LPARAM lParam);//收到信息时显示视频//
        CKinectThread* m_KinThread;
		CRecvPicSendAngle* m_RtpSendAngleThread;
		CRecvAngleSendPicThread* m_RtpRecvAngleThread;
		CGetCamVideoThread* m_GetCamVideoThread;
		CFPGAThread* m_fpgathread;

		bool whu_SendLEDData(double *angle);
		IGpioObject* m_pIObj;

		CvvImage cimg; 
		CDC *pDC;
		
		bool InitDone[3];
		bool BodyControl;
		WINDOWPLACEMENT m_OldWndPlacement;
		CRect m_FullScreenRect;
		CRect m_NormalScreenRect;
		CRect m_large;
		CRect m_small;

		void whu_ShowAllItem();
		void whu_HideAllItem();
		//bool m_WithLocVideo; // m_WithLocVideo=false 接收远程视频  m_WithLocVideo=true 接收本地视频
		
		//【显示本地摄像头】//
public:
		void whu_ClearAngleShake(double *TempAngle,double *FinalAngle,int MaxChange);
		void whu_ShowAngles(double* angle);
		void DrawPicToHDC(IplImage *img, UINT ID =IDC_PIC);
		void whu_ShowOpenNiError(int Message);
public:
		bool whu_Start();
protected:

public:
	afx_msg void OnBnClickedBsetok();
	afx_msg void OnBnClickedBsetcancle();
	afx_msg void OnBnClickedRchinese();
	afx_msg void OnBnClickedRenglish();
	int m_Language;
	//int m_SendMode;
	afx_msg void OnBnClickedLocal();
	afx_msg void OnBnClickedSerialport();
	afx_msg void OnBnClickedSocket();
	afx_msg void OnBnClickedPicstart();
	afx_msg void OnBnClickedSwitch();
	afx_msg void OnBnClickedFullscreen();
	afx_msg void OnStnDblclickPic();
	CIPAddressCtrl whu_IpAddress;
	afx_msg void OnBnClickedClientmode();
	afx_msg void OnBnClickedLocalmode();
	afx_msg void OnBnClickedNobody();
	afx_msg void OnBnClickedBody();
	CSliderCtrl m_Slider1;
	CSliderCtrl m_Slider2;
	CSliderCtrl m_Slider3;
	CSliderCtrl m_Slider4;
	CSliderCtrl m_Slider5;
};

//UINT MyHandThreadProc( LPVOID pParam );