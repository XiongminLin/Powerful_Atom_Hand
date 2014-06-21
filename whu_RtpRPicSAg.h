#include "stdafx.h"

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

class whu_RtpRPicSAg
{
public:
	whu_RtpRPicSAg();
	~whu_RtpRPicSAg();
	bool Init();
	bool RecvPic(IplImage* m_img);
	bool SendAngle(double *angle);
private:
	RTPSession session;
	int status;
	unsigned long timestamp1;
	unsigned char * RawData;
	unsigned char temp[30];

protected:

};