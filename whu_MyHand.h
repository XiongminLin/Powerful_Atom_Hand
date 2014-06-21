#pragma once
#include "stdafx.h"
#include <winsock2.h> //这句要放到开头//
#include <cv.h>
#include <highgui.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <string.h>
#include <Windows.h>
#include <XnCppWrapper.h>
#include <fstream>
#include <time.h>
#include "ArCorelib.h"  //平台LED
#pragma comment (lib,"WS2_32.lib")
using namespace std;
using namespace cv;
class whu_MyHand
{
public:
	int whu_Initialize();
	bool whu_GetSkeleton();
	bool whu_GetGesture();
	//bool whu_LeftWave;
	//bool whu_LeftMove;
	//bool m_StopArm;
	void whu_GetFingerAngle(double &m_Angle);
	void whu_GetHandAngles(double* TempAngle);
	whu_MyHand(void);
	~whu_MyHand(void);
	//void whu_closehand();
	void whu_GetCurPic(IplImage * m_img);
	bool m_BCapSke;

private:
	//【OpenNI变量】
	XnStatus             whu_eResult;
	xn::DepthMetaData whu_DepthMD;
	xn::UserGenerator    whu_UserGenerator;
	XnPoint3D whu_Right_Hand,whu_Right_Elbow,whu_Right_Shoulder; //右手手腕,肘，肩//
	XnPoint3D m_RHand,m_RElbow,m_RShoulder;
	XnPoint3D m_OldLeftHand,m_NewLeftHand,m_Torso,m_Head;
	xn::ImageMetaData whu_ImageMD;
	xn::DepthGenerator   whu_DepthGenerator;
	//xn::HandsGenerator   whu_HandGenerator;
	//xn::GestureGenerator whu_GestureGenerator;
	xn::ImageGenerator   whu_ImageGenerator;
	const XnDepthPixel*  whu_DepthMap;
	XnMapOutputMode whu_MapopMode;
	
	xn::Context          whu_Context;
	//void whu_DisplayAngle(double *Angle);
	//【OpenCV变量】
	CvMemStorage* storage;
	CvSeq* contours;
	IplImage*  imgDepth16u;
	IplImage*  depthShow;
	//IplImage* imageShow;

	//IplImage* imgRGB8u;

private:
	//【OpenNI函数】
	bool whu_CheckError(const char *Error,XnStatus eResult);
	//void whu_InitSkeleton(XnPoint3D& point);
	bool whu_SkeletonShake(XnPoint3D point1,XnPoint3D point2,int m_MChange);
	void whu_ClearSkeletonShake(XnPoint3D point1,XnPoint3D &point2,int m_MChange);
	bool whu_AngleShake(double m_NewAngle,double m_OldAngle,int m_MChange);
	double whu_Arctan(double Mole, double Deno);
	bool whu_SkeletonCapture();



	//【OpenCV函数】
	int min_front(IplImage* img);
	void get_gray_hand(IplImage* img, int min_val);


	friend ostream& operator<<(ostream &out,const XnPoint3D& rPoint);
	friend void XN_CALLBACK_TYPE whu_NewUser(xn::UserGenerator &generator,XnUserID user,void* pCookie);
	friend void XN_CALLBACK_TYPE whu_LostUser( xn::UserGenerator& generator, XnUserID user,void* pCookie );
	friend void XN_CALLBACK_TYPE whu_CalibrationStart( xn::SkeletonCapability& skeleton,XnUserID user,void* pCookie );
	friend void XN_CALLBACK_TYPE  whu_CalibrationEnd(xn::SkeletonCapability &skeleton,XnUserID user,XnCalibrationStatus eStatus,void *pCookie);
	friend void XN_CALLBACK_TYPE PoseDetected( xn::PoseDetectionCapability& poseDetection,const XnChar* strPose,XnUserID user,void* pCookie);


};


