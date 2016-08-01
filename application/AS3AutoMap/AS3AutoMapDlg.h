// AS3AutoMapDlg.h : header file
//

#if !defined(AFX_AS3AUTOMAPDLG_H__1D1E4223_F907_40F0_8FD6_00D6B902018F__INCLUDED_)
#define AFX_AS3AUTOMAPDLG_H__1D1E4223_F907_40F0_8FD6_00D6B902018F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CAS3AutoMapDlg dialog

struct FunctionInfo
{
	CString strName;
	CString strID;
	FunctionInfo*pNext;
};
struct ClassInfo
{ 
	CString		strName;
	CString		strID;
	CStringArray strPackages;
	BOOL    bInstance;
	FunctionInfo*pInfo;
	FunctionInfo*pTail;
	ClassInfo*pNext;
};


class CAS3AutoMapDlg : public CDialog
{
// Construction
public:
	~CAS3AutoMapDlg();
	ClassInfo*m_pClassInfo,*m_pTail;
	CAS3AutoMapDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CAS3AutoMapDlg)
	enum { IDD = IDD_AS3AUTOMAP_DIALOG };
	CButton	m_okBut;
	CTreeCtrl	m_tree;
	CString	m_strAS3Header;
	CString	m_strOutputPath;
	CString	m_strFile;
	CString	m_strNamespace;
	CString	m_strPreHeader;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAS3AutoMapDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HTREEITEM AddClass(ClassInfo*pInfo);
	CImageList m_images;
	void InitTree();
	void ReleaseInfo();
	CString m_strName;
	BOOL ParseHeader(CString&strTxt);
	BOOL SaveFile(const char* file,CString&txt);
	BOOL LoadFile(const char* fileName,CString&txt);
	void Init(BOOL bUpdate);
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CAS3AutoMapDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnChangeOutputfile();
	afx_msg void OnSetas3();
	afx_msg void OnSetout();
	afx_msg void OnClickClasstree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangedClasstree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEnable();
	virtual void OnOK();
	afx_msg void OnAutotext();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AS3AUTOMAPDLG_H__1D1E4223_F907_40F0_8FD6_00D6B902018F__INCLUDED_)
