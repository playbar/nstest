// SWFEditDlg.h : header file
//

#if !defined(AFX_SWFEDITDLG_H__8E39E012_1FEE_4FA6_BCCA_A9D503F4FE5F__INCLUDED_)
#define AFX_SWFEDITDLG_H__8E39E012_1FEE_4FA6_BCCA_A9D503F4FE5F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSWFEditDlg dialog
#include "SWFShow.h"

class XReplaceInfo
{
public:
	XReplaceInfo();
public:
	SCharacter* ch;
	SCharacter* replacech;
	CString strReplaceImage;
	int resWidth;
	int resHeight;
	int mType;
	XReplaceInfo* next;
};

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

class CSWFEditDlg : public CDialog
{
// Construction
public:
	void MakeImageShape(XU8Array&data,XDrawDib&dib,int tagID,int imageID,int offx,int offy);//shapeData,dib,info->ch->tag,nTagID);
	void MakeImageShape(XU8Array&data,XDrawDib&dib,int imageID, XReplaceInfo *info );
	void MakeShape(XU8Array&shapeData,int tag,int typeID,SRECT&rect,XStream&inData);
	int m_nMaxID;
	int MaxBits(int value);
	bool IsShape(SCharacter*ch);
	void PackageLossLess(XDrawDib&dib,XU8Array&data,int nTag);
	void PackageLossLess2(XDrawDib&dib,XU8Array&data,int nTag);
	void ShowPrev();
	void EnableItem();
	void ClearInfo()
	{
		while(m_pInfo)
		{
			XReplaceInfo*next=m_pInfo->next;
			delete m_pInfo;
			m_pInfo=next;
		}
	}
	void ClearReplaceInfo()
	{
		XReplaceInfo*info=m_pInfo;
		while(info)
		{
			info->replacech=NULL;
			info=info->next;
		}
	}
	BYTE* FindTag(BYTE*pData,BYTE*pEnd,int nTag,int&nLen,int&nTotal);
	XReplaceInfo* m_pInfo;
	CSWFEditDlg(CWnd* pParent = NULL);	// standard constructor
	~CSWFEditDlg();
// Dialog Data
	//{{AFX_DATA(CSWFEditDlg)
	enum { IDD = IDD_SWFEDIT_DIALOG };
	CTreeCtrl	m_tTree;
	CTreeCtrl	m_ReplaceTree;
	XSWFShow	m_rShow;
	XSWFShow	m_ReplaceShow;
	CString	m_strRFile;
	CString	m_strEFile;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSWFEditDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL
protected:
	XSWFPlayer* LoadSWF(CString&strFile);
	XSWFPlayer* m_pReplaceSWF;
	XSWFPlayer* m_pRSWF;
	CSize		m_showSize;
// Implementation
protected:
	void InitTreeRoot(CTreeCtrl&tree,XSWFPlayer*player );
	void InitSrcTreeRoot(CTreeCtrl&tree,XSWFPlayer*player );
	CImageList m_images,m_status;
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSWFEditDlg)
	virtual XBOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnEget();
	afx_msg void OnRget();
	afx_msg void OnItemexpandedEtree(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTvnSelchangedEtree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnSelchangedRtree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedReplace();
	afx_msg void OnBnClickedRestore();
	afx_msg void OnBnClickedPackage();
	afx_msg void OnBnClickedReplaceimage();
	afx_msg void OnBnClickedSave();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SWFEDITDLG_H__8E39E012_1FEE_4FA6_BCCA_A9D503F4FE5F__INCLUDED_)
