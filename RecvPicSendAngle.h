#pragma once

#include "whu_RtpRPicSAg.h"
#define WM_SENDANGLE WM_USER+4
//#define WM_INITRTP WM_USER+5
// CRecvPicSendAngle

class CRecvPicSendAngle : public CWinThread
{
	DECLARE_DYNCREATE(CRecvPicSendAngle)

protected:
	CRecvPicSendAngle();           // protected constructor used by dynamic creation
	virtual ~CRecvPicSendAngle();
protected:
	afx_msg void SendAngle(UINT wParam,LONG lParam);
	void whu_ClearAngleShake(double *TempAngle,double *FinalAngle,int MaxChange);
	//afx_msg void InitRtp(UINT wParam,LONG lParam);
	bool m_WithVideo;
	whu_RtpRPicSAg m_rtp;
	double m_sendangle[5];
	bool m_InitDone;
	IplImage* img;
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
};


