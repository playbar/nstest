#if !defined(AFX_ANALYDIALOG_H__03D35743_36D5_44AC_ADC9_BDD2B25CF07E__INCLUDED_)
#define AFX_ANALYDIALOG_H__03D35743_36D5_44AC_ADC9_BDD2B25CF07E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AnalyDialog.h : header file
//
#include "ListCtrlEx.h"
#include "GraphButton.h"

/////////////////////////////////////////////////////////////////////////////
// CAnalyDialog dialog
/*struct _CALLINFO
{
	XString8 strName;
	XU32	nCalls;
	XU32	nTotal;
	XU32	nPecent;
	XU32	nAvg;
};
typedef XVector<_CALLINFO*> XXCALLS;
class XSortCall:public XSort
{
	XXCALLS*m_pCalls;
	XU8 m_nType;
public:
	enum {SBNAME,SBCALLS,SBTOTAL,SBPECENT,SBAVG};
	XSortCall(XXCALLS*pCalls,XU8 nType)
	{
		m_pCalls=pCalls;
		m_nType=nType;
	}
	XU32 GetCount(){return m_pCalls->GetSize();}
	void Add(_CALLINFO*pInfo,XU8 mode)
	{
		int mi;
		switch(m_nType)
		{
		default:
		case SBNAME:
			 Index(pInfo->strName,mi,mode);break;
			 //return CompareString8((XPCTSTR)iSrt,(XPCTSTR)iDst,nArg);
			//return (char*)pInfo->strName;
		case SBCALLS:
			 Index((void*)pInfo->nCalls,mi,mode);
			 break;
			//return (void*)pInfo->nCalls;
		case SBTOTAL:
			 Index((void*)pInfo->nTotal,mi,mode);
			 break;
			 //return (void*)pInfo->nTotal;
		case SBPECENT:
			 Index((void*)pInfo->nPecent,mi,mode);
			 break;
			//return (void*)pInfo->nPecent;
		case SBAVG:
			 Index((void*)pInfo->nAvg,mi,mode);
			 break;
			 //return (XU32)iSrt-(XU32)iDst;	
		}
		m_pCalls->InsertAt(mi,pInfo);
	}
	XINT  Compare(void* iSrt,void* iDst,XU8 nArg)
	{
		switch(m_nType)
		{
		default:
		case SBNAME:
			 if(nArg==XSS_NORMAL)
				return CompareString8((XPCTSTR)iSrt,(XPCTSTR)iDst,XSS_NORMAL);
			 else
				return CompareString8((XPCTSTR)iDst,(XPCTSTR)iSrt,XSS_NORMAL);
			//return (char*)pInfo->strName;
		case SBCALLS:
			 
			//return (void*)pInfo->nCalls;
		case SBTOTAL:
			 //return (void*)pInfo->nTotal;
		case SBPECENT:
			//return (void*)pInfo->nPecent;
		case SBAVG:
			if(nArg==XSS_NORMAL)
				return CompareUInt((XU32)iDst,(XU32)iSrt);
			else
				return CompareUInt((XU32)iSrt,(XU32)iDst);
			//return (XU32)iSrt-(XU32)iDst; 
			//return (void*)pInfo->nAvg;
		}
	}


	void* GetData(XU32 iData)
	{
		_CALLINFO*pInfo=(*m_pCalls)[iData];
		switch(m_nType)
		{
		default:
		case SBNAME:
			return (char*)pInfo->strName;
		case SBCALLS:
			return (void*)pInfo->nCalls;
		case SBTOTAL:
			return (void*)pInfo->nTotal;
		case SBPECENT:
			return (void*)pInfo->nPecent;
		case SBAVG:
			return (void*)pInfo->nAvg;
		}
	}
};*/
class CAnaToolBar:public CToolBarCtrl
{
protected:
	int         m_nButtonCount;
//	TBBUTTON    *m_pTBButtons;
public:
	CAnaToolBar()
	{
//		m_pTBButtons=XNULL;
	}
	~CAnaToolBar()
	{
//		if(m_pTBButtons)
//			delete m_pTBButtons;
	}
	BOOL Create(DWORD nStyle,CWnd*pParent,DWORD nID);
};

class CMobileView;

class CAnalyDialog : public CDialog
{
// Construction
public:
//	XS64 m_nTotal;
	void CloseFlash();
	CAnalyDialog(CWnd* pParent = NULL);   // standard constructor
	~CAnalyDialog();
// Dialog Data
	//{{AFX_DATA(CAnalyDialog)
	enum { IDD = IDD_ANALYDLG };
	CGraphButton	m_graph;
	CTreeCtrl	m_tree;
	CListCtrlEx	m_list;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAnalyDialog)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
public:
	int m_nSeMode;
	CMobileView* m_pView;
	XSWFPlayer*m_pPlayer;
#ifdef _ANALY
	_XCALLENTRY*m_pRoot;
	_XCALLENTRY*m_pRootItem;
#endif
// Implementation
protected:
	bool Search(HTREEITEM item,CString&strTxt,bool bSelf,bool bChild=true,bool bNext=true,bool bParent=true);
	void AddTree(XLock&lock,XXObject*pObject,HTREEITEM item,int iStack=0);
	void AddTree(XLock*lock,void*pObject,HTREEITEM item);
	XBOOL ContinueModal();
	void UpdateCallData();
	void EnableItems();
	HTREEITEM AddTree(XLock&lock,SObject*pObject,HTREEITEM item);
	void UpdateTree();
	CAnaToolBar m_toolBar;
	XU8 m_nMode;
	CImageList m_images,m_objs;
	//void AddTree();
#ifdef _ANALY
	int UpdateData(_XCALLENTRY*pCall,int iItem);
	_XCALLENTRY* AddData(_XCALLENTRY*pCall,_XCALLENTRY*pParent);
	void MakeData();
#endif
	
	XS64 m_nTotalTime,m_nTotalCall;
	double m_dMulti;
//	XU8 m_nType,m_nSortType;
//	XXCALLS m_calls;
	

	// Generated message map functions
	//{{AFX_MSG(CAnalyDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnList2();
	afx_msg void OnCallgraph();
	afx_msg void OnFunction();
	afx_msg void OnObjects();
	afx_msg void OnStart();
	afx_msg void OnStop();
	afx_msg void OnMember();
	afx_msg void OnUpdate();
	afx_msg void OnClear();
	afx_msg void OnSelchangedObjtree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEnble();
	afx_msg void OnDisable();
	afx_msg void OnDisableAll();
	afx_msg void OnGraph();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSearch();
	afx_msg void OnFind();
	afx_msg void OnSaveBmp();
	afx_msg void OnSearchTxt();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ANALYDIALOG_H__03D35743_36D5_44AC_ADC9_BDD2B25CF07E__INCLUDED_)
