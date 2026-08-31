
// NhanVienManager.cpp : Defines the class behaviors for the application.
//

#include "pch.h"
#include "framework.h"
#include "NhanVienManager.h"
#include "NhanVienManagerDlg.h"
#include "CDlgLogin.h"
#include "CDlgConnectDB.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CNhanVienManagerApp

BEGIN_MESSAGE_MAP(CNhanVienManagerApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CNhanVienManagerApp construction

CNhanVienManagerApp::CNhanVienManagerApp()
{
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CNhanVienManagerApp object

CNhanVienManagerApp theApp;


// CNhanVienManagerApp initialization

BOOL CNhanVienManagerApp::InitInstance()
{
    CWinApp::InitInstance();

    CDlgLogin dlgLogin;
    if (dlgLogin.DoModal() != IDOK) return FALSE;

    CDlgConnectDB dlgConnectDB;
    if (dlgConnectDB.DoModal() != IDOK) return FALSE;

    CNhanVienManagerDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();
    return FALSE;
}

