// GGClient.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "GGClient.h"
#include "GGClientDlg.h"
#include <math.h>
//#include "vld.h"
//#define _CRTDBG_MAP_ALLOC
//#include "stdlib.h"
//#include "crtdbg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGGClientApp

BEGIN_MESSAGE_MAP(CGGClientApp, CWinApp)
	//{{AFX_MSG_MAP(CGGClientApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGGClientApp construction

CGGClientApp::CGGClientApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CGGClientApp object

CGGClientApp theApp;

//struct TEST{XPCTSTR strKey;XU32 nID;};
//static TEST datas[]={{"ABC",1},{"111",2},{"222",3}};

/////////////////////////////////////////////////////////////////////////////
// CGGClientApp initialization
#include "flashglobal.h"
#include "XSystem.h"

BOOL CGGClientApp::InitInstance()
{
	AfxEnableControlContainer();
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

	XInitGGDom();	

	//float v1=-100.0F;
	//float v2=((v1/100*fixed_1)/fixed_1)*100;
	//if(v1==v2)
	//	AfxMessageBox(_T("OK"));
    /*double v=round(12.5);
	v=round(12.8);
	v=round(-12.3);
	v=round(-12.9);*/
//	void*p=new BYTE[8];
	//int sinv,cosv;
	//CalcRotate(v,sinv,cosv,XFALSE);

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	TCHAR szLongPathName[_MAX_PATH];
	::GetModuleFileName(AfxGetApp()->m_hInstance, szLongPathName, _MAX_PATH);

	//m_strAppFile=szLongPathName;
	CString strWorkSet = szLongPathName;
	int index = strWorkSet.ReverseFind('\\');
	if(index>0)
		strWorkSet=strWorkSet.Left(index);

	//XString16 s("10000");
	//XSystem::GetClipText(s);
	//XSystem::SetClipText(s);
	//float f=-0.01f;
	//int v=(int)(f>=0?f+0.5F:f-0.5F);
	/*XFastAlloc alloc(32,60);
	void* tmp[2000];
	for(int i=0;i<2000;i++)
	{
		tmp[i]=alloc.Alloc();
	}
	for(i=0;i<2000;i++)
	{
		alloc.Free(tmp[i]);
	}*/
//	char* p1=(char*)alloc.Alloc();
//	p1[0]='a';
//	p1[1]='b';
//	char* p2=(char*)alloc.Alloc();
//	p2[0]='c';
//	p2[1]='d';
	//XString8 str;
	//str.FromInt(0x84ae,XTRUE);
	//AfxMessageBox(CString(str.GetData()));
	//float v=1.0E10F;
	//int   s=2;

	//int dx=566,dy=1233;
	/*float v=2.4E13F;

	XU32 t0=GetTickCount();

	for(int i=0;i<10000000;i++)
	{
		//int len=dy+((XPaintDib::m_nSQRT[(dx<<10)/dy]*dy)>>16);
		float v2=v;
		RSHIFTF(v2,2);
		//(float)sx/FIX16ONE+
		//		(float)XPaintDib::m_nSQRT[(r0<<10)/sx]/65536
		//float v=XSWFMatrix::FixToFloat(s);
	}
	XU32 t1=GetTickCount();
	for(i=0;i<10000000;i++)
	{
		float v2=v;
		v2=v2/4;
		//int len=(int)sqrt(dx*dx+dy*dy);
		//float v=(float)s/FIX16ONE;
		//XU32 v=(XU32)(s*FIX16ONE);
	}
	XU32 t2=GetTickCount();
	CString str;
	str.Format(_T("Shift:%d,Multi:%d"),t1-t0,t2-t1);
	AfxMessageBox(str);*/

	//float v=1.23E10F;
	//RSHIFTF(v,1);
	//LSHIFTF(v,1);
	
	//float vi=XPaintDib::RShift(1.5E10F,2);
	/*XWCHAR str[]={169,169,169,0};
	XString8 str0(str);
	XString16 str1(str0);
	AfxMessageBox(str1);*/
	/*XEnumColor color;
	XU32 c=color.Color("White");*/

	CGGClientDlg dlg;
	dlg.m_strWorkPath=strWorkSet;
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

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

int CGGClientApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	//char*v =new char[200];
	
	XReleaseGGDom();	
	CoUninitialize();
	WindowView::Release();
	return CWinApp::ExitInstance();
	_CrtDumpMemoryLeaks();
}
