#include "stdafx.h"
#include "whu_RtpRAgSPic.h"
#include "whu_Golobal.h"
whu_RtpRAgSPic::whu_RtpRAgSPic()
{

}
whu_RtpRAgSPic::~whu_RtpRAgSPic()
{
#ifdef WIN32
	WSACleanup();
#endif // WIN32
}
bool whu_RtpRAgSPic::Init()
{

#ifdef WIN32
	WSADATA dat;
	WSAStartup(MAKEWORD(2,2),&dat);
#endif // WIN32

	RTPSessionParams sessionparams;
	sessionparams.SetOwnTimestampUnit(1.0/8000.0);

	RTPUDPv4TransmissionParams transparams;
	transparams.SetPortbase(m_GLocalPort);
	status = session.Create(sessionparams,&transparams);
	if (status < 0)
	{
		std::cerr << RTPGetErrorString(status) << std::endl;
		exit(-1);
	}

	//uint8_t localip[]={127,0,0,1};
	//uint8_t localip[]={192,168,16,10};
	RTPIPv4Address addr(m_GRemoteIpAddRess,m_GRemotePort);
	cout<<"linking success"<<endl;
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
bool whu_RtpRAgSPic::SendPic(IplImage* img)
{
	RTPTime delay(0.020);
	CvSize m_Size;
	m_Size.height = 400;
	m_Size.width = 400;
	char m_SendBuf[1204];
	static bool jishu = false;
	int line;
	if (jishu)
	{
		line = 1;
		jishu = false;
	} 
	else
	{
		line = 0;
		jishu = true;
	}
	
	for (;line<m_Size.height;line=line+2)
	{

		uchar* ptr=(uchar*)(img->imageData+line*img->widthStep);
		memcpy(m_SendBuf+4,ptr,1200);
		memcpy(m_SendBuf,&line,sizeof(int));
		status = session.SendPacket(m_SendBuf,1204);
	}
	if (status < 0)
	{
		std::cerr << RTPGetErrorString(status) << std::endl;
		exit(-1);
	}
		
		
		return true;
}
bool whu_RtpRAgSPic::RecvAngle(double *angle)
{
	bool done = false;
	int status;
	RTPTime delay(0.020);
	RTPTime starttime = RTPTime::CurrentTime();
	int lengh ,i;

	session.BeginDataAccess();
	if (session.GotoFirstSource())
	{
		do
		{
			RTPPacket *packet;

			while ((packet = session.GetNextPacket()) != 0)
			{
				/*std::cout << "Got packet with " 
					         << "extended sequence number " 
						    << packet->GetExtendedSequenceNumber() 
					        << " from SSRC " << packet->GetSSRC() 
					        << std::endl;*/
				timestamp1 = packet->GetTimestamp();
				lengh=packet->GetPayloadLength();
				RawData = packet->GetPayloadData();
				memcpy(angle,RawData,48);
				//for(i=0;i<lengh;i++){
					//temp[i]=RawData[i];
					//printf("%c",temp[i]);
				//}
				//temp[i]='\0';
				//printf("   timestamp: %d lengh=%d data:%s\n",timestamp1,lengh,&temp);
				//printf("%3.2f %3.2f",angle[1],angle[2]);
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