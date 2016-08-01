// SBMakeRes.h : main header file for the SBMAKERES application
//

#if !defined(AFX_SBMAKERES_H__2F5FF709_8A48_4FBB_8A18_38F1BBDC1819__INCLUDED_)
#define AFX_SBMAKERES_H__2F5FF709_8A48_4FBB_8A18_38F1BBDC1819__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSBMakeResApp:
// See SBMakeRes.cpp for the implementation of this class
//

class CSBMakeResApp : public CWinApp
{
public:
	CSBMakeResApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSBMakeResApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSBMakeResApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
//	CString m_strSource;
//	CString m_strDest;
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SBMAKERES_H__2F5FF709_8A48_4FBB_8A18_38F1BBDC1819__INCLUDED_)
