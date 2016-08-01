// GGClientDlg.h : header file
//

#if !defined(AFX_GGCLIENTDLG_H__67B01D09_4C21_41AC_A48B_8B8B3C6F3972__INCLUDED_)
#define AFX_GGCLIENTDLG_H__67B01D09_4C21_41AC_A48B_8B8B3C6F3972__INCLUDED_

#include "GGClient.h"	// Added by ClassView
#include "MobileView.h"	// Added by ClassView
#include "TraceDlg.h"
#include "AnalyDialog.h"
#include "AccDlg.h"
#include "DibInfoDlg.h"
#include "MeminfoDlg.h"
#include "WindowView.h"
#include "flashbar.h"
#include "webdialog.h"
#include "datadlg.h"

//#include "CCEGLView.h"
//#include "CCDrawNode.h"

//#include "cocos2d.h"
//USING_NS_CC;

/////////////////////////////////////////////////////////////////////////////
// CGGClientDlg dialog
class CSetupDlg;
class CMobileView;



class CGGClientDlg : public CDialog
{
// Construction
	friend CSetupDlg;
	friend CMobileView;
public:
	XPoint m_mousePoint;
	void ShowDataProc(int nMax);
	void SetDataProc(int nProc);
	void HideDataProc();
public:
	void ShowWeb();
	static CGGClientDlg* instance;
	void UpdateFrames(int nFrames)
	{
		m_trace.m_info.SetFrames(nFrames);
	}
	void AddNetData(XU32 nBytes)
	{
		m_trace.m_info.AddNetData(nBytes);
	}
	void UpdateMemory(XU32 nMemory)
	{
		m_trace.m_info.SetMemory(nMemory/(1024*1024));
	}
	int m_nInfoTicks;
	void Trace(const XU16*txt)
	{
		m_trace.TraceText(txt);
	}
	void OpenFlash();
	void CloseFlash();
	CAnalyDialog* m_pAnaDlg;
	CMeminfoDlg* m_pMemInfoDlg;
	CDibInfoDlg* m_pDibInfoDlg;
	bool m_bInitOK;
	CGGClientDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CGGClientDlg)
	enum { IDD = IDD_GGCLIENT_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGGClientDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	WindowView *mpWinView;
	//CCEGLView* eglView;
	//CCDrawNode *drawNode;
public:
	void SetMode(int nMode);
	static BOOL CGGClientDlg::LoadConfig(BROWNSINFO&info,BOOL bDefault);
	XBOOL ContinueModal();
	static int SplitString(CString&strTxt,CStringArray&list,TCHAR c=' ');
	//static long Atol(LPCTSTR v,BYTE nDec=10);
	static BOOL GetKey(CString &strTxt, CString &strKey, CString &strValue,TCHAR c1='=',TCHAR c2='"');
	static BOOL LoadText(XPCTSTR strFile,XString8&strTxt,BOOL bFile);

	static XString8 m_strWorkPath;
	void PeekMessage();
	int GetLockMode(){return m_nLockMode;}
	void SetLockMode(int mode)
	{m_nLockMode = mode;}
protected:
	XU32 m_nTimer;
	void ShowFlashBar();
	void HideFlashBar();
	enum {ELK_NONE=0,ELK_ENUZOOM=1,ELK_ENUMOVE=2};
	int m_nLockMode,m_nDeltaX,m_nDeltaY,m_bMouseDown,m_nPt2X,m_nPt2Y;
	//int m_nPt2X,m_nPt2Y;
	CTraceDlg m_trace;
	CAccDlg m_acc;
	void CreateTraceWindow(CRect&rect)
	{
		m_trace.CreateTrace(rect);
	}
	void OnMouseDown(XPoint&pt);
	void OnMouseMove(XPoint&pt);
	void OnMouseUp(XPoint&pt);
	void CreateACC(int w);
	void PaintACC(XGraphics&g);
	CRect m_accRect;
	int	  m_nAccRotate,m_nDownRotate,m_nDownRotateZ;
	int	  m_nAccRotateZ;
	//int m_nWidth;
	void RotateArea(int W);
	int CreateMobile();
	void LoadImage(XImage&img,XString8&str);
	XString16 m_strTitle;
	
	void SetConfig();
	CMobileView m_view;
	CEdit m_edit;
	void SetRgn(int w,int h);
	BYTE m_bMove,m_bDown,m_nMode,m_bAcc,m_bAccZ;
	int m_nActiveID;
	CPoint m_downPoint;
	HCURSOR m_hCursor;
	BROWNSINFO m_info;
	XImage m_select;
	XGraphics m_draw;
	XU8	    m_nTick;
//	XImage m_mobile;

	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CGGClientDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	virtual void OnCancel();
	afx_msg void OnAppExit();
	afx_msg void OnAppAbout();
	afx_msg void OnAppConfig();
	virtual void OnOK();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnAppSource();
	afx_msg void OnUpdateAppSource(CCmdUI* pCmdUI);
	afx_msg void OnAppUrl();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnAppTree();
	afx_msg void OnAppDebug();
	afx_msg void OnEditPaste();
	afx_msg void OnAppMode();
	afx_msg void OnAppFindvar();
	afx_msg void OnAppAnaly();
	afx_msg void OnAppBinary();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnUpdateAppOutput(CCmdUI* pCmdUI);
	afx_msg void OnAppOutput();
	afx_msg void OnDibInfo();
	afx_msg void OnMemInfo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
public:
	afx_msg void OnAcc();
	afx_msg void OnUpdateAcc(CCmdUI *pCmdUI);
	CFlashBar m_flashBar;
	CWebDialog m_webShow;
	CDataDlg m_dataShow;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GGCLIENTDLG_H__67B01D09_4C21_41AC_A48B_8B8B3C6F3972__INCLUDED_)
