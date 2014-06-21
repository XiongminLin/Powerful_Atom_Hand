#include "StdAfx.h"
#include "whu_SockSend.h"
#include <iostream>
using namespace std;
void whu_SockSend::Initialize(char* ServerAdd,int Port)
{
	sockaddr_in addr;
	WSAData data;
	WORD wVersionRequested=MAKEWORD(2,0);
	::WSAStartup(wVersionRequested,&data);
	memset(&addr,0,sizeof(addr));
	addr.sin_family=AF_INET;
	addr.sin_port=htons(Port);//原来是80 发送端口与接收端口不一样
	addr.sin_addr.S_un.S_addr=inet_addr(ServerAdd);
	s=socket(AF_INET,SOCK_STREAM,0);
	printf("linking to %s ......\n",ServerAdd);
	::connect(s,(sockaddr*)&addr,sizeof(addr));
	printf("linking to %s success\n",ServerAdd);

}
void whu_SockSend::Transfer(char* Sendbuff,int Length)
{ 
	send(s,Sendbuff,Length,0); //待改
}
void whu_SockSend::Close()
{
	closesocket(s);
	::WSACleanup();
}
