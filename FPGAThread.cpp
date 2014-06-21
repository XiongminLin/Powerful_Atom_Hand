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
		m_pIObj->SetLEDState(m_angle[0]);//ͬ��	//
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
	if(n_first){	          //��һ�ζ�ȡ����ֵ���Ƚ�
		for(int i=0;i<5;i++){
			FinalAngle[i] = TempAngle[i];    	
		}
		n_first = false;
	}
	else    //�ڶ���֮��ʼ�Ƚϣ�����Ҫ���Ÿ�ֵ//
	{
		//�����1��//
		if (TempAngle[0]-FinalAngle[0]>=m_value&&TempAngle[0]-FinalAngle[0]<=150)
		{
			FinalAngle[0]+=MaxChange;   //����������кܿ죬��ô���ֵ��ȡСһ��//

		} 
		else if(FinalAngle[0]-TempAngle[0]>=m_value&&FinalAngle[0]-TempAngle[0]<=150)
		{
			FinalAngle[0]-=MaxChange;

		}
		//�����2��
		if ((TempAngle[1]-FinalAngle[1]>=m_value&&TempAngle[1]-FinalAngle[1]<=150))
		{
			FinalAngle[1]+=MaxChange;
		} 
		else if((FinalAngle[1]-TempAngle[1]>=m_value&&FinalAngle[1]-TempAngle[1]<=150))
		{
			FinalAngle[1]-=MaxChange;
		}
		//�����3��
		if ((TempAngle[2]-FinalAngle[2]>=m_value&&TempAngle[2]-FinalAngle[2]<=150))
		{
			FinalAngle[2]+=MaxChange;
		} 
		else if((FinalAngle[2]-TempAngle[2]>=m_value&&FinalAngle[2]-TempAngle[2]<=150))
		{
			FinalAngle[2]-=MaxChange; 
		}
		//�����4 ����ƽ������
		if (TempAngle[3]-FinalAngle[3]>=m_value&&TempAngle[3]-FinalAngle[3]<=150)
		{
			FinalAngle[3]+=MaxChange;
		} 
		else if(FinalAngle[3]-TempAngle[3]>=m_value&&FinalAngle[3]-TempAngle[3]<=150)
		{
			FinalAngle[3]-=MaxChange;
		}
		////�����5 �������Ʊպϡ�
		if (TempAngle[4]-FinalAngle[4]>=m_value&&TempAngle[4]>=40) //����ȡ=
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
		if (m_PauseControl == false)//��ǰ�����֣���ʼ������//
		{
			whu_ClearAngleShake(m_gKinectAngle,m_SendAngle,1);
			::PostMessageW((HWND)(GetMainWnd()->GetSafeHwnd()),WM_DISPLAYANGLE,(WPARAM)m_SendAngle,NULL);
			whu_SendLEDData(m_SendAngle);
			Sleep(10); ///���ֵҪ��   ƽ̨�ϣ�ǰ�ĸ�������ݵĸ�����ҪԼ250ms�������Ҫ400ms��//

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
