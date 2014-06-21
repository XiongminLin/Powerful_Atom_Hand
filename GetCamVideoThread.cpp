// GetCamVideoThread.cpp : implementation file
//

#include "stdafx.h"
#include "whu_MFC_Hand.h"
#include "GetCamVideoThread.h"
#include "whu_Golobal.h"
#include "whu_MFC_HandDlg.h"
// CGetCamVideoThread

IMPLEMENT_DYNCREATE(CGetCamVideoThread, CWinThread)

CGetCamVideoThread::CGetCamVideoThread()
{
}

CGetCamVideoThread::~CGetCamVideoThread()
{
	cvReleaseCapture(&pCapture);
	cvReleaseImage(&m_CamImg);
}

BOOL CGetCamVideoThread::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return TRUE;
}

int CGetCamVideoThread::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}
void CGetCamVideoThread::GetCamVideo(UINT wParam,LONG lParam)
{
	CvSize m_Size;
	m_Size.width=400;
	m_Size.height=400;
	m_CamImg = cvCreateImage(m_Size,IPL_DEPTH_8U,3);
	pCapture = cvCreateCameraCapture(-1);
	while (true)
	{
		if (m_gCamVideo)//这个这个
		{
			m_CamImg = cvQueryFrame(pCapture);
			::PostMessageW((HWND)(GetMainWnd()->GetSafeHwnd()),WM_DISPLAYVIDEO,(WPARAM)m_CamImg,NULL);
			Sleep(100);
		}else
		{
			Sleep(100);
		}
	}


}

BEGIN_MESSAGE_MAP(CGetCamVideoThread, CWinThread)
		ON_THREAD_MESSAGE(WM_GETCAMVIDEO,GetCamVideo)
END_MESSAGE_MAP()


// CGetCamVideoThread message handlers
