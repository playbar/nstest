// SBMakeRes.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include <math.h>
#include "SBMakeRes.h"
#include "SBMakeResDlg.h"
#include "Register.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSBMakeResApp

BEGIN_MESSAGE_MAP(CSBMakeResApp, CWinApp)
	//{{AFX_MSG_MAP(CSBMakeResApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSBMakeResApp construction

CSBMakeResApp::CSBMakeResApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CSBMakeResApp object

CSBMakeResApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CSBMakeResApp initialization

BOOL CSBMakeResApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.
	/*{
		CFile file;
		file.Open("D:\\er.swf",CFile::modeRead|CFile::typeBinary);
		BYTE*pSrt=new BYTE[13406];
		file.Read(pSrt,13406);
		file.Close();
		BYTE*pDst=new BYTE[13406];
		file.Open("D:\\er1.swf",CFile::modeRead|CFile::typeBinary);
		file.Read(pDst,13406);
		file.Close();
		int n=memcmp(pSrt,pDst,13406);
		if(n)
		{
		}
	} */

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	//int v1=(int)(sqrt(2)*65536+0.5);
	//int v=(int)(65536/sqrt(2)+0.5);
	int v=(int)(65536*sin(3.1415926/8)+0.5);

	CSBMakeResDlg dlg;

	TCHAR szLongPathName[_MAX_PATH];
	::GetModuleFileName(AfxGetApp()->m_hInstance, szLongPathName, _MAX_PATH);
	//m_strAppFile=szLongPathName;
	CString strWorkSet = szLongPathName;
	int index = 0;//strWorkSet.ReverseFind('\\');
	int i=0,nc=strWorkSet.GetLength()-1,ncc=0;
	for(i=nc-1;i>0;i--)
	{
		if(strWorkSet[i]=='\\')
		{
			ncc++;
			if(ncc>=4) {index=i;break;}
		}
		
	}
	if(index>0)
		strWorkSet=strWorkSet.Left(index+1);
	dlg.m_strSource=strWorkSet;
	dlg.m_strSource+="Resource\\GGClient.srs";
	dlg.m_strDest=strWorkSet;
	dlg.m_strDest+="Resource\\Include";
	dlg.m_strInc=strWorkSet;
	dlg.m_strInc="Resource\\Include";

	SetRegistryKey(_T("Symbian Resource Make"));
	CRegister reg("Path");
	dlg.m_strSource=reg.Read("Source",dlg.m_strSource);
	dlg.m_strDest=reg.Read("Dest",dlg.m_strDest);
	dlg.m_strInc=reg.Read("Include",dlg.m_strInc);

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

	reg.Write("Source",dlg.m_strSource);
	reg.Write("Dest",dlg.m_strDest);
	reg.Write("Inc",dlg.m_strInc);


	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

int CSBMakeResApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CWinApp::ExitInstance();
}
