#include "stdafx.h"

//  Í·ÎÄ¼þ
#include "rtpsession.h"
#include "rtpsessionparams.h"
#include "rtpudpv4transmitter.h"
#include "rtpipv4address.h"
#include "rtptimeutilities.h"
#include "rtppacket.h"
#include <stdlib.h>
#include <cxcore.h>
#include <cv.h>
#include <highgui.h>
#include <iostream>

using namespace jrtplib;
using namespace std;

class whu_RtpRAgSPic
{
private:
	RTPSession session;
	int status;
	unsigned long timestamp1;
	unsigned char * RawData;
	unsigned char temp[30];
public:
	bool SendPic(IplImage* img);
	//bool RecvPic(IplImage* m_img);
	bool RecvAngle(double *angle);
	//bool SendAngle(double *angle);
	bool Init();
	whu_RtpRAgSPic();
	~whu_RtpRAgSPic();
};