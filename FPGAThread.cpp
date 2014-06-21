// FPGAThread.cpp : implementation file
//

#include "stdafx.h"
#include "whu_MFC_Hand.h"
#include "FPGAThread.h"
#include "whu_MFC_HandDlg.h"
#include "whu_Golobal.h"

// CFPGAThread

IMPLEMENT_DYNCREATE(CFPGAThread, CWinThread)

CFPGAThread::CFPGAThread()
{
	for (int i=0;i<5;i++)
	{
		m_SendAngle[i] = 90;
	}
	//DWORD dwErr = GpioCreate(&m_pIObj,0);
}

CFPGAThread::~CFPGAThread()
{
	//GpioClose(m_pIObj);
}

BOOL CFPGAThread::InitInstance()
{
	// TODO:  perform and per-thread initialization here

	return TRUE;
}

int CFPGAThread::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}


bool CFPGAThread::whu_SendLEDData(double *angle)
{
	IGpioObject* m_pIObj;
	DWORD dwErr = GpioCreate(&m_pIObj,0);
	bool state=false;
	DWORD m_angle[6]={0};
	if(m_pIObj)
	{
		m_pIObj->SetLEDState(m_angle[0]);//同步	//
		for (int i=1;i<6;i++)
		{
			m_angle[i] = (DWORD)angle[i-1];
			m_pIObj->SetLEDState(m_angle[i]);
			Sleep(1);
		}
		state=true;
	}
	GpioClose(m_pIObj);
	return state;
}
void CFPGAThread::whu_ClearAngleShake(double *TempAngle,double *FinalAngle,int MaxChange)
{
	int m_value = 10;
	static bool n_first=true;
	if(n_first){	          //第一次读取仅赋值不比较
		for(int i=0;i<5;i++){
			FinalAngle[i] = TempAngle[i];    	
		}
		n_first = false;
	}
	else    //第二次之后开始比较，满足要求后才赋值//
	{
		//【舵机1】//
		if (TempAngle[0]-FinalAngle[0]>=m_value&&TempAngle[0]-FinalAngle[0]<=150)
		{
			FinalAngle[0]+=MaxChange;   //如果电脑运行很快，那么这个值就取小一点//

		} 
		else if(FinalAngle[0]-TempAngle[0]>=m_value&&FinalAngle[0]-TempAngle[0]<=150)
		{
			FinalAngle[0]-=MaxChange;

		}
		//【舵机2】
		if ((TempAngle[1]-FinalAngle[1]>=m_value&&TempAngle[1]-FinalAngle[1]<=150))
		{
			FinalAngle[1]+=MaxChange;
		} 
		else if((FinalAngle[1]-TempAngle[1]>=m_value&&FinalAngle[1]-TempAngle[1]<=150))
		{
			FinalAngle[1]-=MaxChange;
		}
		//【舵机3】
		if ((TempAngle[2]-FinalAngle[2]>=m_value&&TempAngle[2]-FinalAngle[2]<=150))
		{
			FinalAngle[2]+=MaxChange;
		} 
		else if((FinalAngle[2]-TempAngle[2]>=m_value&&FinalAngle[2]-TempAngle[2]<=150))
		{
			FinalAngle[2]-=MaxChange; 
		}
		//【舵机4 控制平衡舵机】
		if (TempAngle[3]-FinalAngle[3]>=m_value&&TempAngle[3]-FinalAngle[3]<=150)
		{
			FinalAngle[3]+=MaxChange;
		} 
		else if(FinalAngle[3]-TempAngle[3]>=m_value&&FinalAngle[3]-TempAngle[3]<=150)
		{
			FinalAngle[3]-=MaxChange;
		}
		////【舵机5 控制手掌闭合】
		if (TempAngle[4]-FinalAngle[4]>=m_value&&TempAngle[4]>=40) //不可取=
		{
			FinalAngle[4]+=MaxChange;
		} 
		else if(FinalAngle[4]-TempAngle[4]>=m_value&&TempAngle[4]>=40)
		{
			FinalAngle[4]-=MaxChange;
		}
		for (int j=0;j<4;j++)
		{
			if(FinalAngle[j]>=175) FinalAngle[j]=175;
			if (FinalAngle[j]<5) FinalAngle[j]= 5;
		}
		if (FinalAngle[4]<=50)
		{
			FinalAngle[4] = 50;
		}
		if (FinalAngle[4]>=160)
		{
			FinalAngle[4] = 160;
		}
		
	}
}
void CFPGAThread::SendPfgaAngle(UINT wParam,LONG lParam)
{
	clock_t start;
	clock_t end;
	while (true)
	{
		start = clock();
		if (m_PauseControl == false)//往前推左手，开始传数据//
		{
			whu_ClearAngleShake(m_gKinectAngle,m_SendAngle,1);
			::PostMessageW((HWND)(GetMainWnd()->GetSafeHwnd()),WM_DISPLAYANGLE,(WPARAM)m_SendAngle,NULL);
			whu_SendLEDData(m_SendAngle);
			Sleep(10); ///这个值要测   平台上，前四个舵机数据的更新需要约250ms，第五个要400ms。//

		}
		else{
			Sleep(50);
		}
		end = clock();
		m_runtime = (double)(end-start); 
	}

}
BEGIN_MESSAGE_MAP(CFPGAThread, CWinThread)
	ON_THREAD_MESSAGE(WM_SENDFPGAANGLE,SendPfgaAngle)
END_MESSAGE_MAP()


// CFPGAThread message handlers
