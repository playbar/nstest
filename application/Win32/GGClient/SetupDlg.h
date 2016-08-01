#if !defined(AFX_SETUPDLG_H__DF9B8161_E0A9_49A5_8FC4_394C5856B02C__INCLUDED_)
#define AFX_SETUPDLG_H__DF9B8161_E0A9_49A5_8FC4_394C5856B02C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetupDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetupDlg dialog
#include "ColorBut.h"
#include "IconBut.h"

class CSetupDlg : public CDialog
{
// Construction
public:
	XClientApply* m_pApply;
	DRAWCONTEXT* m_pDraw;
	void GetInfo(BROWNSINFO&info);
	void SetInfo(BROWNSINFO&info);
	CSetupDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSetupDlg)
	enum { IDD = IDD_SETUP };
	CColorBut	m_butColor;
	CComboBox	m_list;
	//CIconBut	m_delBut;
	//CIconBut	m_addBut;
	CSpinButtonCtrl	m_retrySpin;
	CSpinButtonCtrl	m_timeSpin;
	CColorBut	m_activeText;
	CColorBut	m_textColor;
	CColorBut	m_linkText;
	CColorBut	m_hiText;
	CColorBut	m_hiBack;
	CColorBut	m_bkColor;
	CString	m_strAccepts;
	CString	m_strGateway;
	CString m_strMobileType;
	CString m_strChannelID;
	CString m_strClientID;
	int		m_nProtocol;
	UINT	m_nPort;
	UINT	m_nTimeout;
	CString	m_strUserAgent;
	CString	m_strProfile;
	UINT	m_nRetries;
	BOOL	m_bEnu;
	BOOL	m_bEnuACC;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetupDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
// Implementation
protected:
	BOOL EnableItem();

	// Generated message map functions
	//{{AFX_MSG(CSetupDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDefault();
	afx_msg void OnText();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETUPDLG_H__DF9B8161_E0A9_49A5_8FC4_394C5856B02C__INCLUDED_)
