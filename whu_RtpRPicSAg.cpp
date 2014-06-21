#include "stdafx.h"
#include "whu_RtpRPicSAg.h"
#include "whu_Golobal.h"
whu_RtpRPicSAg::~whu_RtpRPicSAg()
{
#ifdef WIN32
	WSACleanup();
#endif // WIN32
}
whu_RtpRPicSAg::whu_RtpRPicSAg()
{

}
bool whu_RtpRPicSAg::Init()
{
	#ifdef WIN32
		WSADATA dat;
		WSAStartup(MAKEWORD(2,2),&dat);
	#endif // WIN32
	RTPSessionParams sessionparams;
	sessionparams.SetOwnTimestampUnit(1.0/8000.0);

	RTPUDPv4TransmissionParams transparams;
	transparams.SetPortbase(m_GLocalPort);

	int status = session.Create(sessionparams,&transparams);
	if (status < 0)
	{
		std::cerr << RTPGetErrorString(status) << std::endl;
		exit(-1);
	}
	//uint8_t localip[]={127,0,0,1};
	//uint8_t localip[]={192,168,16,3};//Ì¨Ê½»úIP//
	//unsigned char sssss[4]={192,168,16,3};
	//uint8_t localip[4];
	//memcpy(localip,m_GRemoteIpAddRess,4);
	RTPIPv4Address addr(m_GRemoteIpAddRess,m_GRemotePort);
	status = session.AddDestination(addr);
	if (status < 0)
	{
		std::cerr << RTPGetErrorString(status) << std::endl;
		exit(-1);
	}

	session.SetDefaultPayloadType(96);
	session.SetDefaultMark(false);
	session.SetDefaultTimestampIncrement(160);
	return true;
}
bool whu_RtpRPicSAg::RecvPic(IplImage* RecvImg)
{
	bool done = false;
	int status;
	char RecvBuf[1204];
	RTPTime delay(0.020);
	RTPTime starttime = RTPTime::CurrentTime();
	int lengh ,i;
	uchar* ptr;
	session.BeginDataAccess();
		if (session.GotoFirstSource())
		{
			int line=0;
			do
			{
				RTPPacket *packet;
				
				while ((packet = session.GetNextPacket()) != 0)
				{
					timestamp1 = packet->GetTimestamp();
					lengh=packet->GetPayloadLength();
					RawData = packet->GetPayloadData();
					memcpy(RecvBuf,RawData,1204);
					memcpy(&line,RecvBuf,4);
					if (line>=0&&line<RecvImg->height)
					{
						
						ptr=(uchar*)(RecvImg->imageData+line*RecvImg->widthStep);
						memcpy(ptr,RecvBuf+4,1200);
					}
					else{
						printf("loss packet\n");
					}
					
					session.DeletePacket(packet);
				}
			} while (session.GotoNextSource());
		}
		session.EndDataAccess();

		RTPTime::Wait(delay);

		RTPTime t = RTPTime::CurrentTime();
		t -= starttime;
		if (t > RTPTime(60.0))
			done = true;
		return true;
}
bool whu_RtpRPicSAg::SendAngle(double *angle)
{
	char m_SendBuf[48];
	RTPTime delay(0.020); //??
	memcpy(m_SendBuf,angle,48);
	status = session.SendPacket(m_SendBuf,48);
	if (status < 0){
		std::cerr << RTPGetErrorString(status) << std::endl;
		exit(-1);
	}
	return true;
}