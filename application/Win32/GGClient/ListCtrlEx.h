#if !defined(AFX_LISTCTRLEX_H__FD2BCDE0_45B3_4734_AC87_37F8FF2FDE86__INCLUDED_)
#define AFX_LISTCTRLEX_H__FD2BCDE0_45B3_4734_AC87_37F8FF2FDE86__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ListCtrlEx.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CListCtrlEx window

class CListCtrlEx : public CListCtrl
{
// Construction
public:
	CListCtrlEx();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListCtrlEx)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CListCtrlEx();

	// Generated message map functions
protected:
	void DrawItem(LPDRAWITEMSTRUCT);
	LPCTSTR MakeShortString(CDC *pDC, LPCTSTR lpszLong, int nColumnLen, int nOffset);
	int m_cxClient;

// Implementation - state icon width
	int m_cxStateImageOffset;
	COLORREF m_clrText;
	COLORREF m_clrTextBk;
	COLORREF m_clrBkgnd;
	BOOL m_bClientWidthSel,m_bFullRowSel;

	//{{AFX_MSG(CListCtrlEx)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTCTRLEX_H__FD2BCDE0_45B3_4734_AC87_37F8FF2FDE86__INCLUDED_)
