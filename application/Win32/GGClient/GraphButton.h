#if !defined(AFX_GRAPHBUTTON_H__CF872538_48FB_4316_AD7D_841196D3677C__INCLUDED_)
#define AFX_GRAPHBUTTON_H__CF872538_48FB_4316_AD7D_841196D3677C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GraphButton.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGraphButton window

class CGraphButton : public CButton
{
// Construction
public:
	CGraphButton();
	double m_dMulti,m_fMaxTime;
	XU32  m_nCount,m_nMaxSamples,m_nHeight,m_nItemHeight;
	//XU64* m_pData,m_nMaxData;
	XU16  *m_pDatas;
	CStringArray m_strNames;

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGraphButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	XBOOL Update();
	virtual ~CGraphButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(CGraphButton)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRAPHBUTTON_H__CF872538_48FB_4316_AD7D_841196D3677C__INCLUDED_)
