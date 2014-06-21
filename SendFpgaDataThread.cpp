// SendFpgaDataThread.cpp : implementation file
//

#include "stdafx.h"
#include "whu_MFC_Hand.h"
#include "SendFpgaDataThread.h"


// CSendFpgaDataThread

IMPLEMENT_DYNAMIC(CSendFpgaDataThread, CWnd)

CSendFpgaDataThread::CSendFpgaDataThread()
{
	//int a=10;
	//SetDlgItemInt(IDC_TIME,a);
	DWORD dwErr = GpioCreate(&m_pIObj,0);
}

CSendFpgaDataThread::~CSendFpgaDataThread()
{
	GpioClose(m_pIObj);
}

bool CSendFpgaDataThread::whu_SendLEDData(double *angle)
{
	//IGpioObject* m_pIObj;
	//DWORD dwErr = GpioCreate(&m_pIObj,0);
	bool state=false;
	DWORD m_angle[6]={0};
	if(m_pIObj)
	{
		m_pIObj->SetLEDState(m_angle[0]);//Í¬²½	//
		for (int i=1;i<6;i++)
		{
			m_angle[i] = (DWORD)angle[i-1];
			m_pIObj->SetLEDState(m_angle[i]);
			Sleep(1);
		}
		state=true;
	}
	/*GpioClose(m_pIObj);*/
	return state;
}
void CSendFpgaDataThread::SendPfgaAngle(UINT wParam,LONG lParam)
{

}

BEGIN_MESSAGE_MAP(CSendFpgaDataThread, CWnd)
		ON_THREAD_MESSAGE(WM_SENDFPGAANGLE,SendPfgaAngle)
END_MESSAGE_MAP()



// CSendFpgaDataThread message handlers


