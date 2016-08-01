// SWFEdit.h : main header file for the SWFEDIT application
//

#if !defined(AFX_SWFEDIT_H__E1E967C3_FD58_4112_94AF_74A1DABB8904__INCLUDED_)
#define AFX_SWFEDIT_H__E1E967C3_FD58_4112_94AF_74A1DABB8904__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSWFEditApp:
// See SWFEdit.cpp for the implementation of this class
//

class CSWFEditApp : public CWinApp
{
public:
	CSWFEditApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSWFEditApp)
	public:
	virtual XBOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSWFEditApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	int ExitInstance(void);
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SWFEDIT_H__E1E967C3_FD58_4112_94AF_74A1DABB8904__INCLUDED_)
