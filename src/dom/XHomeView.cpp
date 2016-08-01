// XHomeView.cpp: implementation of the XHomeView class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxggdom.h"
#include "XHomeView.h"
#include "XDomCard.h"
#include "XDomP.h"
#include "XDomText.h"
#include "XContentType.h"
#include "XDomInput.h"
#include "XDomImg.h"
#include "XDomAnchor.h"
#include "XDomP.h"
#include "XMainWnd.h"

/////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XHomeView::XHomeView(XGraphics*pDraw,XClientApply*pa):XDomView(pDraw,pa)
{
	m_nSelectID=0;
	m_bHome=XTRUE;
	m_bLoadOK=XFALSE;
	m_days.m_bNoTitle=XTRUE;
	
}

XHomeView::~XHomeView()
{
}

void XHomeView::InitHomePage()
{
	//m_status.ShowWindow(XFALSE);
	//return;
	m_req.nMethod=HOMEPAGE;
	if(!m_pClientApply->m_strBootFile.IsEmpty())
	{
		CONNECTREQ req;
		req.nMethod=XEM::GET;
		req.URL=m_pClientApply->m_strBootFile;
		req.URL.SetURL(m_pClientApply->m_strBootFile);
		XDomView::Connect(req);
#ifdef __ANDROID__
#else
		m_status.ShowWindow(XFALSE);
#endif

		m_bShowLoading = XFALSE;
		return;
	}
   XString8 strTxt;
   XResource::LoadText(strTxt,"HOMEPAGE");
   

   InitDayData(strTxt);
   InitNearData(strTxt);
   InitURLData(strTxt);
   InitMarks(strTxt);
   InitHotData(strTxt);

   m_pDom->Reset(XCHARSET_UTF8,XCNT::CTEXT,XCNT::TVND_WAP_WML,&m_draw);
   m_pDom->SetFilter(NULL);
   m_pDom->PhaseXML((XU8*)strTxt.GetData(),strTxt.GetSize(),false,true);

   LoadImages(true);
   ResetEventData();
   m_pDom->Final(m_req,m_draw,XFALSE,NULL);

   if(m_bLoadOK)
	   m_pDom->AppendDom(&m_days);
   if(m_req.nSelect==0)
		m_pDom->OnKeyDown(SELECTMARK::XNEXT,&m_draw,XTRUE);

   m_bLoaded = XTRUE;

   //this->SetFocus(XTRUE);
   //ResetEventData();
   //SetStatus();

   return;
}

void XHomeView::InitDayData(XString8&strHTML)
{
	m_bLoadOK=XTRUE;
	m_bLoaded=XTRUE;
	SetStatus();
	return;
#ifdef _SYMBIAN
	return;
#endif
	if(m_days.IsFinished())
	{
		m_bLoadOK=XTRUE;
		return;
	}
	if(m_pClientApply->m_strDays.IsEmpty()) return;
	CONNECTREQ req;
	req.bForward=XCT_CONNECT;
	req.nMethod=XEM::GET;
	req.URL.SetURL(m_pClientApply->m_strHost);
	req.URL.SetURL(m_pClientApply->m_strDays);

	m_bLoaded=XFALSE;
	m_pClient=m_pClientApply->CreateClient(req,(XU32)&m_days,0,this);
	if(m_pClient==XNULL) return;
	m_nMoveTimes=0;
	m_bStopreq=XFALSE;
	m_days.SetURL(req.URL);
	m_pClient->Connect(req,XCHARSET_GB2312);

}

void XHomeView::InitHotData(XString8&strHTML)
{

}

void XHomeView::InitURLData(XString8&strHTML)
{
   /*m_pDom->AddText(XString16("Search:"));
   m_pDom->AddBR();
   m_pInput=new XDomInput();
   XDomItem* p=m_pInput;
   XVar v(XEAB::TYPE,XEIT::URL);
   ((XDomInput*)p)->SetText(XString8(m_strInput));
   p->AddAttrib(v);
   v=XVar((int)XEAB::SIZE,(int)((m_view.right-20)/(m_nTextHeight>>1)));
   p->AddAttrib(v);
   v=XVar(XEAB::NAME,"URL");
   p->AddAttrib(v);
   m_pDom->AddTag(p);
   m_pDom->Parent();
   XDomItem* pa=new XDomA();
   v=XVar(XEAB::HREF,"sys@$(URL)");
   pa->AddAttrib(v);
   m_pDom->AddTag(pa);
   XDomImg* pi=new XDomImg();
   v=XVar(XEAB::SRC,"res@gogo");
   pi->AddAttrib(v);
   v=XVar(XEAB::ALT,"GO");
   pi->AddAttrib(v);
   v=XVar(XEAB::VALIGN,XEnumAlign::MIDDLE);
   pi->AddAttrib(v);
   m_pDom->AddTag(pi);
   m_pDom->Parent();
   m_pDom->Parent();
   m_pDom->AddBR();*/
}

void XHomeView::InitNearData(XString8&strHTML)
{
#ifdef _SYMBIAN
	return;
#endif
   int id = strHTML.Find("<!--1>");
	XString8Array list;
	if (id >= 0)
	{
		int ie = strHTML.Find("<1-->");
		if (ie > id)
		{
			XString8 strFmt;
			strHTML.Mid(strFmt, id + 6, ie - id - 6);
			strHTML.Delete(id, ie - id + 5);
			list.SplitString(strFmt, '*', XFALSE);
		}
	}
	if (list.GetSize() < 5)
		return;

	if (m_pClientApply->m_lNearest.GetSize() > 0)
	{
		int nMax = (m_rect.Height() / m_nTextHeight) / 2;
		XString8 strCnt;

		XString8 strNetwork = XResource::LoadWString(XIDS_NETWORK);
		XString8 strLocal = XResource::LoadWString(XIDS_LOCAL);
		int i, nCount = m_pClientApply->m_lNearest.GetSize();
		if (nCount > nMax)
			nCount = nMax;
		for (i = 0; i < nCount; i++)
		{
			BOOKMARKER*pm = m_pClientApply->m_lNearest[i];

			strCnt += list[0];
			int iss = pm->strURL.ReverseFind('.');
			if (iss > 0)
			{
				XString8 strTmp;
				pm->strURL.Mid(strTmp, iss + 1);
				strTmp.MakeLower();
				if (strTmp.Find("swf") == 0)
					strCnt += "RES@FLASH";
				else
					strCnt += "RES@WML";
			}
			else
				strCnt += "RES@WML";
			strCnt += list[1];
			XURL url;
			url.SetURL(pm->strURL);
			if (url.m_nProto >= XPROTO_FILE)
				strCnt += strLocal;
			else
				strCnt += strNetwork;
			strCnt += list[2];
			strCnt += pm->strURL;
			strCnt += list[3];

			if (pm->strTitle.GetLength() < 10)
				strCnt += pm->strTitle;
			else
			{
				XString16 strTmp(pm->strTitle, 10);
				strCnt += strTmp;
				strCnt += "...";
			}
			strCnt += list[4];
		}
		strHTML.Insert(id, strCnt);
	}
	else
	{
#ifdef __APPLE__
		XString8 strCnt="<div style=\"color:#777;font-size:16;padding:4\">";
#else
		XString8 strCnt = "<div style=\"color:#777;font-size:12;padding:4\">";
#endif
		strCnt += XResource::LoadWString(XIDS_NOCONTEXT);
		strCnt += "</div>";
		strHTML.Insert(id, strCnt);
	}
}

void XHomeView::InitMarks(XString8&strHTML)
{
#ifdef _SYMBIAN
	return;
#endif
   int id=strHTML.Find("<!--2>");
   XString8Array list,list1;
   XString8 strPad;
   if(id>=0)
   {
	   int ie=strHTML.Find("<2-->");
	   if(ie>id)
	   {
		 XString8 strFmt;
		 strHTML.Mid(strFmt,id+6,ie-id-6);
		 strHTML.Delete(id,ie-id+5);

		 int id1=strFmt.Find("<!--3>");
		 if(id1>=0)
		  {
			   //strHTML.Delete(id,6);
			   int ie1=strFmt.Find("<3-->");
			   if(ie1>id1)
			   {
				 XString8 strFmt1;
				 strFmt.Mid(strFmt1,id1+6,ie1-id1-6);
				 strFmt.Delete(id1,ie-id+5);
				 list1.SplitString(strFmt1,'*',XFALSE);
			   }
		   }

		 list.SplitString(strFmt,'*',XFALSE);

	   }
   }
   if(list.GetSize()<2) return;

   if(list1.GetSize()<3) return;

   XString8 strCnt;
   m_pDom->AddTag(new XDomP());
   for(XU32 i=0;i<m_pClientApply->m_marks.GetSize();i++)
   {
	  MARKLIST&ls=m_pClientApply->m_marks[i]->lMarks;
	  if(ls.GetSize()<=0) continue;
	  strCnt+=list[0];
	  strCnt+=m_pClientApply->m_marks[i]->strClass;
	  strCnt+=list[1];

	  for(XU32 j=0;j<ls.GetSize();j++)
	  {
		  strCnt+=list1[0];
		  strCnt+=ls[j]->strURL;
		  strCnt+=list1[1];
		  strCnt+=ls[j]->strTitle;
		  strCnt+=list1[2];
	  }
	  if(list.GetSize()>2)
		  strCnt+=list[2];

   }
   strHTML.Insert(id,strCnt);
}

XBOOL XHomeView::Connect(CONNECTREQ &req,XBOOL bSet)
{
	if(req.nMethod==HOMEPAGE)
	{
		if(!m_pClientApply->m_strBootFile.IsEmpty())
		{
			m_bHome=XTRUE;
			m_req.nMethod=HOMEPAGE;
			m_pClientApply->SetModify(XFALSE);
			req.URL=m_pClientApply->m_strBootFile;
			req.URL.SetURL(m_pClientApply->m_strBootFile);
			XDomView::Connect(req,bSet);
#ifdef __ANDROID__
#else
			m_status.ShowWindow(XFALSE);
#endif
			m_bShowLoading = XFALSE;
			return XTRUE;
		}
#ifdef __ANDROID__
#else
		m_status.ShowWindow(XTRUE);
#endif
		ToDomView();
		m_bHome=XTRUE;
		m_pClientApply->SetModify(XFALSE);
		InitHomePage();
		XKEYMSG msg={XK_DOWN,XK_DOWN,0,false,false,false};
		OnKeyDown(msg);
		Invalidate();
		SetTitle(XResource::LoadWString(XIDS_EMPTY));
		return XTRUE;
	}

#if defined( __APPLE__ ) || defined( __ANDROID__ ) // modify by hgl 2011-09-27
#else
	if(m_bHome)
		m_saveReq.nMethod=HOMEPAGE;
#endif
//#if defined(_OPEN_WEB_HOME_) || !defined(__APPLE__) || !defined( __ANDROID__ )
//	if(m_bHome)
//		m_saveReq.nMethod=HOMEPAGE;
//#endif

#ifdef __ANDROID__
#else
	if(!m_status.IsVisiable())
		m_status.ShowWindow(XTRUE);
#endif

	//XMainWnd*p=(XMainWnd*)GetParent();
	return XDomView::Connect(req,bSet);
}

static XMENUDATA _homeMenus[]=
{
	{XCMD_DOM_HOME,0,XResource::LoadWString(XIDS_HOMEPAGE),XNULL,0,0},	//0
};

#ifdef __ANDROID__
#else
void XHomeView::AppendMenu(XMenu &menu)
{
	if(!m_bHome)
	{
		if(!m_pFlashView)
			menu.AddMenu(&_homeMenus[0]);

	}
	//XDomView::AppendMenu(menu);
}
#endif

XBOOL XHomeView::OnCommand(XU32 nCmd, XWindow *pWnd)
{
	switch(nCmd)
	{
	case XCMD_DOM_HOME:
#ifdef __ANDROID__
		{
			CONNECTREQ req;
			req.bForward=true;
			req.nMethod=XEnumMethod::GET;
			req.URL.SetURL("http://m.jqbar.com/product/");
			Connect(req, true);
			m_bHome=XTRUE;
			m_req.nMethod=HOMEPAGE;
		}
#else
		 SaveREQ();
		 PushREQ(XFALSE);
		 m_req.bForward=XCT_CONNECT;
		 m_bHome=XTRUE;
		 m_pClientApply->SetModify(XFALSE);
		 InitHomePage();
		 Invalidate();
		 SetTitle(XResource::LoadWString(XIDS_EMPTY));
		 SetOffset(0,0,false);
		 SetStatus();
		 Invalidate();
#endif
		 break;
	case XCMD_VIEW_CLOSE:return XTRUE;
	}
	return XDomView::OnCommand(nCmd,pWnd);
}

void XHomeView::OnFocus()
{

#if defined( __APPLE__ ) || defined( __ANDROID__ ) // modify by hgl 2011-09-27
	m_req.nMethod = HOMEPAGE;
	XDomView::OnFocus();
	return;
#endif

//#if defined(_OPEN_WEB_HOME_) || !defined(__APPLE__)  || !defined( __ANDROID__ )
//#else
//	m_req.nMethod = HOMEPAGE;
//	XDomView::OnFocus();
//	return;
//#endif
	if(m_bHome&&m_pClientApply->IsModified())
	{
		m_pClientApply->SetModify(XFALSE);
		InitHomePage();
		Invalidate();
	}
	XDomView::OnFocus();
}


void XHomeView::OnCreate()
{
	m_pClientApply->ResetConfig(XTRUE,&m_draw);
	m_pClientApply->ResetConfig(XFALSE,&m_draw);
}


XBOOL XHomeView::ProcSysURL(CONNECTREQ &req)
{
    int id=req.URL.m_strFile.Find(':');
	XString8 strCmd,strArg;
	if(id>=0)
	{
		req.URL.m_strFile.Left(strCmd,id);
		req.URL.m_strFile.Mid(strArg,id+1);
	}
	else
		strCmd=req.URL.m_strFile;

	strCmd.MakeLower();

	if(strCmd=="remove all click")
	{
		m_pClientApply->RemoveAllClick();
		m_pClientApply->SetModify(XFALSE);
		InitHomePage();
		Invalidate();
	}
	else
	{
		return XDomView::ProcSysURL(req);
	}
	return XFALSE;
}

XBOOL XHomeView::HandleSocket(XU32 nCmd, XClient *pClient)
{
	XDom*pDom=XNULL;
	XBOOL bDays=XFALSE;
	if(pClient==m_pClient && pClient->Get_CallID()==(XU32)&m_days)
		bDays=XTRUE;
	else if(!m_pClient&&IsImageClient(pClient))//pClient->GetCallID()!=-1)
	{
		XIMAGEDATA*pData=(XIMAGEDATA*)pClient->Get_CallID();
		if(pData->pDom==&m_days)
			bDays=XTRUE;
	}
	if(bDays)
	{
		pDom=m_pDom;
		m_pDom=&m_days;
		m_bBackList=XFALSE;
	}

	XBOOL bOK=XDomView::HandleSocket(nCmd,pClient);
	if(pDom)
	{
		m_pDom=pDom;
		m_bBackList=XTRUE;
		if(m_days.IsFinished())
		{
			m_days.UpdateCSS();
			m_pDom->AppendDom(&m_days);
			Invalidate();
			//int v=0;
		}
	}
	return bOK;
}

XBOOL XHomeView::OnPaint(XGraphics &g)
{
	return XDomView::OnPaint(g);
}
