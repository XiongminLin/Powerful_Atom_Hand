#pragma once
#include "StdAfx.h"
#include <stdio.h>
#include <winsock2.h>
#include <cv.h>
#include <highgui.h>
#include <iostream>
using namespace std;
#pragma comment(lib,"WS2_32.lib")

class whu_SockRecv
{
public:
	SOCKET s,s1;


public:
	int Initialize();
	int ReceiveAngle(double *m_Angle);
	int ReceiveVideo(IplImage* m_img);
	int Close();
};