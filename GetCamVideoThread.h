#pragma once
//#include "whu_MFC_HandDlg.h"
#include <cv.h>
#include <highgui.h>
#define  WM_GETCAMVIDEO WM_USER+8

// CGetCamVideoThread

class CGetCamVideoThread : public CWinThread
{
	DECLARE_DYNCREATE(CGetCamVideoThread)

protected:
	CGetCamVideoThread();           // protected constructor used by dynamic creation
	virtual ~CGetCamVideoThread();
	afx_msg void GetCamVideo(UINT wParam,LONG lParam);
public:
	IplImage *m_CamImg;
	CvCapture *pCapture;

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
};


