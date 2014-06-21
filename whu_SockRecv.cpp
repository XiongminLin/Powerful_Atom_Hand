
#include "StdAfx.h"
#include "whu_SockRecv.h"

int whu_SockRecv::Initialize()
{	
	cout<<"socket initialize start....."<<endl;
	sockaddr_in addr,addr1;
	int n=sizeof(addr1);
	WSAData data;
	WORD wVersionRequested=MAKEWORD(2,0);
	::WSAStartup(wVersionRequested,&data);
	if((s=socket(AF_INET,SOCK_STREAM,0))==INVALID_SOCKET)
	{	
		printf("socket error");
		return 0;
	}

	memset(&addr,0,sizeof(addr));
	addr.sin_family=AF_INET;
	addr.sin_port=htons(80);
	addr.sin_addr.S_un.S_addr=INADDR_ANY;
	if(bind(s,(struct sockaddr*)&addr,sizeof(addr))==SOCKET_ERROR)
	{	
		printf("bind error \n");
		return 0;
	}
	else printf("bind success \n");
	if(listen(s,5)==SOCKET_ERROR)
	{	
		printf("listen error\n");
		return 0;
	}
	else printf("listen success\n");
	if((s1=accept(s,(struct sockaddr*)&addr1,&n))==INVALID_SOCKET)
	{
		printf("accept error\n");
		return 0;
	}
	else printf("accept success");
	cout<<"socket initialize success"<<endl;
	return 0;
}
int whu_SockRecv::ReceiveAngle(double *m_Angle)
{

	////unsigned char m_RecvBuf[7];
	//char RecvBuf[50];
	////double m_Angle[6]={90};
	////int angle[6]={90};
	//while(recv(s1,RecvBuf,50,0)>0)
	//{
	//	cout<<(int)RecvBuf[0]<<" ";
	//	memcpy(m_Angle,RecvBuf+1,48);
	//	//memcpy(m_RecvBuf,RecvBuf,7);
	//	for (int i=0;i<6;i++)
	//	{
	//		//angle[i-1]=(int)m_RecvBuf[i];
	//		cout<<m_Angle[i]<<" ";
	//	}
	//	cout<<endl;
	//	//printf("角度%s：",RecvBuf);
	//}
	//return 0;
	//下面是接收double型舵机角度的//
	char RecvBuf[50];
	double m_Tangle[7];
	int RecvLength=recv(s1,RecvBuf,50,0);
	memcpy(m_Angle,RecvBuf+1,48);
	//while(recv(s1,RecvBuf,50,0)>0)
	//{
	//	memcpy(m_Angle,RecvBuf+1,48);
	//	for (int i=0;i<6;i++)
	//	{
	//		//angle[i-1]=(int)m_RecvBuf[i];
	//		cout<<m_Angle[i]<<" ";
	//	}
	//	cout<<endl;
	//}
	return 0;

}
int whu_SockRecv::ReceiveVideo(IplImage* m_img)
{
	int recvbyte=0;//这个值，只是一帧图像的一行//
	//char m_ImgBuf[640][480];
	char recvbuf[1924];
	uchar* ptr;
	int line;
	for(int y=0;y<m_img->height;y++)
	{
		recvbyte=recv(s1,recvbuf,1924,0); //
		memcpy(&line,recvbuf,4);
		if (line>=0&&line<m_img->height)  
		{
			ptr=(uchar*)(m_img->imageData+y*m_img->widthStep);
			memcpy(ptr,recvbuf+4,1920); 
		}

	}
	////为什么这样会死机啊！！！
	//for(int y=0;y<m_img->height;y++)
	//{
	//	recvbyte=recv(s1,recvbuf,1924,0); //
	//	memcpy(&line,recvbuf,4);
	//	if (line>=0&&line<m_img->height)  
	//	{
	//		ptr=(uchar*)(m_img->imageData+y*m_img->widthStep);
	//	}
	//	memcpy(ptr,recvbuf+4,1920); //为什么这样会死机啊！！！
	//}
	return recvbyte;
}
int whu_SockRecv::Close()
{

	::closesocket(s);
	::closesocket(s1);
	::WSACleanup();
	return 0;
}
