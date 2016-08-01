#if !defined(AFX_DOM_H__BFC5E2B7_CEC5_48CE_9D33_9B08CFD9B0DF__INCLUDED_)
#define AFX_DOM_H__BFC5E2B7_CEC5_48CE_9D33_9B08CFD9B0DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dom.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDom dialog

class CDom : public CDialog
{
// Construction
public:
	CDom(XDomItem*p,CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDom)
	enum { IDD = IDD_DOMTREE };
	CTreeCtrl	m_tree;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDom)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CImageList m_images;
	HTREEITEM m_pRoot;
	HTREEITEM AddItem(HTREEITEM root,XDomItem*p);
	XDomItem* m_pDoc;

	// Generated message map functions
	//{{AFX_MSG(CDom)
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkTree(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DOM_H__BFC5E2B7_CEC5_48CE_9D33_9B08CFD9B0DF__INCLUDED_)
