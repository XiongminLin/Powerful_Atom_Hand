#ifdef WHU_GLOBALS

#define WHU_EXT

#else

#define WHU_EXT extern

#endif

//WHU_EXT bool m_LocalVideo ;
WHU_EXT bool m_FullScreen;
WHU_EXT bool m_NormalScreen;
WHU_EXT bool m_PauseControl;
WHU_EXT unsigned char m_GRemoteIpAddRess[4];
WHU_EXT int m_GRemotePort;
WHU_EXT int m_GLocalPort;
WHU_EXT double m_runtime;
WHU_EXT double m_gKinectAngle[6];
WHU_EXT bool m_gKinectVideo;  //������ģʽ ����ģʽ
WHU_EXT bool m_gCamVideo;  //�ͻ�ģʽ ����ģʽ
WHU_EXT bool m_gRecvVideo; //������ģʽ �ͻ�ģʽ  
WHU_EXT bool m_gStopArm;
//WHU_EXT bool m_RemoteVideo ;