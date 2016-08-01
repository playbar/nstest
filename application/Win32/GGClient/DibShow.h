#pragma once
#include "afxwin.h"

class XDib;
class CDibShow : public CButton
{
// Construction
public:
	CDibShow();
	int m_nOffX;
	int m_nOffY;
	int m_nDownX,m_nDownY;
	int m_nDownMX,m_nDownMY;
	bool m_isMoving;
// Attributes
public:
	bool m_bScale;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDibShow)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDibShow();
	void SetDib(XDib*pDib)
	{
		m_pDib=pDib;
		m_nOffX = 0;
		m_nOffY = 0;
		if(this->GetSafeHwnd()!=NULL)
			Invalidate();
	}
	// Generated message map functions
protected:
	XDib* m_pDib;
	//{{AFX_MSG(CDibShow)
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};
