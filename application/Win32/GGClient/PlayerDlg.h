#if !defined(AFX_PLAYERDLG_H__DF9B8161_E0A9_49A5_8FC4_394C5856B02C__INCLUDED_)
#define AFX_PLAYERDLG_H__DF9B8161_E0A9_49A5_8FC4_394C5856B02C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PlayerDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPlayerDlg dialog

class CPlayerDlg : public CDialog
{
// Construction
protected:
	XSWFPlayer*m_pPlayer;
	int m_nType;
public:
	CPlayerDlg(XSWFPlayer*player,CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPlayerDlg)
	enum { IDD = IDD_PLAYERLIST };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPlayerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
// Implementation
protected:
	BOOL EnableItem();

	// Generated message map functions
	//{{AFX_MSG(CPlayerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeSource();
	afx_msg void OnSearch();
	afx_msg void OnReplace();
	afx_msg void OnSave();
	afx_msg void OnSaveCompress();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:

	CListCtrl m_list;
	int GetTextType(void);
	bool GetTextData(int id,XU8Array&data);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLAYERDLG_H__DF9B8161_E0A9_49A5_8FC4_394C5856B02C__INCLUDED_)
