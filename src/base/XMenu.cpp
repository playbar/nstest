// XMenu.cpp: implementation of the XMenu class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxGGBase.h"
#include "XMenu.h"

#ifdef __ANDROID__
#else
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//#define _WINMODE
XMenu::XMenu()
{
	m_nCount=0;
	m_nOffset=0;
	m_nSelect=0;
	m_nImageWidth=0;
	m_pRootMenu=XNULL;
	m_pSubMenu=XNULL;
//	m_pImage=XNULL;
//	m_pSelectImage=XNULL;
}

XMenu::~XMenu()
{
	if(m_pSubMenu!=XNULL) delete m_pSubMenu;
	m_pSubMenu=XNULL;
}

XBOOL XMenu::Create(XPoint &pt,XU8 nStyle,XWindow*pWnd,XMenu*pRoot)
{
   if(!m_rect.IsRectEmpty()) return XFALSE;
//   m_pImage=GetIcon(XICON_BLUEBLOCK);
 //  m_pSelectImage=GetIcon(XICON_GREENBLOCK);
   m_pRootMenu=pRoot;
   m_nOffset=m_nSelect=0;
   m_nMove=0;
   m_nTick=0;
   XRect r;
   pWnd->GetClientRect(r);
   int dy=m_nTextHeight+(m_nTextHeight>>2);
//   if(m_pImage&&m_pSelectImage)
//	   dy=m_pImage->Height();
   //m_m_menus.Copy(m_menus);
   m_nImageWidth=0;
   XU32 cx=0,cix=12,cy=4;
   //XBitmap bmp(1,1);
   XGraphics g(1,1);//bmp);// g(bmp);
   g.SetFontSize(m_nTextSize);
   m_nCount=m_menus.GetSize();
   for(XU32 i=0;i<m_menus.GetSize();i++)
   {
	  if(m_menus[i]->pImage!=XNULL)
	  	  cix=XMAX(cix,m_menus[i]->pImage->Width()+1);
	  if(m_menus[i]->strTitle!=XNULL&&
		 (m_menus[i]->nStyle&XMS_SEPARATOR)==0)
	  {
		  XSize sz;
		  g.MeasureString(m_menus[i]->strTitle,sz);
		  cx=XMAX(cx,(XU32)sz.cx);
	  }
	  if((m_menus[i]->nStyle&XMS_SEPARATOR)==0)
	  {
		 cy+=dy;
		 //m_nCount++;
	  }
	  else cy+=2;
	  //cy+=2;
   }
   m_nImageWidth=(XU8)cix;
   cx+=cix+12;
   XU32 my=r.Height();
   switch(nStyle&0xf0)
   {
   case XMPS_BOTTOM:
	    my=pt.y;break;
   case XMPS_HCENTER:
	    my=XMIN((XINT)my,(pt.y<<1));break;
   case XMPS_TOP:
	    my=my-pt.y;
		break;
   }
   if(cy>my) cy+=10;
   m_nCount=m_menus.GetSize();
   while(m_nCount>2&&cy>my)
   {
	   m_nCount--;
	   cy-=dy;
   }
   //int cy=m_nCount*m_nTextHeight+6;
   //if(m_nCount<m_menus.GetSize()) cy+=10;
   
   switch(nStyle&0xf)
   {
   case XMPS_VCENTER:
	    pt.x-=cx>>1;
		break;
   case XMPS_RIGHT:
	    pt.x-=cx;
		break;
   case XMPS_WLEFT:pt.x=0;break;
   case XMPS_WRIGHT:pt.x=r.right-cx;break;
   case XMPS_WVCENTER:pt.x=(r.right-cx)>>1;break;
   }
   pt.x=XMAX(0,pt.x);
   //pt.x=XMIN((int)(r.right-cx),pt.x);

   switch(nStyle&0xf0)
   {
	case XMPS_BOTTOM:
		 pt.y-=cy;
		 break;
	case XMPS_HCENTER:
		 pt.y-=cy>>1;
		 break;
	case XMPS_WTOP:pt.y=0;break;
	case XMPS_WBOTTOM:pt.y=r.bottom-cy;break;
	case XMPS_WHCENTER:pt.y=(r.bottom-cy)>>1;break;
   }
   pt.y=XMAX(0,pt.y);
   //pt.y=XMIN((int)(r.bottom-cy),pt.y);
   return XWindow::Create(XRect(pt.x,pt.y,pt.x+cx,pt.y+cy),XES_VISIABLE|XES_POPUP,pWnd);
   //return XTRUE;
}

#define _NO3D

XBOOL XMenu::OnPaint(XGraphics &g)
{
   //XU32 c=((GetSysColor(XCW_BUTTON)&0xffffff))|0xd0000000;
   XColor c(GetSysColor(XCW_BUTTON));
   c.Alpha(0xe0);
   XRect r;
   GetClientRect(r);
   int dy=m_nTextHeight+(m_nTextHeight>>2);
//   if(m_pImage&&m_pSelectImage)
//	   dy=m_pImage->Height();
   //if(m_pImage==XNULL)
//   else
   if(GetIcon(XICON_BUTTON)!=XNULL)
	   DrawButton(g,r,c,XBS_BFRAME);
   else
   {
#ifdef _NO3D
   g.SetBackColor(0xffffffff);
   g.FillRectangle(r);
   g.SetColor(0xff000000);
   g.FrameRectangle(r);
#else
   g.DrawButton(r,c,XBS_BFRAME);
#endif
   }
   XImage*pImage=GetIcon(XICON_GREENBLOCK);
   int by=5,bx=m_nImageWidth+2;
   if(m_nCount<m_menus.GetSize()) by+=5; 
   for(XU8 i=m_nOffset;i<m_nOffset+m_nCount;i++)
   {
	  if(m_menus[i]->nStyle&XMS_SEPARATOR)
	  {
		  XColor hc(c);
		  hc.Bright(50);
		  g.SetColor(hc);
		  g.DrawLine(r.left+5,by-1,r.right-4,by-1);
		  hc=c;
		  hc.Dark(24);
		  g.SetColor(hc);
		  g.DrawLine(r.left+4,by-2,r.right-5,by-2);
		  by+=2;
		  //continue;
	  }
	  else
	  {	
		  XColor bc=c; 		 
		  if(i==m_nSelect)
		  {
			if(pImage==XNULL)
			{
			 bc=GetSysColor(XCW_SELECTBACK);
			 g.SetBackColor(bc);
			//g.FillRectangle(XRect(r.left+3,by-1,r.right-3,by+m_nTextHeight));
			 g.DrawBar(XRect(r.left+3,by-2,r.right-3,by+m_nTextHeight+1),bc,4);
			}
			else
			{
				int x,w=pImage->Width();
				XRect rr(0,2,w,pImage->Height()-2);
				for(x=r.left+2;x<r.right-2;x+=w)
				{
					XPoint dtt(x,by-2);
					if(x+rr.right>r.right-2)
						rr.right=r.right-2-x;
					g.DrawImage(*pImage,dtt,rr);
				}
			}
		  }
		  XColor cc(i==m_nSelect?(pImage?0xffffffff:GetSysColor(XCW_SELECTTEXT)):GetSysColor(XCW_TEXT));
		  //XU32 dcc=i==nSelect?0xffffffff;
		  if(m_menus[i]->nStyle&XMS_GRAY)
			  cc.Mix(bc,0.4f);
		  
			  //cc.Bright(30);
		  if(m_menus[i]->nStyle&XMS_CHECKED)
		  {
			  int dd=XMIN(8,m_nTextHeight);
			  int dm=((m_nTextHeight-dd)>>1)-1;
			  bc.Mix(cc,0.7f);
			  g.DrawCheck(XRect(bx-dd,by+dm,bx,by+dd+dm),cc,bc);
		  }
		  else if(m_menus[i]->pImage!=XNULL)
		  {
			XImage*p=m_menus[i]->pImage;
			int dd=(m_nTextHeight-p->Height())>>1;
			p->SetActive(m_menus[i]->nIndex);
			g.DrawImage(*p,XPoint(bx-p->Width(),by+dd),
						XRect(0,0,p->Width(),p->Height()));

		  }
		  g.SetColor((i==m_nSelect&&pImage)?0xff2756ad:0xffffffff);
		  g.DrawString(m_menus[i]->strTitle,bx+1,by+2);
		  g.SetColor(cc);		  
		  g.DrawString(m_menus[i]->strTitle,bx,by+1);
		  if(m_menus[i]->nStyle&XMS_POPUP)
		  {
			  int dm=XMIN(12,m_nTextHeight);
			  int dd=(m_nTextHeight-dm)>>1;
			  g.DrawDirect(XRect(r.right-dm-2,by+dd,r.right-2,by+dd+dm),XK_RIGHT,cc);
		  }
		  by+=dy;//m_nTextHeight;
		  //by+=2;
	  }	  
   }
   XColor cc(c);
   cc.Dark(60);
   int cx=(r.right+r.left)>>1;
   if(m_nOffset>0)
   {
	  
	  g.DrawDirect(XRect(cx-6,r.top+2,cx+6,r.top+12),XK_UP,cc);
   }
   if(m_nOffset+m_nCount<(XINT)m_menus.GetSize())
   {
	  g.DrawDirect(XRect(cx-6,r.bottom-10,cx+6,r.bottom+1),XK_DOWN,cc);
   }
   //g.SetColor(0xff000000);
   //g.FrameRectangle(r);
   return XTRUE;
}

void XMenu::OnMouseDown(XPoint &pt,int iPos)
{
   if(iPos) return;
   XRect r;
   GetClientRect(r);
   if(!r.PtInRect(pt))
   {
#ifdef _WINMODE
	   CloseMenu(XTRUE);
#endif
	   return;
   }
   else
   {
	   int v=HitTest(pt);
	   if(v>=0)
	   {
		   if(m_menus[v]->nStyle&XMS_GRAY) return;
		   else if(m_menus[m_nSelect]->nStyle&XMS_POPUP)
			{
#ifdef _WINMODE
				PopMenu();
#endif
				return;
			}
#ifdef _WINMODE
		   PostMsg(XWM_COMMAND,m_menus[v]->nCommand,m_menus[v]->nData,XMS_NORMAL);
		   CloseMenu(XTRUE);
#endif
	   }
   }
}

void XMenu::OnMouseUp(XPoint &pt,int iPos)
{
   if(iPos) return;
   XRect r;
   GetClientRect(r);
   if(!r.PtInRect(pt))
   {
	   CloseMenu(XTRUE);
	   return;
   }
   else
   {
	   int v=HitTest(pt);
	   if(v>=0)
	   {
		   if(m_menus[v]->nStyle&XMS_GRAY) return;
		   else if(m_menus[m_nSelect]->nStyle&XMS_POPUP)
			{
#ifdef _WINMODE
#else
				PopMenu();
#endif
				return;
			}
		   PostMsg(XWM_COMMAND,m_menus[v]->nCommand,m_menus[v]->nData,XMS_NORMAL);
		   CloseMenu(XTRUE);
	   }
   }
}

XBOOL XMenu::OnKeyDown(XKEYMSG&m)
{
   m_nMove=0;
   switch(m.nKeyCode)
   {
   case XK_LEFT:
	    if(m_pRootMenu!=XNULL)
			CloseMenu(XFALSE);
	    break;
   case XK_UP:
	    Select(-1);
		break;
   case XK_DOWN:
	    Select(1);
		break;
   case XK_RIGHT:
	    PopMenu();
	    break;
   case XK_RETURN:
	    if(m_menus[m_nSelect]->nStyle&XMS_GRAY) break;
		else if(m_menus[m_nSelect]->nStyle&XMS_POPUP)
		{
			PopMenu();
			break;
		}
	    PostMsg(XWM_COMMAND,m_menus[m_nSelect]->nCommand,m_menus[m_nSelect]->nData,XMS_NORMAL);
	    CloseMenu(XTRUE);
	    break;
   //case XK_MENU:
   case XK_LEFTSOFT:
   case XK_RIGHTSOFT:
   case XK_CLOSE:
   case XK_CLEAR:
	    CloseMenu(XTRUE);
		break;
   }
   return XTRUE;
}

void XMenu::OnMouseMove(XPoint &pt,int iPos)
{
	if(iPos) return;
    int v=HitTest(pt);
	if(v<-2) 
	{
		if(m_pRootMenu)
		{
			XPoint pt1=pt;
			pt1+=m_rect.TopLeft();
			pt1-=m_pRootMenu->m_rect.TopLeft();
			m_pRootMenu->OnMouseMove(pt1,iPos);
		}
		return;
	}
	if(v>=0) m_nMove=0;
	if(m_nSelect!=v)
	{		
		//if(v>=-2) 
		CloseSubMenu();
		if(v>=0)
		{		
			m_nSelect=v;
#ifdef _WINMODE
			PopMenu();
#endif
			Invalidate();
		}
		else if(v>=-2&&m_nMove==0)
		{
			//CloseSubMenu();
		    m_nTick=XW_WAITTIME;
			m_nMove=(XU8)-v;
			MoveStep();
		}
	}
}

void XMenu::CloseMenu(XBOOL bClearAll)
{
   m_menus.RemoveAll();
   PostDestory();
   if(bClearAll)
   {
	   if(m_pRootMenu!=XNULL)
	   {
		   m_pRootMenu->CloseMenu(XTRUE);
		   m_pRootMenu=XNULL;
	   }
   }
   m_nCount=0;
   m_pRootMenu=XNULL;
}

void XMenu::Select(int d)
{
   int v=m_nSelect;
Again:
   v+=d;
   if(v<0) v=m_menus.GetSize()-1;
   else if(v>=(int)m_menus.GetSize()) v=0;
   if(m_menus[v]->nStyle&XMS_SEPARATOR) goto Again;
   if(m_nSelect==v) return;	
   if(m_nOffset>v) m_nOffset=v;
   else if(v-m_nOffset+1>=m_nCount)
	   m_nOffset=v-m_nCount+1;
   m_nSelect=v;
   Invalidate();
}

int XMenu::HitTest(XPoint &pt)
{
	XRect r;
    GetClientRect(r);
    if(!r.PtInRect(pt)) return -100;
	int dy=m_nTextHeight+(m_nTextHeight>>2);
    int by=3,bx=m_nImageWidth+2;
    if(m_nCount<m_menus.GetSize()) by+=5;
    for(XU8 i=m_nOffset;i<m_nOffset+m_nCount;i++)
    {
	  if(m_menus[i]->nStyle&XMS_SEPARATOR)
	  {
		  by+=2;
	  }
	  else
	  {	
			XRect rt(0,by,r.right,by+m_nTextHeight);
			if(rt.PtInRect(pt))
			{
				//m_nSelect=i;
				//Invalidate();
				return i;
			}
		    by+=dy;//m_nTextHeight;
			//by+=2;
	  }
    }
	if(m_nOffset>0&&pt.y<8) return -1;
	if(m_nOffset+m_nCount<(XINT)m_menus.GetSize()&&pt.y>r.bottom-8) return -2;
	return -100;
}

void XMenu::OnTimer()
{
	if(m_nMove<=0) return;
	if(m_nTick>0) m_nTick--;
	else
	{
		MoveStep();
		m_nTick=XW_REPTIME;
	}
}

void XMenu::MoveStep()
{
    switch(m_nMove)
	{
	case 1:
		 if(m_nOffset>0) 
		 {
			 m_nOffset--;
			 if(m_nSelect-m_nOffset+1>m_nCount)
				 m_nSelect=m_nCount+m_nOffset-1;
			 Invalidate();
		 }
		 else m_nMove=0;
		 break;
	case 2:
		if(m_nOffset+m_nCount<(XINT)m_menus.GetSize())
		{
			m_nOffset++;
			if(m_nSelect<m_nOffset) m_nSelect=m_nOffset;
			Invalidate();
		}
		else m_nMove=0;
	}
}

void XMenu::PopMenu()
{
	int i;
	if(!(m_menus[m_nSelect]->nStyle&XMS_POPUP)) return;
	MENUDATAS* p=(MENUDATAS*)m_menus[m_nSelect]->nData;
	if(p==XNULL||p->GetSize()<1) return;
	if(m_pSubMenu==XNULL)
		m_pSubMenu=new XMenu();
	XRect r;
    GetClientRect(r);
    int by=3;
    if(m_nCount<m_menus.GetSize()) by+=5;
    for(i=m_nOffset;i<m_nOffset+m_nCount;i++)
    {
	  if(i==m_nSelect) break;
	  if(m_menus[i]->nStyle&XMS_SEPARATOR)
	    by+=2;
	  else
	    by+=m_nTextHeight;	  	  
    }
	XU8 s=XMPS_TOP;
	XPoint pt(r.right,by);
	pt+=m_rect.TopLeft();
	XRect rp;
	m_pParent->GetClientRect(rp);
	if(rp.bottom-pt.y<pt.y+m_nTextHeight)
	{
		pt.y+=m_nTextHeight;
		s=XMPS_BOTTOM;
	}
	if(rp.right-pt.x<m_rect.left)
	{
		pt.x=m_rect.left;
		s|=XMPS_RIGHT;
	}
	else s|=XMPS_LEFT;
	
	m_pSubMenu->RemoveMenus();
	for(i=0;i<(int)p->GetSize();i++)
		m_pSubMenu->AddMenu((*p)[i]);
	
	m_pSubMenu->Create(pt,s,m_pParent,this);
	m_pSubMenu->Invalidate();
}

XBOOL XMenu::CloseSubMenu()
{
	if(m_pSubMenu==XNULL) return XFALSE;
	if(m_pSubMenu->m_menus.GetSize()<=0) return XFALSE;
	m_pSubMenu->CloseSubMenu();
	m_pSubMenu->CloseMenu(XFALSE);
    
	return XTRUE;
}

static XMENUDATA _separator={0,0,XNULL,XNULL,0,XMS_SEPARATOR};

void XMenu::AddSeparator()
{
	AddMenu(&_separator);
}

#endif
