// RecvPicSendAngle.cpp : implementation file
//

#include "stdafx.h"
#include "whu_MFC_Hand.h"
#include "RecvPicSendAngle.h"
#include "whu_MFC_HandDlg.h"
#include "whu_Golobal.h"
// CRecvPicSendAngle

IMPLEMENT_DYNCREATE(CRecvPicSendAngle, CWinThread)

CRecvPicSendAngle::CRecvPicSendAngle()
{
	//m_InitDone = false;
	////RtpInitData * m_RtpInitData = (RtpInitData*)wParam;
	//unsigned char sss[4]={192,168,16,3};
	//m_InitDone = m_rtp.Init(m_GRemoteIpAddRess,m_GRemotePort,m_GLocalPort);
	//CvSize m_size;
	//m_size.height = 400;
	//m_size.width = 400;
	//img = cvCreateImage(m_size,IPL_DEPTH_8U,3);
}

CRecvPicSendAngle::~CRecvPicSendAngle()
{
	cvReleaseImage(&img);
}

BOOL CRecvPicSendAngle::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	m_InitDone = false;
	m_InitDone = m_rtp.Init();
	CvSize m_size;
	m_size.height = 400;
	m_size.width = 400;
	img = cvCreateImage(m_size,IPL_DEPTH_8U,3);
	return TRUE;
}

int CRecvPicSendAngle::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

void  CRecvPicSendAngle::SendAngle(UINT wParam,LONG lParam)
{

	double* angel = (double*) wParam;
	while (true)  //&&m_PauseControl==false
	{
		if (m_PauseControl==false)
		{
			whu_ClearAngleShake(m_gKinectAngle,m_sendangle,1);
			::PostMessageW((HWND)(GetMainWnd()->GetSafeHwnd()),WM_DISPLAYANGLE,(WPARAM)m_sendangle,NULL);
			Sleep(10);
			m_sendangle[5] = m_gKinectAngle[5];
			m_rtp.SendAngle(m_sendangle); //直接发送kinect更新的角度
			if (m_gRecvVideo)
			{
				m_rtp.RecvPic(img);
				::PostMessageW((HWND)(GetMainWnd()->GetSafeHwnd()),WM_DISPLAYVIDEO,(WPARAM)img,NULL);
				Sleep(20);
			}
		}else{
				Sleep(50);
		}

	}
}
void CRecvPicSendAngle::whu_ClearAngleShake(double *TempAngle,double *FinalAngle,int MaxChange)
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
		if (TempAngle[0]-FinalAngle[0]>=m_value&&TempAngle[0]-FinalAngle[0]<=150)
		{
			FinalAngle[0]+=MaxChange;   //如果电脑运行很快，那么这个值就取小一点//

		} 
		else if(FinalAngle[0]-TempAngle[0]>=m_value&&FinalAngle[0]-TempAngle[0]<=150)
		{
			FinalAngle[0]-=MaxChange;

		}
		//【舵机2】
		if ((TempAngle[1]-FinalAngle[1]>=m_value&&TempAngle[1]-FinalAngle[1]<=150))
		{
			FinalAngle[1]+=MaxChange;
		} 
		else if((FinalAngle[1]-TempAngle[1]>=m_value&&FinalAngle[1]-TempAngle[1]<=150))
		{
			FinalAngle[1]-=MaxChange;
		}
		//【舵机3】
		if ((TempAngle[2]-FinalAngle[2]>=m_value&&TempAngle[2]-FinalAngle[2]<=150))
		{
			FinalAngle[2]+=MaxChange;
		} 
		else if((FinalAngle[2]-TempAngle[2]>=m_value&&FinalAngle[2]-TempAngle[2]<=150))
		{
			FinalAngle[2]-=MaxChange; 
		}
		//【舵机4 控制平衡舵机】
		if (TempAngle[3]-FinalAngle[3]>=m_value&&TempAngle[3]-FinalAngle[3]<=150)
		{
			FinalAngle[3]+=MaxChange;
		} 
		else if(FinalAngle[3]-TempAngle[3]>=m_value&&FinalAngle[3]-TempAngle[3]<=150)
		{
			FinalAngle[3]-=MaxChange;
		}
		////【舵机5 控制手掌闭合】
		if (TempAngle[4]-FinalAngle[4]>=m_value) //不可取=
		{
			FinalAngle[4]+=1;
		} 
		else if(FinalAngle[4]-TempAngle[4]>=m_value)
		{
			FinalAngle[4]-=1;
		}
		for (int j=0;j<4;j++)
		{
			if(FinalAngle[j]>=175) FinalAngle[j]=175;
			
		}
		if(FinalAngle[4]<=60) FinalAngle[4] = 60;
		if (FinalAngle[4]>=160) FinalAngle[4] = 160;
	}
	
}
BEGIN_MESSAGE_MAP(CRecvPicSendAngle, CWinThread)
	ON_THREAD_MESSAGE(WM_SENDANGLE,SendAngle)
END_MESSAGE_MAP()


// CRecvPicSendAngle message handlers
