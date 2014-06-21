// RecvAngleSendPicThread.cpp : implementation file
//

#include "stdafx.h"
#include "whu_MFC_Hand.h"
#include "RecvAngleSendPicThread.h"
#include "whu_MFC_HandDlg.h"
#include "whu_Golobal.h"
// CRecvAngleSendPicThread

IMPLEMENT_DYNCREATE(CRecvAngleSendPicThread, CWinThread)

CRecvAngleSendPicThread::CRecvAngleSendPicThread()
{
	for (int i=0;i<6;i++)
	{
		m_RecvAngle[i] = 90;
	}
}

CRecvAngleSendPicThread::~CRecvAngleSendPicThread()
{
	cvReleaseImage(&m_SendImg);
	cvReleaseCapture(&pCapture);
}

BOOL CRecvAngleSendPicThread::InitInstance()
{
	// TODO:  perform and per-thread initialization here

	return TRUE;
}

int CRecvAngleSendPicThread::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here

	return CWinThread::ExitInstance();
}
void CRecvAngleSendPicThread::RecvAngle(UINT wParam,LONG lParam)
{
	
	CvSize m_Size;
	m_Size.width=400;
	m_Size.height=400;
	m_SendImg=cvCreateImage(m_Size,IPL_DEPTH_8U,3);
	m_RecvAngle[5] = CLIENTNOSENDVIDEO; //表示不需要给服务器发视频
	pCapture = cvCreateCameraCapture(-1);
	//cvNamedWindow("SendImg",WINDOW_AUTOSIZE);//
	clock_t m_start,m_end;
	m_InitDone = false;
	m_InitDone = m_rtp.Init();
	int i=0;
	MSG message;
	while (TRUE)
	{

		if(::PeekMessage(&message,NULL,0,0,PM_REMOVE))   
		{   
			::TranslateMessage(&message);   
			::DispatchMessage(&message); 
			//Sleep(1);
		}
		i++;
		m_start = clock();
		m_SendImg = cvQueryFrame(pCapture);
		if (m_gCamVideo)
		{
			::PostMessageW((HWND)(GetMainWnd()->GetSafeHwnd()),WM_DISPLAYVIDEO,(WPARAM)m_SendImg,NULL);
			Sleep(40);
		}

		if (m_InitDone)
		{
			m_rtp.RecvAngle(m_RecvAngle);
			::PostMessageW((HWND)(GetMainWnd()->GetSafeHwnd()),WM_DISPLAYANGLE,(WPARAM)&m_RecvAngle,NULL);
			Sleep(5);
			if (m_RecvAngle[5] == CLIENTSENDVIDEO) //校验
			{
				m_gCamVideo = false;
				m_rtp.SendPic(m_SendImg);
				Sleep(30);
			}
			else if(m_RecvAngle[5] == CLIENTNOSENDVIDEO)
			{
				m_gCamVideo = TRUE;
			}
			
		}
		m_end = clock();
		m_runtime = (double)(m_end - m_start);

	}


}
BEGIN_MESSAGE_MAP(CRecvAngleSendPicThread, CWinThread)
	ON_THREAD_MESSAGE(WM_RECVANGLE,RecvAngle)
END_MESSAGE_MAP()


// CRecvAngleSendPicThread message handlers
