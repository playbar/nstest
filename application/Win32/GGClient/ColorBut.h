#if !defined(AFX_COLORBUT_H__B8F514D7_EC3A_4A30_84BB_D27DC4A112D6__INCLUDED_)
#define AFX_COLORBUT_H__B8F514D7_EC3A_4A30_84BB_D27DC4A112D6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColorBut.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CColorBut window

class CColorBut : public CButton
{
// Construction
public:
	CColorBut();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorBut)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	COLORREF m_nColor;
	virtual ~CColorBut();

	// Generated message map functions
protected:
	CToolTipCtrl m_toolTip;
	//{{AFX_MSG(CColorBut)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORBUT_H__B8F514D7_EC3A_4A30_84BB_D27DC4A112D6__INCLUDED_)
