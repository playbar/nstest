// Dom.cpp : implementation file
//

#include "stdafx.h"
#include "GGClient.h"
#include "Dom.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDom dialog


CDom::CDom(XDomItem*p,CWnd* pParent /*=NULL*/)
	: CDialog(CDom::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDom)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDoc=p;
}


void CDom::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDom)
	DDX_Control(pDX, IDC_TREE, m_tree);
	//}}AFX_DATA_MAP
	m_pRoot=TVI_ROOT;
}


BEGIN_MESSAGE_MAP(CDom, CDialog)
	//{{AFX_MSG_MAP(CDom)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE, OnDblclkTree)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDom message handlers

BOOL CDom::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_images.Create(IDB_DOMLIST,16,16,0xffffff);
	m_tree.SetImageList(&m_images,TVSIL_NORMAL);
	if(m_pDoc!=NULL)
		AddItem(TVI_ROOT,m_pDoc);
	/*{
		XStack<XDomItem*> stacks;
		if(m_pDoc!=XNULL)
			stacks.Add(m_pDoc);
		m_pRoot=TVI_ROOT;
		while(stacks.GetSize()>0)
		{
			XDomItem*p=XNULL;
			if(!stacks.Pop(p)) break;
			m_pRoot=AddItem(p);
			int nCount=p->GetChildCount();
			for(int i=0;i<nCount;i++)
			{
				XDomItem*pi=p->GetChild(i);
				if(pi!=NULL) 
				{
					stacks.Push(pi);

				}
			}
		}

	}*/
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HTREEITEM CDom::AddItem(HTREEITEM root,XDomItem *p)
{
	int id=p->GetID();
	XPCTSTR pStr=XDomPhaseXML::GetToken(id);
	CString str;
	if(pStr==XNULL) 
	{
		if(id==XDOM_TEXT)
			str=_T("文本");
		else if(id==XDOM_SPACE)
			str=_T("空格");
		else str=_T("未知");
	}
	else			   str=pStr;
	HTREEITEM ht=m_tree.InsertItem(str,0,0,root);
	m_tree.SetItemData(ht,(DWORD)p);
	int nCount=p->GetAttribCount();
	if(nCount>0)
	{
		XVar var;
		for(int i=0;i<nCount;i++)
		{
			if(!p->GetAttribByIndex(var,i)) continue;
			if(var.nID>0xff) continue;
		    pStr=XEAB()[var.nID];
			if(pStr==XNULL) continue;
			str=pStr;
			XString16 strValue=var.ToString();
			str+=_T("(");
			str+=strValue;
			str+=_T(")");
			m_tree.InsertItem(str,1,1,ht);
			
		}
		XString16 strTxt;
		if(p->GetText(strTxt))
		{
			str=strTxt;
			m_tree.InsertItem(str,2,2,ht);
		}
	}
	nCount=p->GetChildCount();
	for(int i=0;i<nCount;i++)
	{
		XDomItem*pi=p->GetChild(i);
		if(pi==NULL) continue;
		AddItem(ht,pi);
	}
	return ht;
}

void CDom::OnDblclkTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	HTREEITEM item=m_tree.GetSelectedItem();
	if(item!=XNULL)
	{
		XDomItem*p=(XDomItem*)m_tree.GetItemData(item);
		if(p!=XNULL)
		{
			XRect rect;
			p->GetRoundRect(rect);
			CString str;
			str.Format(_T("%d,%d,%d,%d"),rect.left,rect.top,rect.right,rect.bottom);
			AfxMessageBox(str);
		}
	}
	*pResult = 0;
}
