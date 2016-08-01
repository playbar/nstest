#if !defined(AFX_DIRBROWNS_H__4FA4D035_6E74_4BDB_B930_25F2EC8CD2D8__INCLUDED_)
#define AFX_DIRBROWNS_H__4FA4D035_6E74_4BDB_B930_25F2EC8CD2D8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DirBrowns.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDirBrowns dialog
#include "DriverBox.h"
#include "DirTree.h"
/////////////////////////////////////////////////////////////
//目录浏览对话框,浏览不包含隐含属性的目录和子目录
//用户在本地系统定位一个路径
class CDirBrowns : public CDialog
{
// Construction
public:
	CDirBrowns(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDirBrowns)	

	CEdit		m_pathEdit;
	CDirTree	m_dirTree;
	CDriverBox	m_drvList;
	CString	m_pathName;//当前路径,可以通过指定该变量设置初始位置
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDirBrowns)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL CheckPath(CString&path);
	CString m_currentPath;

	// Generated message map functions
	//{{AFX_MSG(CDirBrowns)
	afx_msg void OnDblclkDirtree();//NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangedDirtree();//NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeDrvlist();
	virtual BOOL OnInitDialog();
	afx_msg void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIRBROWNS_H__4FA4D035_6E74_4BDB_B930_25F2EC8CD2D8__INCLUDED_)
