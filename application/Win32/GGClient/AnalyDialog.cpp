// AnalyDialog.cpp : implementation file
//

#include "stdafx.h"
#include "GGClient.h"
#include "AnalyDialog.h"
#include "MobileView.h"
#include "XFlashView.h"
#include "ObjectCreator.h"
#include "XFilter.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAnalyDialog dialog


#ifdef _ANALY
static void _FreeData(_XCALLENTRY*p)
{	
	if(!p) return;
	while(p)
	{
		if(p->pChild)
		{
			_FreeData(p->pChild);
		}
		_XCALLENTRY*pn=p->pNext;		
		delete p;
		p=pn;
	}
}
#endif



CAnalyDialog::CAnalyDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CAnalyDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAnalyDialog)
	//}}AFX_DATA_INIT
#ifdef _ANALY	
	m_pRoot=XNULL;
	m_pRootItem=XNULL;
#endif
	m_nMode=0;
	m_nSeMode=0;
	m_pPlayer=XNULL;
	m_dMulti=1.0;
	
}

CAnalyDialog::~CAnalyDialog()
{
#ifdef _ANALY
	if(m_pRoot)
		_FreeData(m_pRoot);
	if(m_pRootItem)
		_FreeData(m_pRootItem);
#endif
	
}

void CAnalyDialog::CloseFlash()
{
#ifdef _ANALY
	_FreeData(m_pRoot);
	_FreeData(m_pRootItem);
	m_pRoot=NULL;
	m_pRootItem=NULL;
#endif
	this->DestroyWindow();
	//ShowWindow(SW_HIDE);
}

void CAnalyDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAnalyDialog)
	DDX_Control(pDX, IDC_GRAPH, m_graph);
	DDX_Control(pDX, IDC_OBJTREE, m_tree);
	DDX_Control(pDX, IDC_LIST1, m_list);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAnalyDialog, CDialog)
	//{{AFX_MSG_MAP(CAnalyDialog)
	ON_BN_CLICKED(IDC_LIST1, OnList2)
	ON_COMMAND(IDA_CALLGRAPH, OnCallgraph)
	ON_COMMAND(IDA_FUNCTION, OnFunction)
	ON_COMMAND(IDA_OBJECTS, OnObjects)
	ON_COMMAND(IDA_START, OnStart)
	ON_COMMAND(IDA_STOP, OnStop)
	ON_COMMAND(IDA_UPDATE, OnUpdate)
	ON_COMMAND(IDA_CLEAR, OnClear)
	ON_NOTIFY(TVN_SELCHANGED, IDC_OBJTREE, OnSelchangedObjtree)
	ON_COMMAND(IDA_ENBLE, OnEnble)
	ON_COMMAND(IDA_DISABLE, OnDisable)
	ON_COMMAND(IDA_DISABLE_ALL, OnDisableAll)
	ON_COMMAND(IDA_GRAPH, OnGraph)
	ON_COMMAND(IDA_SHOWMEMBER,OnMember)
	ON_COMMAND(IDA_SEARCH,OnSearch)
	ON_COMMAND(IDA_BITMAP_SAVE,OnSaveBmp)
	ON_COMMAND(IDC_FIND,OnFind)
	ON_COMMAND(ID_SEARCH_TEXT,OnSearchTxt)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAnalyDialog message handlers

BOOL CAnalyDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	/*if (!m_toolBar.Create(this) ||
		!m_toolBar.LoadToolBar(IDR_ANABAR))
	{
		TRACE0("Failed to create toolbar\n");
		return XFALSE;      // fail to create
	}
	m_toolBar.ShowWindow(SW_SHOW);*/
	m_toolBar.Create(WS_BORDER | WS_VISIBLE | WS_CHILD
			| CCS_TOP | CCS_ADJUSTABLE | TBSTYLE_TOOLTIPS,this, IDR_ANABAR);
	
	m_images.Create(IDB_STATUS, 13, 1, RGB(255, 0, 0));
	m_objs.Create(IDB_OBJS,16,1,RGB(255,255,255));


	m_list.SetImageList(&m_images, LVSIL_STATE);
	m_tree.SetImageList(&m_objs,TVSIL_NORMAL);
	m_graph.ShowWindow(SW_HIDE);
	// TODO: Add extra initialization here
	m_list.InsertColumn(0,_T("函数"),LVCFMT_LEFT,160);
	m_list.InsertColumn(1,_T("比例"),LVCFMT_LEFT,80);
	m_list.InsertColumn(2,_T("调用次数"),LVCFMT_LEFT,80);
	m_list.InsertColumn(3,_T("总时间"),LVCFMT_LEFT,100);	
	m_list.InsertColumn(4,_T("平均"),LVCFMT_LEFT,80);
	m_list.InsertColumn(5,_T("最大"),LVCFMT_LEFT,80);
//	m_nType=XSS_NORMAL;
//	m_nSortType=XSortCall::SBPECENT;
#ifdef _ANALY
	UpdateTree();
	MakeData();
//	UpdateTree(XNULL,m_pRoot);
	switch(m_nMode)
	{
	case 0:
		UpdateData(m_pRoot,0);
		break;
	case 1:
		UpdateData(m_pRootItem,0);
		break;
	}
#endif
	EnableItems();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAnalyDialog::UpdateCallData()
{
#ifdef _ANALY
	_FreeData(m_pRoot);
	_FreeData(m_pRootItem);
	
	m_pRootItem=XNULL;
	UpdateTree();
	m_pRoot=_ANGetRoot();
	if(!m_pRoot) return;
	MakeData();
	m_list.DeleteAllItems();
//	UpdateTree(XNULL,m_pRoot);
	switch(m_nMode)
	{
	case 0:
		UpdateData(m_pRoot,0);
		break;
	case 1:
		UpdateData(m_pRootItem,0);
		break;
	}
#endif
}


#ifdef _ANALY

//DEL void CAnalyDialog::UpdateTree(HTREEITEM item,_XCALLENTRY*pRoot)
//DEL {    
//DEL 	if(item==XNULL)
//DEL 		item=TVI_ROOT;
//DEL 	_XCALLENTRY*pi=pRoot;
//DEL 	while(pi)
//DEL 	{
//DEL 		//if(pi->pChild)
//DEL 		CString strTitle(pi->strFunction);
//DEL 		HTREEITEM it=m_tree.InsertItem(strTitle,item);
//DEL 		m_tree.SetItemData(it,(DWORD)pi);
//DEL 		if(pi->pChild)
//DEL 			UpdateTree(it,pi->pChild);
//DEL 		pi=pi->pNext;
//DEL 	}
//DEL }

int CAnalyDialog::UpdateData(_XCALLENTRY*pCall,int iItem)//XU8 nType);
{
	if(pCall==XNULL) return iItem;
		//m_list.GetSelectionMark();
	while(pCall)
	{
		CString strTitle;
		strTitle=pCall->strFunction;
		m_list.InsertItem(iItem,strTitle);
		m_list.SetItemData(iItem,(DWORD)pCall);
		strTitle.Format(_T("%3.2f%%"),pCall->nPecent);
		m_list.SetItemText(iItem,1,strTitle);
		strTitle.Format(_T("%d"),pCall->nTimes);
		m_list.SetItemText(iItem,2,strTitle);

		strTitle.Format(_T("%7.2f"),pCall->nTotalTime*m_dMulti);
		m_list.SetItemText(iItem,3,strTitle);

		strTitle.Format(_T("%6.2f"),pCall->nTotalTime*m_dMulti/pCall->nTimes);
		m_list.SetItemText(iItem,4,strTitle);

		strTitle.Format(_T("%6.2f"),pCall->nMaxTime*m_dMulti);
		m_list.SetItemText(iItem,5,strTitle);

		iItem++;
		if(pCall->bExtern&&pCall->pChild)
			iItem=UpdateData(pCall->pChild,iItem);
		pCall=pCall->pNext;
	}
	//m_list.SetSelectionMark(iOffset);
	return iItem;

	/*XXCALLS calls;
	calls.Append(m_calls);
	m_calls.RemoveAll();
	XSortCall sort(&m_calls,nType);
	int i,nc=calls.GetSize();
	for(i=0;i<nc;i++)
		sort.Add(calls[i],m_nType);
	m_list.DeleteAllItems();
	for(i=0;i<nc;i++)
	{
		_CALLINFO*pInfo=m_calls[i];
		CString strTxt=(XPCTSTR)pInfo->strName;
		int item=m_list.InsertItem(i+1,strTxt);
		strTxt.Format(_T("%d"),pInfo->nCalls);
		m_list.SetItemText(item,1,strTxt);
		strTxt.Format(_T("%d"),pInfo->nTotal);
		m_list.SetItemText(item,2,strTxt);
		strTxt.Format(_T("%d.%#02d%%"),pInfo->nPecent/100,pInfo->nPecent%100);
		m_list.SetItemText(item,3,strTxt);
		strTxt.Format(_T("%d"),pInfo->nAvg);
		m_list.SetItemText(item,4,strTxt);
	}*/
}

class XSortCall:public XSort
{
public:
	XVector<_XCALLENTRY*> m_list;
	XU32 GetCount(){return m_list.GetSize();}
	void* GetData(XU32 iData){return (void*)(XU32)(m_list[iData]->nPecent*100);}
	int Compare(void*iSrt,void*iDst,XU8)
	{
		return CompareUInt((XU32)iSrt,(XU32)iDst);
	}
	void Add(_XCALLENTRY*p)
	{
		int mi;
		Index((void*)(XU32)(p->nPecent*100),mi);
		m_list.InsertAt(mi,p);
	}
};
void CAnalyDialog::MakeData()
{				
	/*m_nTotal=0;
	XS64 nTotal=_ANGetNowTime()-_ANGetStartTime();
    AddData(m_pRoot);
	int i,nc=m_calls.GetSize();
	
	nTotal=m_nTotal;

	for(i=0;i<nc;i++)
	{
		_CALLINFO*pInfo=m_calls[i];
		pInfo->nPecent=(XS64)pInfo->nTotal*10000/nTotal;
		pInfo->nAvg=pInfo->nTotal/pInfo->nCalls;
	}*/
	XFlashView*pView=m_pView->GetActiveView()->m_pFlashView;
	if(pView==XNULL) return;
	XLock lock(&pView->m_runLock);
	if(!lock.Lock()) return;

	m_nTotalTime=_ANGetTotalTime();
	m_nTotalCall=0;
	XS64 nTime=_ANGetNowTime()-_ANGetStartTime();
	XU32 nTick=XTime::GetTickCount()-_ANGetStartTick();
	m_dMulti=nTick*1000.0/nTime;
	m_pRoot=AddData(m_pRoot,XNULL);

	_XCALLENTRY*pp=m_pRootItem;
	
	//m_pRootItem=XNULL;
	XSortCall sort;
	
	while(pp)
	{
		//_XCALLENTRY**pi=&m_pRootItem;
		

		pp->nPecent=pp->nTotalTime*100.0/m_nTotalCall;
		sort.Add(pp);
		_XCALLENTRY*pi=pp->pChild;
		while(pi)
		{
			pi->nPecent=pi->nTotalTime*100.0/m_nTotalCall;
			pi=pi->pNext;
		}
		pp=pp->pNext;
	}

	int i,nc=sort.m_list.GetSize();
	m_pRootItem=XNULL;
	_XCALLENTRY**pi=&m_pRootItem;
	for(i=0;i<nc;i++)
	{
		_XCALLENTRY*p=sort.m_list[i];
		p->pNext=*pi;
		*pi=p;
	}
	lock.UnLock();
	
}

_XCALLENTRY* CAnalyDialog::AddData(_XCALLENTRY *pCall,_XCALLENTRY*pParent)
{
	_XCALLENTRY*pRoot=XNULL;
	while(pCall)
	{
		////////////////////////////////////////
		_XCALLENTRY**ppi=&m_pRootItem;
		_XCALLENTRY*pSet=XNULL;
		for(;;)
		{
			_XCALLENTRY*pi=*ppi;
			if(pi==XNULL) break;
			if(pi->strFunction==pCall->strFunction)
			{
				pSet=pi;
				break;
			}
			ppi=&pi->pNext;
		}
		if(pSet==XNULL)
		{
		   pSet=new _XCALLENTRY;
		   pSet->bExtern=XFALSE;
		   pSet->nPecent=0;
		   pSet->nTimes=0;
		   pSet->nTotalTime=0;
		   pSet->strFunction=pCall->strFunction;
		   pSet->pChild=XNULL;
		   pSet->nMaxTime=0;
		   pSet->pParent=XNULL;
		   pSet->pNext=*ppi;
		   *ppi=pSet;
		}
		pSet->nTotalTime+=pCall->nTotalTime;
		if(pSet->nMaxTime<pCall->nMaxTime)
			pSet->nMaxTime=pCall->nMaxTime;
		m_nTotalCall+=pCall->nTotalTime;
		pSet->nTimes+=pCall->nTimes;
		
		if(pCall->pParent)
		{
			_XCALLENTRY*pp=pCall->pParent;
			ppi=&pSet->pChild;
			_XCALLENTRY*pNew=XNULL;
			for(;;)
			{
				_XCALLENTRY*pi=*ppi;
				if(pi==XNULL) break;
				if(pi->strFunction==pp->strFunction)
				{
					pNew=pi;
					break;
				}
				ppi=&pi->pNext;
			}
			if(!pNew)
			{
			   pNew=new _XCALLENTRY;
			   pNew->bExtern=XFALSE;
			   pNew->nPecent=0;
			   pNew->nTimes=0;
			   pNew->nTotalTime=0;
			   pNew->pChild=XNULL;
			   pNew->nMaxTime=0;
			   pNew->strFunction=pp->strFunction;
			   pNew->pNext=*ppi;
			   *ppi=pNew;
			   pNew->pParent=pSet;
			}
			pNew->nTotalTime+=pCall->nTotalTime;
			pNew->nTimes+=pCall->nTimes;
			if(pNew->nMaxTime<pCall->nMaxTime)
				pNew->nMaxTime=pCall->nMaxTime;
			
		}

		///////////////////////////////////////

		_XCALLENTRY*pItem=new _XCALLENTRY;
		pItem->bExtern=XFALSE;
		pItem->nTotalTime=pCall->nTotalTime;
		pItem->nPecent=pCall->nTotalTime*100.0/m_nTotalTime;
		pItem->nMaxTime=pCall->nMaxTime;
		pItem->nTimes=pCall->nTimes;
		pItem->pChild=XNULL;
		pItem->pParent=pParent;
		pItem->pNext=XNULL;
		pItem->strFunction=pCall->strFunction;
		if(pRoot==XNULL) pRoot=pItem;
		else
		{
		   _XCALLENTRY**p=&pRoot;
		   for(;;)
		   {
			   _XCALLENTRY*pi=*p;
			   if(pi==XNULL)
			   {
				   *p=pItem;
				   break;
			   }
			   else if(pi->nTotalTime<=pItem->nTotalTime)
			   {
				   pItem->pNext=pi;
				   *p=pItem;
				   break;
			   }
			   p=&pi->pNext;
		   }
		}
		if(pCall->pChild)
			pItem->pChild=AddData(pCall->pChild,pItem);
		pCall=pCall->pNext;
	}
	return pRoot;
}

#endif


//DEL void CAnalyDialog::OnClickList1(NMHDR* pNMHDR, LRESULT* pResult) 
//DEL {
//DEL 	// TODO: Add your control notification handler code here
//DEL 	
//DEL 	*pResult = 0;
//DEL }

void CAnalyDialog::OnList2() 
{
	// TODO: Add your control notification handler code here
	int iOffset=m_list.GetScrollPos(SB_VERT);
	//int is=m_list.GetScrollPos(SB_HORZ);
	//SCROLLINFO info;
	//m_list.GetScrollInfo(SB_VERT,&info);
	m_list.DeleteAllItems();
#ifdef _ANALY
	switch(m_nMode)
	{
	case 0:
		UpdateData(m_pRoot,0);
		break;
	case 1:
		UpdateData(m_pRootItem,0);
		break;
	}
	//UpdateData(m_pRoot,0);
#endif
	//m_list.SetSelectionMark(iOffset);
	if(iOffset)//iOffset)
	{
		m_list.Scroll(CSize(0,iOffset*20));
		//m_list.SetSelectionMark(iOffset+1);
		//m_list.SetScrollPos(SB_VERT,iOffset);
	}
	//m_list.SetScrollPos(SB_VERT,iOffset*20);
}

/*void CAnalyDialog::OnButton1() 
{
	// TODO: Add your control notification handler code here
	SCROLLINFO info;
	int iOffset=m_list.GetScrollInfo(SB_VERT,&info);
	//m_list.Set
	//m_list.SetScrollPos(SB_VERT,iOffset+1,FALSE);
	m_list.Scroll(CSize(0,20));
}*/

/*void CAnalyDialog::OnMode() 
{
	// TODO: Add your control notification handler code here
	m_nMode=1-m_nMode;
	m_list.DeleteAllItems();
	switch(m_nMode)
	{
	case 0:
		UpdateData(m_pRoot,0);
		break;
	case 1:
		UpdateData(m_pRootItem,0);
		break;
	}

}*/
struct CToolBarData
{
	WORD wVersion;
	WORD wWidth;
	WORD wHeight;
	WORD wItemCount;
	//WORD aItems[wItemCount]

	WORD* items()
		{ return (WORD*)(this+1); }
};



BOOL CAnaToolBar::Create(DWORD nStyle, CWnd *pParent, DWORD nID)
{
	CRect rect(0,0,0,0);
	//pParent->GetClientRect(rect);
	//rect.InflateRect(20,20);
  	BOOL bRet = CToolBarCtrl::Create(nStyle, rect, pParent, nID);


	LPCTSTR lpszResourceName=MAKEINTRESOURCE(nID);
	ASSERT_VALID(this);
	ASSERT(lpszResourceName != NULL);

	// determine location of the bitmap in resource fork
	HINSTANCE hInst = AfxFindResourceHandle(lpszResourceName, RT_TOOLBAR);
	HRSRC hRsrc = ::FindResource(hInst, lpszResourceName, RT_TOOLBAR);
	if (hRsrc == NULL)
		return FALSE;

	HGLOBAL hGlobal = LoadResource(hInst, hRsrc);
	if (hGlobal == NULL)
		return FALSE;

	CToolBarData* pData = (CToolBarData*)LockResource(hGlobal);
	if (pData == NULL)
		return FALSE;
	ASSERT(pData->wVersion == 1);

	m_nButtonCount=pData->wItemCount;

	//UINT* pItems = new UINT[pData->wItemCount];
	//for (int i = 0; i < pData->wItemCount; i++)
	//	pItems[i] = pData->items()[i];
	//BOOL bResult = SetButtons(pItems, pData->wItemCount);
	
	/*if (bResult)
	{
		// set new sizes of the buttons
		CSize sizeImage(pData->wWidth, pData->wHeight);
		CSize sizeButton(pData->wWidth + 7, pData->wHeight + 7);
		SetSizes(sizeButton, sizeImage);

		// load bitmap now that sizes are known by the toolbar control
		bResult = LoadBitmap(lpszResourceName);
	}*/	

	VERIFY(AddBitmap(m_nButtonCount,nID) != -1);

	//m_pTBButtons = new TBBUTTON[m_nButtonCount];
	TBBUTTON sepButton;
	sepButton.idCommand = 0;
	sepButton.fsStyle = TBSTYLE_SEP;
	sepButton.fsState = TBSTATE_ENABLED;
	sepButton.iString = 0;
	sepButton.iBitmap = 0;
	sepButton.dwData = 0;

	AddButtons(1,&sepButton);
	XBOOL bGroup=TRUE;
	int iImage=0;
	for (int nIndex = 0; nIndex < m_nButtonCount; nIndex++)
	{				
		WORD nCommand=pData->items()[nIndex];
		if(nCommand)
		{
			CString string;
			string.LoadString(nCommand);
			TBBUTTON button;
			// Add second '\0'
			int nStringLength = string.GetLength() + 1;
			TCHAR * pString = string.GetBufferSetLength(nStringLength);
			pString[nStringLength] = 0;

			VERIFY((button.iString = AddStrings(pString)) != -1);

			string.ReleaseBuffer();


			button.fsState = TBSTATE_ENABLED;
			if(nIndex==0)
				button.fsState|=TBSTATE_CHECKED;
			button.fsStyle = bGroup?TBSTYLE_CHECKGROUP:TBSTYLE_BUTTON;
			button.dwData = 0;
			button.iBitmap = iImage;
			button.idCommand = nCommand;
			AddButtons(1,&button);
			iImage++;
		}
		else
		{
			bGroup=FALSE;
			AddButtons(1,&sepButton);
		}
	}

	

	/*for (nIndex = 0; nIndex < m_nButtonCount; nIndex++)
	{
		VERIFY(AddButtons(1,&m_pTBButtons[nIndex]));
		if (!((nIndex +1) % 3))
		{
			VERIFY(AddButtons(1,&sepButton));
		}
	}*/

	UnlockResource(hGlobal);
	FreeResource(hGlobal);
	
	AutoSize();

	return bRet;
}

void CAnalyDialog::OnCallgraph() 
{
	// TODO: Add your command handler code here
#ifdef _ANALY
	m_nMode=0;
	KillTimer(2000);
	m_list.ShowWindow(SW_SHOW);
	m_tree.ShowWindow(SW_HIDE);
	m_graph.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_FINDTEXT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_FIND)->ShowWindow(SW_HIDE);
	m_list.DeleteAllItems();
	UpdateData(m_pRoot,0);
	EnableItems();
#endif
}

void CAnalyDialog::OnFunction() 
{
	// TODO: Add your command handler code here
#ifdef _ANALY
	m_nMode=1;
	KillTimer(2000);
	GetDlgItem(IDC_FINDTEXT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_FIND)->ShowWindow(SW_HIDE);
	m_list.ShowWindow(SW_SHOW);
	m_tree.ShowWindow(SW_HIDE);
	m_graph.ShowWindow(SW_HIDE);	
	m_list.DeleteAllItems();
	UpdateData(m_pRootItem,0);
	EnableItems();
#endif
}


void CAnalyDialog::OnGraph() 
{
	// TODO: Add your command handler code here
	m_nMode=3;
	GetDlgItem(IDC_FINDTEXT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_FIND)->ShowWindow(SW_HIDE);
	m_list.ShowWindow(SW_HIDE);
	m_tree.ShowWindow(SW_HIDE);
	m_graph.ShowWindow(SW_SHOW);	
	SetTimer(2000,1000,XNULL);
	EnableItems();
}


void CAnalyDialog::OnObjects() 
{
	// TODO: Add your command handler code here
	m_nMode=2;
	KillTimer(2000);
	m_list.ShowWindow(SW_HIDE);
	m_tree.ShowWindow(SW_SHOW);	
	m_graph.ShowWindow(SW_HIDE);
	EnableItems();
}

void CAnalyDialog::UpdateTree()
{
    if(m_pPlayer==XNULL) return;

	m_tree.DeleteAllItems();
	SObject*pObject=m_pPlayer->GetRootObject();
	if(!pObject) return;

	XFlashView*pView=m_pView->GetActiveView()->m_pFlashView;
	if(pView==XNULL) return;
	XLock lock(&pView->m_runLock);
	if(!lock.Lock()) return;

	//pObject=pObject->bottomChild;
	while(pObject)
	{
		//pObject=pObject->p
		HTREEITEM item=AddTree(lock,pObject,TVI_ROOT);		
		pObject=pObject->above;
	}

	lock.UnLock();
}



HTREEITEM CAnalyDialog::AddTree(XLock&lock,SObject *pObject, HTREEITEM item)
{
	CString strTitle;
	/*if(!pObject->name.IsNull())
	{
		strTitle=pObject->name.strTxt;
		if(pObject->character&&pObject->character->className)
		{
			strTitle+=_T("(");
			XString8 s(pObject->character->className);
			s.ConvertToGB();
			strTitle+=s;
			strTitle+=_T(")");
		}
	}
	else if(pObject->character&&pObject->character->className)
	{
		strTitle=_T("(");
		XString8 s(pObject->character->className);
		s.ConvertToGB();
		strTitle+=s;
		strTitle+=_T(")");
	}
	else*/
	{
		XString16 name;
		pObject->GetName(name);
		strTitle=name;
	}
	if(pObject->pCacheBmp)
	{
		strTitle+=XString16("(Cached)");
	}
	//CString strTmp;
	//XString16 name;
	//pObject->GetName(name);
	/*XXVar var=pObject->name;
	if(var.IsNull()&&pObject->character)
	{
		var=pObject->character->className;
	}	
	CString strName(var.strTxt),strTmp;
	if(var.IsNull()&&pObject->pASObject)*/
	CString strTmp;
	strTmp.Format(_T("(%x,depth:%d,clip:%d,tag:%d,type:%d,ratio:%d,blend:%d)(s:%x,as2:%x,as3:%x,flag:%x,%x)"),
		pObject->pASObject,
		pObject->depth,pObject->clipDepth,
		pObject->character?pObject->character->tag:0,pObject->character?pObject->character->type:0,
		pObject->ratio,pObject->blendMode,
		pObject,
		pObject->pObject,pObject->pASObject,pObject->flags,pObject->exflags);
	int iType=0;
	
	if(pObject->character)
	{
		switch(pObject->character->type)
		{
		case morphShapeChar:iType=20;break;
		case shapeChar:iType=1;break;
		case bitmapChar:
		case bitsChar:iType=2;break;
		case buttonChar:iType=3;break;
		case textChar:iType=4;break;
		case spriteEmptyChar:iType=pObject->pASObject?22:7;break;
		case spriteExternalChar:iType=7;break;
		case spriteChar:iType=5;
			 if(pObject->IsCloneObject())
				 iType=6;
			 if(pObject->thread->playing)
			 {
				 strTmp+=_T("-playing");
			 }
			 else
			 {
				 strTmp+=_T("-stoped");
			 }
			 break;
		case textLineChar:
		case editTextChar:iType=8;break;			// used for the Flash Generator
//		case drawChar:iType=9;break;
		case rootChar:iType=10;break;
		case videoChar:iType=21;break;
		//case spriteEmptyChar:iType=22;break;
		case editTextEmptyChar:iType=23;break;
		case buttonEmptyChar:iType=24;break;
		case shapeEmptyChar:iType=25;break;
		//case bitmapChar:iType=2;break;
		case loaderImageChar:
		case bitmapEmptyChar:iType=26;break;
		case playerChar:iType=27;break;
		case videoEmptyChar:iType=21;break;	
		case loaderChar:iType=35;break;
		}
	}
	if(pObject->clipDepth)
		iType=11;
	strTitle+=strTmp;
	item=m_tree.InsertItem(strTitle,iType,iType,item);
	
	m_tree.SetItemData(item,(DWORD)pObject);
	//if(iType!=10)
	{
		MATRIX&mat=pObject->xform.mat;
		strTitle.Format(_T("a:%f b:%f c:%f d:%f tx:%d ty:%d,rot:%d"),
						FixedToFloat(mat.a),
						FixedToFloat(mat.b),
						FixedToFloat(mat.c),
						FixedToFloat(mat.d),mat.tx,mat.ty,
						pObject->control.rotX>>16);
		m_tree.InsertItem(strTitle,12,12,item);
		ColorTransform&t=pObject->xform.cxform;		
		strTitle.Format(_T("f:%x,aa:%d ra:%d ga:%d ba:%d,ab:%d rb:%d gb:%d bb:%d"),
						t.flags,
						t.aa,t.ra,t.ga,t.ba,t.ab,t.rb,t.gb,t.bb);
		m_tree.InsertItem(strTitle,12,12,item);

	}
	if(!RectIsEmpty(&pObject->devBounds))
	{
	   SRECT&rr=GETBOUNDS(pObject->character,pObject->editText);
	   strTitle.Format(_T("(l:%d t:%d r:%d b:%d),(l:%d t:%d r:%d b:%d)"),
					   pObject->devBounds.xmin,
					   pObject->devBounds.ymin,
					   pObject->devBounds.xmax,
					   pObject->devBounds.ymax,
					   rr.xmin,
					   rr.ymin,
					   rr.xmax,
					   rr.ymax);
	   m_tree.InsertItem(strTitle,12,12,item);
	}
	if(pObject->gridRect)
	{
		strTitle.Format(_T("Grid9Scale(l:%d t:%d r:%d b:%d)"),
			pObject->gridRect->xmin,
			pObject->gridRect->ymin,
			pObject->gridRect->xmax,
			pObject->gridRect->ymax);
		m_tree.InsertItem(strTitle,12,12,item);
	}
	if(pObject->IsRootObject())
	{
		XString16 url;
		url.ConvertFrom(pObject->thread->player->m_url.strTxt,XCHARSET_GB2312);
		m_tree.InsertItem(url,0,0,item);
		url.ConvertFrom(pObject->thread->player->m_loadUrl.strTxt,XCHARSET_GB2312);
		m_tree.InsertItem(url,0,0,item);
		strTitle.Format(_T("Script Length:%d"),pObject->thread->player->scriptLen);
		m_tree.InsertItem(strTitle,12,12,item);
	}
	
	if(!pObject->IsVisible())
	{
		strTitle.Format(_T("Visible:false"));
		m_tree.InsertItem(strTitle,12,12,item);
	}

	if(!pObject->IsEnable())
	{
		strTitle.Format(_T("Enable:false"));
		m_tree.InsertItem(strTitle,12,12,item);
	}

	if(pObject->pFilter)
	{
		strTitle.Format(_T("Filters:%d"),pObject->pFilter->m_nFilterID);
		m_tree.InsertItem(strTitle,12,12,item);
	}

	int nCmds=pObject->GetDrawCmds();
	if(nCmds)
	{
		strTitle.Format(_T("Graphics Draw:%d"),nCmds);
		m_tree.InsertItem(strTitle,9,9,item);
	}

	if(pObject->character)
	{
		switch(pObject->character->type)
		{
		  case textLineChar:
		  case editTextChar:
			   if(pObject->editText->m_variable.IsNull())
			   {
				   CString str(pObject->editText->m_variable.strTxt);
				   strTitle.Format(_T("var:%s"),str);
				   m_tree.InsertItem(strTitle,12,12,item);
			   }
			   {
				   //XString8 str(pObject->editText->m_buffer);
				   //str.ConvertToGB();
				   strTitle.Format(_T("text:%s"),pObject->editText->m_buffer);
				   m_tree.InsertItem(strTitle,12,12,item);
			   }
			   {
				   XString16 s(pObject->editText->m_initialText.strTxt);
				   //CString str(pObject->editText->m_initialText.strTxt);
				   strTitle.Format(_T("html:%s"),s);
				   m_tree.InsertItem(strTitle,12,12,item);
			   }
			   break;
		 case spriteEmptyChar:
		 case spriteExternalChar:
		 case spriteChar:iType=5;		 
		 case rootChar:iType=10;
			  strTitle.Format(_T("Total Frames:%d,Current Frame:%d"),
							  pObject->thread->numFrames,pObject->thread->curFrame);
			  m_tree.InsertItem(strTitle,15,15,item);
			  if(pObject->thread->m_cmds)
			  {
				  _XCOMMAND*cmd=pObject->thread->m_cmds;
				  int iCmds=0;
				  while(cmd)
				  {					  
					  iCmds++;
					  cmd=cmd->pNext;
				  }
				  strTitle.Format(_T("Draw Commands:%d"),iCmds);
				  m_tree.InsertItem(strTitle,15,15,item);
			  }break;
		}
	}


	SObject*pObj=pObject->bottomChild;
	while(pObj)
	{
		AddTree(lock,pObj,item);
		pObj=pObj->above;
	}
	//if(pObject->pASObject)
	//{
	//	if(!pObject->character||pObject->character->type!=bitsChar)
	//		AddTree(&lock,pObject->pASObject,item);
	//}
	if(pObject->pObject&&pObject->pObject->m_pTokens)
	{
		AddTree(lock,pObject->pObject,item,0);

	}

	if(pObject->character&&pObject->character->type==rootChar)
	{
		ScriptPlayer* player = pObject->character->splayer;//rootPlayer;
		XXObject*pGlobal=(XXObject*)player->m_pGlobal;
		if(pGlobal)
		{
			item=m_tree.InsertItem(_T("Global"),7,7,item);
			AddTree(lock,pGlobal,item);
		}
	}

	return item;
}


void CAnalyDialog::EnableItems()
{
#ifdef _ANALY
	XBOOL bStart=_ANIsStarted();
	m_toolBar.EnableButton(IDA_START,!bStart);
	m_toolBar.EnableButton(IDA_STOP,bStart);
	m_toolBar.EnableButton(IDA_CLEAR,TRUE);//_ANGetRoot()!=XNULL&&m_nMode<=2);
	XU32 nID=0;
	if(m_nMode==2)
	{
		HTREEITEM item=m_tree.GetSelectedItem();
		if(item!=XNULL) 
			nID=m_tree.GetItemData(item);
		SObject*pObj=(SObject*)(nID&~7);
		if(nID&7)
			m_toolBar.EnableButton(IDA_BITMAP_SAVE,TRUE);
		else if(!pObj||!pObj->character)
			m_toolBar.EnableButton(IDA_BITMAP_SAVE,FALSE);
		else
		{
			switch(pObj->character->type)
			{
			case bitsChar:
			case bitmapChar:
			case bitmapEmptyChar:
				 m_toolBar.EnableButton(IDA_BITMAP_SAVE,TRUE);
				 break;
			default:
				if(pObj->pCacheBmp)
					m_toolBar.EnableButton(IDA_BITMAP_SAVE,TRUE);
				else
					m_toolBar.EnableButton(IDA_BITMAP_SAVE,FALSE);
			}
		}
	}
	else
		m_toolBar.EnableButton(IDA_BITMAP_SAVE,FALSE);
	m_toolBar.EnableButton(IDA_ENBLE,nID&&_ANIsDisabled(nID));
	m_toolBar.EnableButton(IDA_DISABLE,nID&&!_ANIsDisabled(nID));
	m_toolBar.EnableButton(IDA_DISABLE_ALL,nID);
	m_toolBar.EnableButton(IDA_SHOWMEMBER,(nID>>3));
	m_toolBar.EnableButton(IDA_SEARCH,m_nMode==2);
	
#endif
}



void CAnalyDialog::OnStart() 
{
	// TODO: Add your command handler code here
#ifdef _ANALY
	_ANStart();
#endif
	EnableItems();
}

void CAnalyDialog::OnStop() 
{
	// TODO: Add your command handler code here
#ifdef _ANALY
	_ANStop();
#endif
	EnableItems();
}

void CAnalyDialog::OnUpdate() 
{
	// TODO: Add your command handler code here
	UpdateCallData();
	EnableItems();
}

void CAnalyDialog::OnClear() 
{
	// TODO: Add your command handler code here
#ifdef _ANALY
	_ANClear();
	_FreeData(m_pRoot);
	_FreeData(m_pRootItem);
	m_pRootItem=XNULL;
	m_pRoot=XNULL;
	m_list.DeleteAllItems();
	EnableItems();
#endif
}

void CAnalyDialog::OnSelchangedObjtree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
	EnableItems();
}

void CAnalyDialog::OnEnble() 
{
	// TODO: Add your command handler code here
	
	HTREEITEM item=m_tree.GetSelectedItem();
	if(item==XNULL) return;
	DWORD id=m_tree.GetItemData(item);
	if(id==0) return;

	int nImage,nSelect;
	m_tree.GetItemImage(item,nImage,nSelect);
	m_tree.SetItemImage(item,nSelect,nSelect);
#ifdef _ANALY
	_ANEnable(id);
	_XSObject*pObject=(_XSObject*)id;
	pObject->SetVisible(XTRUE);
#endif
//	m_pPlayer->SetCamera(XSWFPlayer::updateNow);
	m_pPlayer->display.Invalidate();
//	m_pPlayer->display.Update();
//	m_pView->Invalidate();
//	m_pView->OnIdle();
	//AfxGetMainWnd()->Invalidate();
	EnableItems();
}

void CAnalyDialog::OnDisable() 
{
	// TODO: Add your command handler code here
	HTREEITEM item=m_tree.GetSelectedItem();
	if(item==XNULL) return;
	DWORD id=m_tree.GetItemData(item);
	if(id==0) return;
	int nImage,nSelect;
	m_tree.GetItemImage(item,nImage,nSelect);
	m_tree.SetItemImage(item,16,nSelect);
#ifdef _ANALY
	_ANDisable(id);
	_XSObject*pObject=(_XSObject*)id;
	pObject->SetVisible(XFALSE);
#endif
	//m_pPlayer->SetCamera(XSWFPlayer::updateNow);
	m_pPlayer->display.Invalidate();
//	m_pPlayer->display.Update();
//	m_pView->Invalidate();
	//m_pView->OnIdle();
	//AfxGetMainWnd()->Invalidate();
	EnableItems();

}

//DEL int CAnalyDialog::DoModal() 
//DEL {
//DEL 	// TODO: Add your specialized code here and/or call the base class
//DEL 	
//DEL 	return CDialog::DoModal();
//DEL }

XBOOL CAnalyDialog::ContinueModal()
{
	m_pView->OnIdle();
	return CDialog::ContinueModal();
}

void CAnalyDialog::OnDisableAll() 
{
	// TODO: Add your command handler code here
	HTREEITEM item=m_tree.GetSelectedItem();
	if(item==XNULL) return;
	item=m_tree.GetChildItem(item);
	while(item)
	{
	   DWORD id=m_tree.GetItemData(item);
	   if(id)
	   {
#ifdef _ANALY
		   _ANDisable(id);
#endif
		   int im,is;
		   m_tree.GetItemImage(item,im,is);
		   m_tree.SetItemImage(item,16,is);
	   }
	   item=m_tree.GetNextItem(item,TVGN_NEXT);
	}
	m_pPlayer->display.Invalidate();
}





void CAnalyDialog::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_nMode==3)
	{
		m_graph.m_dMulti=m_dMulti;
		XBOOL bSet=XFALSE;
		XFlashView*pView=m_pView->GetActiveView()->m_pFlashView;
		if(!pView) return;
		XLock lock(&pView->m_runLock);
		if(lock.Lock()) //return;
		{
			bSet=m_graph.Update();
			lock.UnLock();
		}
		if(bSet)
			m_graph.Invalidate();
	}
	CDialog::OnTimer(nIDEvent);
}

void CAnalyDialog::AddTree(XLock&lock,XXObject *pObject,HTREEITEM item,int iStack)
{
	if(iStack>3) return;
	CString strTitle;
	XXTOKENS*tokens=pObject->m_pTokens;
	if(tokens==XNULL) return;
	int i,nc=tokens->GetSize();
	for(i=0;i<nc;i++)
	{
		XXTOKEN* token=(*tokens)[i];

		if(token->pGetter)
		{
			strTitle=token->strKey.strTxt;
			m_tree.InsertItem(strTitle,14,14,item);
		}
		else
		{
			XXVar var=token->member;
			int iIcon=(var.IsObject()&&!var.IsSysCall())?17:13;			
			XXObject*po=var.pObject;
			int is=iIcon==17&&po&&po->IsObject(XXOBJ_FUNCTION)?19:iIcon;
			
			strTitle=token->strKey.strTxt;
			strTitle+=_T(":");
			if(var.IsObject()&&var.pObject)
			{
				XXObject*pObject=var.pObject->m_pSuper;
#ifdef _ANALY
				if(pObject&&!pObject->m_strClassNameT.IsNull()&&!
							 pObject->m_strClassNameT.IsUndefine())
					strTitle+=pObject->m_strClassNameT.strTxt;
				else
#endif
				{
					var.ToString(XFALSE);
					XString16 strTmp(var.strTxt);					
					//strTmp.ConvertToGB();
					strTitle+=strTmp.GetData();//var.strTxt;
				}
			}
			else
			{
				var.ToString(XFALSE);
				XString16 strTmp(var.strTxt);					
				//strTmp.ConvertToGB();
				strTitle+=strTmp.GetData();//var.strTxt;
				//strTitle+=var.strTxt;
			}
			HTREEITEM ii=m_tree.InsertItem(strTitle,is,is,item);

			if(iIcon==17&&po)
				AddTree(lock,po,ii,iStack+1);
		}
	}
	if(pObject->m_pEvents)
	{
		XXObject::_XEVENT*p=pObject->m_pEvents;
		while(p)
		{
			XXVar var(p->pHandler);
			var.ToString(XFALSE);
			strTitle=_T("OnEvent:");
			strTitle+=var.strTxt;
			m_tree.InsertItem(strTitle,18,18,item);
			p=p->pNext;
		}
	}

}

void CAnalyDialog::OnSaveBmp()
{
#ifdef _ANALY
	HTREEITEM item=m_tree.GetSelectedItem();
	XU32 nData=0;
	if(item!=XNULL) 
		nData=m_tree.GetItemData(item);
	bool bAS = (nData&7)!=0;
	nData&=~7;
	if(!nData) return;
	SObject*pObj=(SObject*)nData;
	XDrawDib dib;
	if(this->m_pPlayer->GetObjBitmap(pObj,dib,bAS))
	{
		//dib.SaveFile(
		CFileDialog dlg(false,_T("png"),_T(""),OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("图片|*.png|"));
		if(dlg.DoModal()!=IDOK) return;
		CString strFile=dlg.GetPathName();
		XString16 ss(strFile);
		XString8  s2(ss);
		dib.SaveFile(s2);
	}
#endif
}

void CAnalyDialog::OnMember()
{
#ifdef _ANALY
	HTREEITEM item=m_tree.GetSelectedItem();
	XU32 nData=0;
	if(item!=XNULL) 
		nData=m_tree.GetItemData(item);
	if((nData&7)!=7)
	{
		nData&=~7;
		if(nData==0) return;
		_XSObject*obj=(_XSObject*)nData;
		if(obj->pASObject)
			AddTree(NULL,obj->pASObject,item);
		return;
	}
	nData&=~7;
	if(nData==0) return;
	AddTree(NULL,(void*)nData,item);	

#endif
}

void CAnalyDialog::OnSearch()
{
#ifdef _ANALY
	m_tree.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_FINDTEXT)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_FIND)->ShowWindow(SW_SHOW);
	m_nSeMode=0;
#endif
}

bool CAnalyDialog::Search(HTREEITEM item,CString&strTxt,bool bSelf,bool bChild,bool bNext,bool bParent)
{
	if(bSelf)
	{
		CString strCnt=m_tree.GetItemText(item);
		if(strCnt.Find(strTxt)>=0)
		{
			m_tree.Expand(item,TVE_EXPAND);
			m_tree.Select(item,TVGN_CARET);
			return true;
		}
	}
	if(bChild)
	{
		HTREEITEM child=m_tree.GetNextItem(item,TVGN_CHILD);
			//m_tree.GetChildItem(item);
		while(child)
		{
			if(Search(child,strTxt,true,true,false,false)) return true;
			child=m_tree.GetNextItem(child,TVGN_NEXT);
			//child=m_tree.
		}
	}
	if(bNext)
	{
		HTREEITEM child=m_tree.GetNextItem(item,TVGN_NEXT);
		while(child)
		{
			if(Search(child,strTxt,true,true,false,false)) return true;
				child=m_tree.GetNextItem(child,TVGN_NEXT);
		}
	}
	if(bParent)
	{
	//if(child) return Search(child,strTxt,true);
		HTREEITEM child=m_tree.GetParentItem(item);
		if(child)
			return Search(child,strTxt,false,false,true,true);
	}
	//if(child)
	//	return Search(child,strTxt,false,false);
	return false;
}

void CAnalyDialog::OnSearchTxt()
{
#ifdef _ANALY
	CString strTxt;
	GetDlgItemText(IDC_FINDTEXT,strTxt);
	if(strTxt.IsEmpty())
	{
		m_tree.ShowWindow(SW_HIDE);
		GetDlgItem(IDC_FINDTEXT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_FIND)->ShowWindow(SW_SHOW);
		m_nSeMode=1;
		m_tree.SetFocus();
		return;
	}
	GetDlgItem(IDC_FINDTEXT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_FIND)->ShowWindow(SW_HIDE);	
	m_tree.ShowWindow(SW_SHOW);
	HTREEITEM item=m_tree.GetSelectedItem();
	if(item)
	{
		Search(item,strTxt,false);
	}
	else
		Search(m_tree.GetRootItem(),strTxt,true);
	//if(item==NULL)
	//	item=m_tree.GetRootItem();
#endif
}

void CAnalyDialog::OnFind()
{
#ifdef _ANALY
	GetDlgItem(IDC_FINDTEXT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_FIND)->ShowWindow(SW_HIDE);	
	m_tree.ShowWindow(SW_SHOW);
	
	CString strTxt;
	GetDlgItemText(IDC_FINDTEXT,strTxt);
	if(m_nSeMode==1)
	{
		if(!strTxt.IsEmpty())
			OnSearchTxt();
		return;
	}
	//XString16 str16(strTxt);	
	if(!strTxt.IsEmpty())
	{
		XString8 str(strTxt);
		str.ConvertFrom(XCHARSET_GB2312);
		HTREEITEM item=m_tree.GetSelectedItem();
		void*pASObject=NULL;
		if(item)
		{
			XU32 nData=m_tree.GetItemData(item);
			if((nData&7)==7&&(nData>>3))
				pASObject=(void*)(nData&~7);
		}
		else item=TVI_ROOT;
		void*pObj=m_pPlayer->AS3FindClass(str,pASObject);
		if(pObj)
		{
			HTREEITEM root=m_tree.InsertItem(strTxt,22,22,item);
			AddTree(NULL,pObj,root);
		}
	}
#endif
}

void CAnalyDialog::AddTree(XLock*lock,void* pASObject,HTREEITEM root)
{
	//return;
#ifdef _ANALY
	if(lock)
		lock->UnLock();
	XString8 strTree;
	m_pPlayer->GetClassTree(strTree,pASObject);
	if(!strTree.IsEmpty())
	{
		XString8Array list;
		list.SplitString(strTree,'\n');
		int i;
		HTREEITEM item=root;
		for(i=0;i<list.GetSize();i++)
		{
			XString8&strTmp=list[i];
			if(!strTmp.IsEmpty())
			{
				strTmp.ConvertToGB();
				CString str(strTmp);
				item=m_tree.InsertItem(str,12,12,item);
			}
		}
	}
	AS3PROPITEM*item=m_pPlayer->AS3GetChildren(pASObject,true);
	if(lock)
		lock->Lock();
	while(item)
	{
		AS3PROPITEM*next=item->next;
		CString strTitle=item->name;
		int nType=item->nType+28;
		if(!item->value.IsEmpty())
		{
			strTitle+=":";
			strTitle+=item->value;
		}

		/*switch(item->nType)
		{
		case AS3PROPITEM::NUMBER:
			 nType=28;
		case AS3PROPITEM::OBJECT:
		case AS3PROPITEM::EVENT:
		case AS3PROPITEM::STRING:
		case AS3PROPITEM::LOGIC:
		case AS3PROPITEM::NAMESPACE:
		case AS3PROPITEM::OTHER:
			 break;
		}*/
		HTREEITEM it=m_tree.InsertItem(strTitle,nType,nType,root);
		m_tree.SetItemData(it,item->nData|0x7);
		delete item;
		item=next;
	}
#endif
}