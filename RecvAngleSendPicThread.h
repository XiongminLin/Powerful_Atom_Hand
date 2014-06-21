#pragma once
#include "whu_RtpRAgSPic.h"
#define WM_RECVANGLE WM_USER+7

// CRecvAngleSendPicThread

class CRecvAngleSendPicThread : public CWinThread
{
	DECLARE_DYNCREATE(CRecvAngleSendPicThread)

protected:
	CRecvAngleSendPicThread();           // protected constructor used by dynamic creation
	virtual ~CRecvAngleSendPicThread();
	afx_msg void RecvAngle(UINT wParam,LONG lParam);
	whu_RtpRAgSPic m_rtp;
	IplImage* m_SendImg;
	CvCapture* pCapture;
	bool m_InitDone;
	double m_RecvAngle[6];
     
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
};


