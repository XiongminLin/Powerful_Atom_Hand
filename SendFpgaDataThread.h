#pragma once
#include "whu_MFC_HandDlg.h"
#include "whu_Golobal.h"
#define  WM_SENDFPGAANGLE WM_USER+9
// CSendFpgaDataThread

class CSendFpgaDataThread : public CWnd
{
	DECLARE_DYNAMIC(CSendFpgaDataThread)

public:
	CSendFpgaDataThread();
	virtual ~CSendFpgaDataThread();
	bool whu_SendLEDData(double *angle);
	afx_msg void SendPfgaAngle(UINT wParam,LONG lParam);
	IGpioObject* m_pIObj;

protected:
	DECLARE_MESSAGE_MAP()
};


