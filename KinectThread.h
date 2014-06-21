#pragma once
#include "whu_MyHand.h"
#define WM_GETANGLE WM_USER+1

// CKinectThread

class CKinectThread : public CWinThread
{
	DECLARE_DYNCREATE(CKinectThread)

protected:
	CKinectThread();           // protected constructor used by dynamic creation
	virtual ~CKinectThread();
protected:
	whu_MyHand m_KinHand;
	afx_msg void GetKinectAngle(UINT wParam,LONG lParam);
	double m_KinAngle[5];
	clock_t m_start;
	clock_t m_end;

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
};


