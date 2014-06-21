#pragma once
//#include "whu_MFC_HandDlg.h"//
#include "whu_Golobal.h"
#include "ArCorelib.h"  //ƽ̨LED
#include <time.h>
#define  WM_SENDFPGAANGLE WM_USER+9

// CFPGAThread

class CFPGAThread : public CWinThread
{
	DECLARE_DYNCREATE(CFPGAThread)

protected:
	CFPGAThread();           // protected constructor used by dynamic creation
	virtual ~CFPGAThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	bool whu_SendLEDData(double *angle);
	afx_msg void SendPfgaAngle(UINT wParam,LONG lParam);
	void whu_ClearAngleShake(double *TempAngle,double *FinalAngle,int MaxChange);
	//IGpioObject* m_pIObj;
	double m_SendAngle[5];
protected:
	DECLARE_MESSAGE_MAP()
};


