#if !defined(AFX_DOMDIALOG_H__5494198C_09E3_4A52_8DCE_728433B9C70C__INCLUDED_)
#define AFX_DOMDIALOG_H__5494198C_09E3_4A52_8DCE_728433B9C70C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DomDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDomDialog dialog

class CDomDialog : public CDialog
{
// Construction
public:
	virtual BOOL OnInitDialog();
	CDomDialog(XDomItem*p,CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDomDialog)
	enum { IDD = IDD_DOMTREE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDomDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	XDomItem* m_pDoc;

	// Generated message map functions
	//{{AFX_MSG(CDomDialog)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DOMDIALOG_H__5494198C_09E3_4A52_8DCE_728433B9C70C__INCLUDED_)
