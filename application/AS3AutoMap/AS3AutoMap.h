// AS3AutoMap.h : main header file for the AS3AUTOMAP application
//

#if !defined(AFX_AS3AUTOMAP_H__7A6B7F9E_B48C_4CDC_9CCC_5F6B868993D2__INCLUDED_)
#define AFX_AS3AUTOMAP_H__7A6B7F9E_B48C_4CDC_9CCC_5F6B868993D2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CAS3AutoMapApp:
// See AS3AutoMap.cpp for the implementation of this class
//

class CAS3AutoMapApp : public CWinApp
{
public:
	CAS3AutoMapApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAS3AutoMapApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CAS3AutoMapApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AS3AUTOMAP_H__7A6B7F9E_B48C_4CDC_9CCC_5F6B868993D2__INCLUDED_)
