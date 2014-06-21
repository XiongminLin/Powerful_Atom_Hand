
// whu_MFC_Hand.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "whu_MFC_Hand.h"
#include "whu_MFC_HandDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cwhu_MFC_HandApp

BEGIN_MESSAGE_MAP(Cwhu_MFC_HandApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// Cwhu_MFC_HandApp construction

Cwhu_MFC_HandApp::Cwhu_MFC_HandApp()
{
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only Cwhu_MFC_HandApp object

Cwhu_MFC_HandApp theApp;


// Cwhu_MFC_HandApp initialization

BOOL Cwhu_MFC_HandApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();



	//skinppLoadSkin(_T("Beige.ssk"));
	skinppLoadSkin(_T("AquaOS.ssk")); //可以
	//skinppLoadSkin(_T("Anion.ssk"));
	//skinppLoadSkin(_T("DameK UltraBlue.ssk"));
	//skinppLoadSkin(_T("Royale.ssk"));
	//skinppLoadSkin(_T("bbq.ssk"));
	//skinppLoadSkin(_T("FauxS-TOON.ssk")); //可以
	//skinppLoadSkin(_T("MSN Messenger.ssk"));//可以
	//skinppLoadSkin(_T(""));

	AfxEnableControlContainer();

	// Create the shell manager, in case the dialog contains
	// any shell tree view or shell list view controls.
	CShellManager *pShellManager = new CShellManager;

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	Cwhu_MFC_HandDlg dlg;
	//dlg.GetDlgItem(IDC_ENGINE5)->SetWindowTextA("app");
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();


	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Delete the shell manager created above.
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

