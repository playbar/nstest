// SBMakeResDlg.h : header file
//

#if !defined(AFX_SBMAKERESDLG_H__971CC70B_B9C9_4B65_8759_47E0B8C95F47__INCLUDED_)
#define AFX_SBMAKERESDLG_H__971CC70B_B9C9_4B65_8759_47E0B8C95F47__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XMutex.h"
#include "FontSample.h"
/////////////////////////////////////////////////////////////////////////////
// CSBMakeResDlg dialog
class CSBMakeResDlg : public CDialog,XBase
{
// Construction
public:
	static CStringArray m_strFonts,m_strFonts2;
	static int CALLBACK EnumFontCallBack(LOGFONT FAR * font, TEXTMETRIC FAR *, DWORD Type, LPARAM);
	XU32 HashCode(char* buf);
	CSBMakeResDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CSBMakeResDlg)
	enum { IDD = IDD_SBMAKERES_DIALOG };
	CSpinButtonCtrl	m_spin;
	CComboBox	m_fontList2;
	VGFontSample	m_sample;
	CComboBox	m_fontList;
	CProgressCtrl	m_bar;
	CString	m_strSource;
	CString	m_strDest;
	CString m_strInc;
	UINT	m_nEnPecent;
	BOOL	m_bFullFont;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSBMakeResDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void SetFont();
	XU32 DoFonts();
	void SetPecent(int v);
	void Trace(XPCTSTR strTxt,...);
	XUINT CallBack(XU32 nID);
	XThread m_thread;
	XEvent	m_event;
	int m_nPecent,m_nTask;
	void EnableItem(BOOL bStart);
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSBMakeResDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBrowns();
	afx_msg void OnBrowns2();
	afx_msg void OnStart();
	afx_msg void OnStop();
	afx_msg void OnClear();
	afx_msg void OnSorttab();
	afx_msg void OnHash();
	afx_msg void OnLog();
	afx_msg void OnFonts();
	afx_msg void OnChinese();
	afx_msg void OnSelchangeFontlist();
	afx_msg void OnArray();
	afx_msg void OnSelchangeFontlist2();
	afx_msg void OnChangePecent();
	afx_msg void OnTest();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SBMAKERESDLG_H__971CC70B_B9C9_4B65_8759_47E0B8C95F47__INCLUDED_)
