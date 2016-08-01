// AS3AutoMap.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "AS3AutoMap.h"
#include "AS3AutoMapDlg.h"
#include "Register.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAS3AutoMapApp

BEGIN_MESSAGE_MAP(CAS3AutoMapApp, CWinApp)
	//{{AFX_MSG_MAP(CAS3AutoMapApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAS3AutoMapApp construction

CAS3AutoMapApp::CAS3AutoMapApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CAS3AutoMapApp object

CAS3AutoMapApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CAS3AutoMapApp initialization

BOOL CAS3AutoMapApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CAS3AutoMapDlg dlg;

	SetRegistryKey(_T("AS3 Native Auto Map"));

	CRegister reg("Datas");
	dlg.m_strAS3Header=reg.Read("AS3",dlg.m_strAS3Header);
	dlg.m_strFile=reg.Read("File",dlg.m_strFile);
	dlg.m_strOutputPath=reg.Read("Output",dlg.m_strOutputPath);
	dlg.m_strNamespace=reg.Read("Namespace",dlg.m_strNamespace);
	dlg.m_strPreHeader=reg.Read("Pre",dlg.m_strPreHeader);
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
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

	reg.Write("AS3",dlg.m_strAS3Header);
	reg.Write("File",dlg.m_strFile);
	reg.Write("Output",dlg.m_strOutputPath);
	reg.Write("Namespace",dlg.m_strNamespace);
	reg.Write("Pre",dlg.m_strPreHeader);

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

int CAS3AutoMapApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CWinApp::ExitInstance();
}
