// GraphButton.cpp : implementation file
//

#include "stdafx.h"
#include "GGClient.h"
#include "GraphButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGraphButton

CGraphButton::CGraphButton()
{
	m_nCount=0;
	m_nMaxSamples=0;
	m_pDatas=XNULL;
}

CGraphButton::~CGraphButton()
{
	if(m_pDatas)
		delete m_pDatas;
}


BEGIN_MESSAGE_MAP(CGraphButton, CButton)
	//{{AFX_MSG_MAP(CGraphButton)
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGraphButton message handlers

void CGraphButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your code to draw the specified item
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	CBrush brush((COLORREF)0);
	pDC->FillRect(CRect(0,0,m_nMaxSamples,m_nHeight),&brush);
	CPen pen(PS_SOLID,1,0x3fff3f);
	CPen tpen(PS_SOLID,1,0x1f7f1f);
	CBrush tb(0x1f7f1f);
	void*pOld=pDC->SelectObject(&pen);
	pDC->SetBkMode(TRANSPARENT);
	for(XU32 i=0;i<m_nCount;i++)
	{
		//::rand()
		int y=5+m_nItemHeight*(i+1);
		pDC->SelectObject(&tpen);
		//pDC->MoveTo(0,y);
		//pDC->LineTo(m_nMaxSamples,y);
		pDC->FrameRect(CRect(0,y-m_nItemHeight,m_nMaxSamples,y),&tb);
		pDC->SetTextColor(0x807070);
		pDC->TextOut(1,y-18,m_strNames[i]);
		CString strTemp;
		strTemp.Format(_T("%3.4f"),m_fMaxTime);
		pDC->TextOut(1,y-m_nItemHeight,strTemp);
		pDC->SelectObject(&pen);
		XU16*pDatas=m_pDatas+i*m_nMaxSamples;
		XBOOL bStart=XFALSE;
		
		
		for(XU32 j=0;j<m_nMaxSamples;j++)
		{
			if(!pDatas[j])
			{
				if(!bStart) continue;
			}
			else if(!bStart)
			{
				pDC->MoveTo(j-1,y);
				bStart=XTRUE;
			}
			pDC->LineTo(j,y-pDatas[j]);
				//continue;

		}
	}
	pDC->SelectObject(pOld);
}

XBOOL CGraphButton::Update()
{
#ifdef _ANALY
   _XCALLGRAPH*pData=_ANGetGraph();
   if(!pData) return XFALSE;
   XU32 nNewCount=0;
   _XCALLGRAPH*p=pData;
   XS64 m_nMaxData=0;
   m_strNames.RemoveAll();
   while(p)
   {
	    nNewCount++;
		if(m_nMaxData<p->nMaxTime)
			m_nMaxData=p->nMaxTime;
		m_strNames.Add(CString(p->strFunction));
		p=p->pNext;
   }
   if(nNewCount!=m_nCount)
   {
	   m_nCount=nNewCount;
	   if(m_nCount)
	   {
		   m_pDatas=new XU16[m_nMaxSamples*m_nCount];
		   if(m_pDatas)
			   memset(m_pDatas,0,m_nMaxSamples*m_nCount*sizeof(XU16));
	   }
   }
   if(!m_nCount||!m_pDatas) return XFALSE;

   m_fMaxTime=(double)m_nMaxData*m_dMulti/1000;

   m_nItemHeight=(m_nHeight-5)/m_nCount-5;
  // if(m_nMaxData>

   p=pData;
   XU16*pDatas=m_pDatas;
   while(p)
   {
	    if(p->nPosition>=m_nMaxSamples)
		{
			XU64*p64=p->pDatas+p->nPosition-m_nMaxSamples;
			for(XU16 i=0;i<m_nMaxSamples;i++)
				pDatas[i]=(XU16)(p64[i]*m_nItemHeight/m_nMaxData);
			//memcpy(pDatas,p->pDatas+p->nPosition-m_nMaxSamples,m_nMaxSamples*sizeof(XU64));
		}
		else
		{
			XU16*p16=pDatas+m_nMaxSamples-p->nPosition;
			for(XU32 i=0;i<p->nPosition;i++)
				p16[i]=(XU16)(p->pDatas[i]*m_nItemHeight/m_nMaxData);
			//memcpy(pDatas+m_nMaxSamples-p->nPosition,p->pDatas,p->nPosition*sizeof(XU64));
		}
	    pDatas+=m_nMaxSamples;
		p=p->pNext;
   }

#endif
	return XTRUE;
}





//DEL BOOL CGraphButton::PreCreateWindow(CREATESTRUCT& cs) 
//DEL {
//DEL 	// TODO: Add your specialized code here and/or call the base class
//DEL 	
//DEL 	return CButton::PreCreateWindow(cs);
//DEL }

void CGraphButton::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	CRect rect;
	GetClientRect(rect);
	m_nMaxSamples=rect.Width();	
	m_nHeight=rect.Height();
	CButton::PreSubclassWindow();
}

BOOL CGraphButton::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	return XTRUE;
	//return CButton::OnEraseBkgnd(pDC);
}
