#include "StdAfx.h"
#include "whu_MyHand.h"
#include "whu_Golobal.h"
#define  OPENNI_CONTEXT_ERROR 0
#define  OPENNI_CHECK_KINECT 1
#define  OPENNI_GENERATOR_ERROR 2
#define  OPENNI_USERREGISTER_ERROR 3
#define  OPENNI_OK 4
#define  CLIENTNOSENDVIDEO 1700
#define  CLIENTSENDVIDEO 4600
ostream& operator<<(ostream &out,const XnPoint3D& rPoint)
{
	out<<"("<<rPoint.X<<","<<rPoint.Y<<","<<rPoint.Z<<")";
	return out;
}
void XN_CALLBACK_TYPE whu_NewUser(xn::UserGenerator &generator,
	XnUserID user,
	void* pCookie)
{
	cout<<"New User Identified:"<<user<<endl;
	//generator.GetSkeletonCap().RequestCalibration(user,true);//假如不需要做出投降姿势才开始的话，把此句解注释
	generator.GetPoseDetectionCap().StartPoseDetection("Psi", user);//做出[ 。]姿势以开始用户认证//
}

void XN_CALLBACK_TYPE whu_LostUser( xn::UserGenerator& generator, XnUserID user,void* pCookie )
{
	cout << "User " << user << " lost" << endl;
}

void XN_CALLBACK_TYPE whu_CalibrationStart( xn::SkeletonCapability& skeleton,XnUserID user,void* pCookie )
{
	cout << "Calibration start for user " <<  user << endl;
}

void XN_CALLBACK_TYPE  whu_CalibrationEnd(xn::SkeletonCapability &skeleton,XnUserID user,XnCalibrationStatus eStatus,void *pCookie)
{
	cout<<"user"<<user<<": calibrate ";
	if(eStatus == XN_CALIBRATION_STATUS_OK)
	{
		cout<<"success"<<endl;
		skeleton.StartTracking(user);
	}
	else
	{
		cout<<"failed"<<endl;
		//skeleton.RequestCalibration(user,true);//假如不需要做出投降姿势才开始的话，把此句解注释
		((xn::UserGenerator*)pCookie)->GetPoseDetectionCap().StartPoseDetection( "Psi", user );
	}	
}

void XN_CALLBACK_TYPE PoseDetected( xn::PoseDetectionCapability& poseDetection,const XnChar* strPose,XnUserID user,void* pCookie)
{
	cout << "Pose " << strPose << " detected for user " <<  user << endl;
	((xn::UserGenerator*)pCookie)->GetSkeletonCap().RequestCalibration( user, FALSE );
	poseDetection.StopPoseDetection( user );
}


whu_MyHand::whu_MyHand(void)
{
}

whu_MyHand::~whu_MyHand(void)
{
	whu_Context.StopGeneratingAll();
	whu_Context.Release();
	cvReleaseImage(&imgDepth16u);
	//cvReleaseImage(&imgRGB8u);
	cvReleaseImage(&depthShow);
	//cvReleaseImage(&imageShow);

}
//void whu_MyHand::whu_closehand()
//{
	//whu_Context.StopGeneratingAll();
	//whu_Context.Release();
	//cvReleaseImage(&imgDepth16u);
	//cvReleaseImage(&imgRGB8u);
	//cvReleaseImage(&depthShow);
	//cvReleaseImage(&imageShow);
//}
bool whu_MyHand::whu_CheckError(const char *Error,XnStatus eResult)
{
	if(eResult != XN_STATUS_OK)
	{
		cout<<Error<<":"<<xnGetStatusString(eResult)<<endl;
		return true;
	}
	else{
		cout<<Error<<":"<<"success"<<endl;
		return false;
	}
}

//【初始化】
//void whu_MyHand::whu_InitSkeleton(XnPoint3D& point)
//{
//	point.X = 100;
//	point.Y =100;
//	point.Z = 1000;
//}

int whu_MyHand::whu_Initialize()
{

	m_BCapSke=false;
	//【OpenNI初始化】
	whu_MapopMode.nXRes = XN_VGA_X_RES;
	whu_MapopMode.nYRes = XN_VGA_Y_RES;
	whu_MapopMode.nFPS = 30;

	//for(int a=0;a<5;a++)
	//{
	//	whu_Angle[a] = 90;
	//	whu_TemAngle[a] = 90;
	//}
	//whu_InitSkeleton(temp_Right_Hand);
	//whu_InitSkeleton(temp_Right_Shoulder);
	//whu_InitSkeleton(temp_Right_Elbow);
	//whu_InitSkeleton(whu_Right_Hand);
	//whu_InitSkeleton(whu_Right_Elbow);
	//whu_InitSkeleton(whu_Right_Shoulder);

	whu_eResult = whu_Context.Init();
	if (whu_CheckError("Context Initial ",whu_eResult))
	{
		whu_Context.Release();
		return OPENNI_CONTEXT_ERROR;
	}
	whu_eResult = whu_DepthGenerator.Create(whu_Context);
	if(whu_CheckError("Create DepthGenerator ",whu_eResult))
	{
		return OPENNI_CHECK_KINECT;
	}
	whu_eResult = whu_DepthGenerator.SetMapOutputMode(whu_MapopMode);

	whu_eResult = whu_ImageGenerator.Create(whu_Context);
	if (whu_CheckError("Create ImageGenerator ",whu_eResult))
	{
		return OPENNI_GENERATOR_ERROR;
	}

	whu_eResult = whu_ImageGenerator.SetMapOutputMode(whu_MapopMode);

	//whu_eResult = whu_HandGenerator.Create(whu_Context);
	//if (whu_CheckError("Create HandGenerator ",whu_eResult))
	//{
		//return OPENNI_GENERATOR_ERROR;
	//}


	//whu_eResult = whu_GestureGenerator.Create(whu_Context);
	//if (whu_CheckError("Create GestureGenerator ",whu_eResult))
	//{
	//	return OPENNI_GENERATOR_ERROR;
	//}


	whu_eResult=whu_UserGenerator.Create(whu_Context);
	if (whu_CheckError("Create UserGenerator ",whu_eResult))
	{
		return OPENNI_GENERATOR_ERROR;
	}
	whu_DepthMap = whu_DepthGenerator.GetDepthMap();
	//whu_DepthGenerator.GetMetaData(whu_DepthMD);
	//whu_ImageGenerator.GetMetaData(whu_ImageMD);

	XnCallbackHandle hUserCallBack;
	whu_eResult = whu_UserGenerator.RegisterUserCallbacks(whu_NewUser,whu_LostUser,NULL,hUserCallBack);
	whu_CheckError("whu_UserGenerator.RegisterUserCallbacks ",whu_eResult);
	whu_eResult = whu_UserGenerator.GetSkeletonCap().SetSkeletonProfile(XN_SKEL_PROFILE_ALL);
	whu_CheckError("whu_UserGenerator.GetSkeletonCap().SetSkeletonProfile ",whu_eResult);
	XnCallbackHandle mCallbackH;
	whu_eResult = whu_UserGenerator.GetSkeletonCap().RegisterToCalibrationStart( whu_CalibrationStart,&whu_UserGenerator, mCallbackH );    
	whu_eResult = whu_UserGenerator.GetSkeletonCap().RegisterToCalibrationComplete(whu_CalibrationEnd,&whu_UserGenerator,mCallbackH);
	whu_CheckError("whu_UserGenerator.GetSkeletonCap().RegisterToCalibrationComplete ",whu_eResult);
	XnCallbackHandle poseCBHandle;
	whu_eResult = whu_UserGenerator.GetPoseDetectionCap().RegisterToPoseDetected( PoseDetected,&whu_UserGenerator, poseCBHandle );
	whu_CheckError("whu_UserGenerator.GetPoseDetectionCap().RegisterToPoseDetected ",whu_eResult);
	//cout<<"Please Hand Up Your Hand after Ten Seconds ......"<<endl;
	whu_Context.StartGeneratingAll();//开始捕获//
	whu_Context.WaitAndUpdateAll();
	whu_DepthGenerator.GetMetaData(whu_DepthMD);
	whu_ImageGenerator.GetMetaData(whu_ImageMD);
	whu_DepthGenerator.GetAlternativeViewPointCap().SetViewPoint( whu_ImageGenerator );

	//【OpenCV初始化】

	storage = cvCreateMemStorage(0);
	contours = NULL;
	imgDepth16u=cvCreateImage(cvSize(640,480),IPL_DEPTH_16U,1);
	//imgRGB8u=cvCreateImage(cvSize(640,480),IPL_DEPTH_8U,3);
	depthShow=cvCreateImage(cvSize(640,480),IPL_DEPTH_8U,1);
	//imageShow=cvCreateImage(cvSize(640,480),IPL_DEPTH_8U,3);
	
	return OPENNI_OK;

}

//【获得骨骼坐标】
bool whu_MyHand::whu_SkeletonCapture()
{
	bool capture=false;
	whu_Context.WaitNoneUpdateAll();
	XnUInt16 nUsers = whu_UserGenerator.GetNumberOfUsers();
	if(nUsers > 0)
	{

		XnUserID* aUserID = new XnUserID[nUsers];
		whu_UserGenerator.GetUsers(aUserID,nUsers);
		if(nUsers>=1) nUsers=1;
		for(int i=0;i<nUsers;++i) //以后要改成只有一个用户。因为只有一个机械手臂, nUsers处替换为1 //
		{
			if(whu_UserGenerator.GetSkeletonCap().IsTracking(aUserID[i]))  //以后得修改aUserID[0]
			{
				XnSkeletonJointTransformation mJointTran;

				whu_UserGenerator.GetSkeletonCap().GetSkeletonJoint(aUserID[i],XN_SKEL_LEFT_HAND,mJointTran);//右手手腕//
				whu_Right_Hand = mJointTran.position.position;

				whu_UserGenerator.GetSkeletonCap().GetSkeletonJoint(aUserID[i],XN_SKEL_LEFT_ELBOW,mJointTran);//右手手肘//
				whu_Right_Elbow = mJointTran.position.position;

				whu_UserGenerator.GetSkeletonCap().GetSkeletonJoint(aUserID[i],XN_SKEL_LEFT_SHOULDER,mJointTran);//右手肩关节//
				whu_Right_Shoulder = mJointTran.position.position;

				whu_UserGenerator.GetSkeletonCap().GetSkeletonJoint(aUserID[i],XN_SKEL_RIGHT_HAND,mJointTran);//左手手腕//
				m_NewLeftHand = mJointTran.position.position;

				whu_UserGenerator.GetSkeletonCap().GetSkeletonJoint(aUserID[i],XN_SKEL_HEAD,mJointTran);//头部坐标//
				m_Head = mJointTran.position.position;

				whu_UserGenerator.GetSkeletonCap().GetSkeletonJoint(aUserID[i],XN_SKEL_TORSO,mJointTran);//中心坐标//
				m_Torso = mJointTran.position.position;

				capture = true;
			}
		}
		delete [] aUserID;
	}
	return capture;

}
bool whu_MyHand::whu_SkeletonShake(XnPoint3D point1,XnPoint3D point2,int m_MChange)
{
	//如果抖动，输出true//
	double a=point1.X-point2.X;
	double b=point1.Y-point2.Y;
	double c=point1.Z-point2.Z;
	double d=pow(pow(a,2.0)+pow(b,2.0)+pow(c,2),0.5);
	double dd=sqrt(a*a+b*b+c*c);
	if (d>m_MChange)
	{
		return true;
	} 
	else
	{
		return false;
	}
}
void whu_MyHand::whu_ClearSkeletonShake(XnPoint3D point1,XnPoint3D &point2,int m_MChange)
{
	XnPoint3D m_point;
	static bool m_first=true;
	if (m_first)
	{
		point2 = point1;
		m_first=false;
	}
	else if (whu_SkeletonShake(point1,point2,m_MChange)) //假如抖动过于厉害的话，就去掉这个点
	{
		m_point.X = 0.5*(point1.X+point2.X);
		m_point.Y = 0.5*(point1.Y+point2.Y);
		m_point.Z = 0.5*(point1.Z+point2.Z);
		if (whu_SkeletonShake(point1,m_point,m_MChange)==false)
		{
			point2.X = 0.5*(point1.X+m_point.X);
			point2.Y = 0.5*(point1.Y+m_point.Y);
			point2.Z = 0.5*(point1.Z+m_point.Z);
		}
		//cout<<"skeleton shake shake !"<<endl;
	}
	else{
		point2=point1;
	}

}



//【获得舵机角度】
int whu_MyHand::min_front(IplImage* img)
{
	int min_val=255, temp;
	for(int y = 0; y < img->height; y++)
	{
		uchar* ptr = (uchar*) (img->imageData+y*img->widthStep);
		for(int x = 0; x < img->width; x++)
		{
			temp=ptr[x];
			if(temp < min_val&&temp > 30)min_val = temp;
		}
	}	
	return min_val;
}

void whu_MyHand::get_gray_hand(IplImage* img, int min_val)
{
	for(int y = 0; y < img->height; y++)
	{
		uchar* ptr = (uchar*) (img->imageData+y*img->widthStep);
		for(int x = 0; x < img->width; x++)
		{

			if( ptr[x] > min_val+5 ){
				ptr[x] = 0;
			}

		}
	}
	cvSmooth(img,img,CV_MEDIAN,5,0,0);

	// cpimg = cvCreateImage(cvSize(640,480),img->depth, img->nChannels);
	// cvCopy(img,cpimg);
	cvThreshold(img,img,min_val-1,255,CV_THRESH_BINARY);  //二值化
	//// cvCanny(img, img, 1, 3, 5);
	cvFindContours( img, storage, &contours, sizeof(CvContour), CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0) );
}

void whu_MyHand::whu_GetFingerAngle(double &m_Angle)
{
	whu_Context.WaitNoneUpdateAll();
	//double m_Angle;
	int min_val;
	int FingerNum=0;//得到的指尖的个数//
	static int m_HandOpen=0;
	static int m_handClosed=0;
	memcpy(imgDepth16u->imageData,whu_DepthMD.Data(),640*480*2);
	cvConvertScale(imgDepth16u,depthShow,255/4096.0,0);      //转化为灰度图
	min_val = min_front(depthShow);    //取得灰度图最前端
	get_gray_hand(depthShow, min_val);

	CvRect rect = cvBoundingRect( contours, 0 );//返回一个2d矩形的点集合//得到包含轮廓的最小矩形
	for(int i = 20;i < contours->total;i++)
	{
		CvPoint *p = CV_GET_SEQ_ELEM(CvPoint,contours,i);
		CvPoint *a = CV_GET_SEQ_ELEM(CvPoint,contours,i-10);
		CvPoint *b = CV_GET_SEQ_ELEM(CvPoint,contours,i+10);
		if((double)((a->x-b->x)*(a->x-b->x)+(a->y-b->y)*(a->y-b->y))/(double)((a->x-p->x)*(a->x-p->x)+(a->y-p->y)*(a->y-p->y))<0.9)//&&cvGetReal2D(cpimg,(a->x+b->x)/2,(a->y+b->y)/2) >1)      //&&*(uchar*)(depthShow->imageData+((a->y+b->y)/2)*depthShow->widthStep)[(a->x+b->x)/2]==1
		{
			if((p->x-rect.x-(rect.width)/2)*(p->x-rect.x-(rect.width)/2)+(p->y-rect.y-(rect.height)/2)*(p->y-rect.y-(rect.height)/2) > rect.height*rect.height/10)//???
			{
				FingerNum++;
			}
		}
	}
	if (FingerNum>5)
	{
			if (m_Angle<150)
			{
				m_Angle=m_Angle+15;
			}
	} 
	else
	{
		if (m_Angle>60)
		{
			m_Angle=m_Angle-15;
		}
	}
	

}

void whu_MyHand::whu_GetHandAngles(double* TempAngle)
{
	//【舵机1】的转角，右手肩关节及腕关节//
	if(m_RHand.X > m_RShoulder.X){ 
		TempAngle[0] =90+whu_Arctan(abs(m_RHand.X - m_RShoulder.X) , abs(m_RHand.Z - m_RShoulder.Z));
	}
	else{
		TempAngle[0] =90-whu_Arctan(abs(m_RHand.X - m_RShoulder.X) , abs(m_RHand.Z - m_RShoulder.Z));
	}
	//【舵机2】的转角，右手肩关节及肘关节//算法1 根据角度
	double m_xz1=sqrt(pow(m_RElbow.X-m_RShoulder.X,2)+pow(m_RElbow.Z-m_RShoulder.Z,2));
	if(m_RElbow.Z < m_RShoulder.Z){
		TempAngle[1] = 60+whu_Arctan(m_xz1 , abs(m_RElbow.Y - m_RShoulder.Y));
	}
	else{
		TempAngle[1] = 60;
	}
	TempAngle[1]=180-TempAngle[1];
	//【舵机3】的转角//下面是算法一 //
	double m_xz2 = sqrt(pow(m_RElbow.X-m_RHand.X,2)+pow(m_RElbow.Z-m_RHand.Z,2));
	if(m_RHand.Z<m_RElbow.Z){
		TempAngle[2] = whu_Arctan(m_RHand.Y - m_RElbow.Y , m_xz2);//elbow.Y - hand.Y不加abs 这样得到的反tan值可以是负的//
	}
	else {
		TempAngle[2] = 90+whu_Arctan( m_xz2,abs(m_RElbow.Y - m_RHand.Y)); 
	}
	//【舵机4】平衡舵机 
	TempAngle[3]=90-TempAngle[2];  

	double CHanAnlge23 = 145-TempAngle[1]; //CHanAnlge23是2号舵机移动时对3号舵机的影响
	TempAngle[2]+=CHanAnlge23;

	for (int a=0;a<4;a++)
	{
		if (TempAngle[a]<=2) TempAngle[a]=2;
		if(TempAngle[a]>=178) TempAngle[a]=178;

	}
	//m_BCapSke=true;
}

bool whu_MyHand::whu_AngleShake(double m_NewAngle,double m_OldAngle,int m_MChange)
{
	if (abs(m_NewAngle-m_OldAngle)>m_MChange)
	{
		return true;
	} 
	else
		return false;//unchanged
}

double whu_MyHand::whu_Arctan(double Mole, double Deno)
{

	if(Deno!=0){
		return 180*atan2(Mole,Deno)/3.1415;
	}
	//注意返回值,目前先乘180，但是因为算舵机转动占空比的时候，还要除180，所以后面优化程序的时候可以把这里的180去掉
	else{
		cout<<"whu_OpenNI::whu_Arctan：denominator can not be zero"<<endl;
		return 0;
	}
}




//void whu_MyHand::whu_DisplayAngle(double *Angle)
//{
	//for(int i=0;i<5;i++)
	//{
	//	cout<<(int)Angle[i]<<"  ";
	//}
	//cout<<endl;
//}
void whu_MyHand::whu_GetCurPic(IplImage * m_img)
{
	whu_Context.WaitNoneUpdateAll();
	memcpy(m_img->imageData,whu_ImageMD.Data(),640*480*3);
	//把骨骼点画出来
	if (m_BCapSke)
	{
		//不需要这么多点！！！！//
		XnPoint3D m_PointIn[10];
		m_PointIn[0] = whu_Right_Hand;
		m_PointIn[1] = m_RHand;
		m_PointIn[2] = whu_Right_Elbow;
		m_PointIn[3] = m_RElbow;
		m_PointIn[4] = whu_Right_Shoulder;
		m_PointIn[5] = m_RShoulder;
		m_PointIn[6] = m_NewLeftHand;
		m_PointIn[7] = m_OldLeftHand;
		m_PointIn[8] = m_Torso;
		m_PointIn[9] = m_Head;
		
		XnPoint3D m_PointOut[10];
		whu_DepthGenerator.ConvertRealWorldToProjective(10,m_PointIn,m_PointOut);
		CvPoint m_Point2D[10];
		for(int i=0;i<6;i++)
		{
			m_Point2D[i].x = m_PointOut[i].X;
			m_Point2D[i].y = m_PointOut[i].Y;
			cvCircle(m_img,m_Point2D[i],3,CV_RGB(255,0,255),10);
		}
		cvLine(m_img,m_Point2D[0],m_Point2D[2],CV_RGB(0,255,255),5);
		cvLine(m_img,m_Point2D[2],m_Point2D[4],CV_RGB(0,255,255),5);
/*		cvLine(m_img,m_Point2D[0],m_Point2D[1],CV_RGB(200,0,0),5);
		cvLine(m_img,m_Point2D[2],m_Point2D[3],CV_RGB(200,0,0),5);
		cvLine(m_img,m_Point2D[4],m_Point2D[5],CV_RGB(200,0,0),5);
		cvLine(m_img,m_Point2D[6],m_Point2D[7],CV_RGB(200,0,0),5);*/		
	}
	
		
	
}
bool whu_MyHand::whu_GetSkeleton()
{
	
	m_BCapSke = whu_SkeletonCapture();
	if (m_BCapSke)
	{
		m_RHand = whu_Right_Hand;
		m_RElbow = whu_Right_Elbow;
		m_RShoulder = whu_Right_Shoulder;
		//whu_ClearSkeletonShake(whu_Right_Hand,m_RHand,5000);
		//whu_ClearSkeletonShake(whu_Right_Elbow,m_RElbow,5000);
		//whu_ClearSkeletonShake(whu_Right_Shoulder,m_RShoulder,5000);
	}
	return m_BCapSke;

}
bool whu_MyHand::whu_GetGesture()
{
	
	static bool first = true;
	if (first)
	{
		m_OldLeftHand = m_NewLeftHand;
		first = false;
	} 

	//左手左挥 控制视频切换//
	if (m_NewLeftHand.Y-m_Torso.Y > 50 &&
		m_NewLeftHand.X-m_Head.X > 450 &&
		m_OldLeftHand.X-m_Head.X <=450)
	{
		//whu_LeftWave = true;
		//if (m_LocalVideo){
		//	m_LocalVideo=false;
		//} 
		//else{
		//	m_LocalVideo=true;
		//}
		if (m_gKinectVideo)
		{
			m_gKinectVideo = false;
			m_gKinectAngle[5] = CLIENTSENDVIDEO;//
		} 
		else
		{
			m_gKinectVideo = true;
			m_gKinectAngle[5] = CLIENTNOSENDVIDEO;//
		}

		if (m_gCamVideo)
		{
			m_gCamVideo = false;
		} 
		else
		{
			m_gCamVideo = true;
		}

		if (m_gRecvVideo)
		{
			m_gRecvVideo = false;
		} 
		else
		{
			m_gRecvVideo = true;
		}


	} 
	//else
	//{
		//whu_LeftWave = false;
	//}
	//左手前推，机械手臂控制的开启与关闭//cancel
	if (m_NewLeftHand.Y-m_Torso.Y > 50 &&
		m_Head.Z-m_NewLeftHand.Z>450&&
		m_Head.Z-m_OldLeftHand.Z<=450)
	{
		//whu_LeftMove = true;
		if (m_PauseControl){
			//m_PauseControl = false;
			m_PauseControl = false;
		} 
		else{
			m_PauseControl = false;
		}
	} 


	//手掌判定信号//
	if (m_Head.Z-m_NewLeftHand.Z <=300&&m_NewLeftHand.Y-m_Head.Y>120&&m_OldLeftHand.Y-m_Head.Y<=120)
	{
		if (m_gStopArm){
			m_gStopArm=false;
		} 
		else{
			m_gStopArm=true;
		}

		/*if (m_FullScreen){
			m_FullScreen=false;
		}else{
			m_FullScreen = true;
		}*/

	} 
	////手掌判定信号//
	//if (m_Torso.Y - m_NewLeftHand.Y>50&&
	//	m_NewLeftHand.X-m_Torso.X>350&&
	//		m_OldLeftHand.X-m_Torso.X<=350){
	//	if (m_StopArm){
	//		m_StopArm=false;
	//	} 
	//	else{
	//		m_StopArm=true;
	//	}
	//}

	m_OldLeftHand = m_NewLeftHand;
	return true;

}
