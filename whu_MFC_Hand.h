
// whu_MFC_Hand.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// Cwhu_MFC_HandApp:
// See whu_MFC_Hand.cpp for the implementation of this class
//

class Cwhu_MFC_HandApp : public CWinApp
{
public:
	Cwhu_MFC_HandApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern Cwhu_MFC_HandApp theApp;