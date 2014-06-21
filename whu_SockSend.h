#pragma once
//·¢ËÍ¶Ë client
#include "stdafx.h"
#include <stdio.h>
#include <winsock2.h>
#pragma comment (lib,"WS2_32.lib")
class whu_SockSend
{
public:

	SOCKET s;

public :
	void Initialize(char* ServerAdd,int Port);
	void Transfer(char* Sendbuff,int Length);

	void Close();
};

