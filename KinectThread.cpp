// KinectThread.cpp : implementation file
//

#include "stdafx.h"
#include "whu_MFC_Hand.h"
#include "KinectThread.h"
#include "whu_MFC_HandDlg.h"
#include "whu_Golobal.h"
// CKinectThread

IMPLEMENT_DYNCREATE(CKinectThread, CWinThread)

CKinectThread::CKinectThread()
{
}

CKinectThread::~CKinectThread()
{
}

BOOL CKinectThread::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	

	return TRUE;
}

int CKinectThread::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}
void CKinectThread::GetKinectAngle(UINT wParam,LONG lParam)
{
	

	CvSize m_size;
	m_size.height = 480;//zzz
	m_size.width = 640;//zzzzz
	IplImage* m_LocalImg=cvCreateImage(m_size,IPL_DEPTH_8U,3);

	m_KinHand.whu_Initialize();
	MSG message;
	int first=0;
	while (true)
	{
		if(::PeekMessage(&message,NULL,0,0,PM_REMOVE))   
		{   
			::TranslateMessage(&message);   
			::DispatchMessage(&message); 
			//Sleep(1);
		}
		
		m_start = clock();
		if(m_KinHand.whu_GetSkeleton())
		{
			m_KinHand.whu_GetGesture(); //m_gKinectAngle[5]代表需不需要远程视频
			if (!m_PauseControl)
			{
				
				if(m_gStopArm){
					m_KinHand.whu_GetFingerAngle(m_gKinectAngle[4]);
				}
				else{
					m_KinHand.whu_GetHandAngles(m_gKinectAngle);
				}
			}
			if (first<=3)
			{
				::PostMessageW((HWND)(GetMainWnd()->GetSafeHwnd()),WM_DISPLAYANGLE,(WPARAM)m_gKinectAngle,NULL);
				first++;
			}
			else{
				m_gKinectVideo =false;
			}
			//::PostMessageW((HWND)(GetMainWnd()->GetSafeHwnd()),WM_DISPLAYANGLE,(WPARAM)m_gKinectAngle,NULL); 
			//Sleep(30);
		}
		m_end = clock();
		m_runtime = (double)(m_end - m_start);
		if (m_gKinectVideo)
		{
			m_KinHand.whu_GetCurPic(m_LocalImg);
			::PostMessageW((HWND)(GetMainWnd()->GetSafeHwnd()),WM_DISPLAYVIDEO,(WPARAM)m_LocalImg,NULL);
			Sleep(100);
		}
	}

}


BEGIN_MESSAGE_MAP(CKinectThread, CWinThread)

	ON_THREAD_MESSAGE(WM_GETANGLE,GetKinectAngle)

END_MESSAGE_MAP()


// CKinectThread message handlers
