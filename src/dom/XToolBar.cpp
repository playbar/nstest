// XToolBar.cpp: implementation of the XToolBar class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfxggdom.h"
#include "XToolBar.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#ifdef __ANDROID__
#else

#if defined(__TOUCHPAD__)||defined(__POINT__)

#define SLIDER_WIDTH 6

XToolBar::XToolBar()
{
	m_nMvStatus=XTOOLBAR_MV_MOVE;
	m_pBack=0;
	m_nDownID=-1;
	m_nItemCount=0;
	m_nItemLeft=0;
	m_nItemRight=0;
	m_nPageCount=0;
	m_nOffset=0;
	m_nItemWidth=0;
	m_nShowBegin=0;
	m_nActiveID=-1;
	m_bDown=XFALSE;
	m_nStep=0;
	m_bIsDown=XFALSE;
	m_nSubID=0;
	//m_pBar=NULL;
}

XToolBar::~XToolBar()
{

}


XBOOL XToolBar::OnPaint(XGraphics &g)
{   
	//return XTRUE;
   if(m_pBack==XNULL) return XTRUE;
   //XRect rect=m_clip;
   //int mx,my;
   //g.GetOrigin(mx,my);
   //g.SetOrigin(mx+rect.left,my+rect.top);
  // rect-=rect.TopLeft();
   int x,w=m_pBack->Width();
   XRect rect(0,0,w,m_pBack->Height());
   //rect.right=m_pBack->Width();
   for(x=0;x<m_clip.right;x+=w)
   {
	   if(x+w<m_clip.left) continue;
	   XPoint dt(x,0);
	   if(rect.right+x>m_clip.right)
		   rect.right=m_clip.right-x;
		   //rect.right=m_nWidth-x;
	   g.DrawImage(*m_pBack,dt,rect);
   }


   int dw=m_nItemWidth,ddh=m_pBack->Height();
   int il=0,ir=1,im=0,llx=m_nItemLeft*dw,rrx=m_rect.Width()-m_nItemRight*dw;

   for(int i=0;i<m_nItemCount;i++)
	{
		//int ix=CommandToPos(iCommand);
	    XRect irect;
		XPoint dt(0,0);
		XImage*pImage=&m_normal;
		switch(m_items[i].iStatus)
		{
		case XTS_NORMAL:
			 pImage=&m_normal;break;
		case XTS_DISABLE:
			 if(!m_gray.IsEmpty())
				pImage=&m_gray;
			 break;
		//case XTS_CHECKED:
		//	 pImage=&m_check;break;
		case XTS_ACTIVE:
			 if(!m_active.IsEmpty())
			 pImage=&m_active;break;
		}
		if(m_items[i].iImage==TICON_SLIDER)
		{			
			im+=SLIDER_WIDTH;
			XImage*pImageBut=pImage;
			if(m_items[i].iPos<=0&&!m_gray.IsEmpty())
				pImageBut=&m_gray;
			else if(m_items[i].iStatus==XTS_ACTIVE1&&!m_active.IsEmpty())
				pImageBut=&m_active;
			if(pImageBut&&!pImageBut->IsEmpty())
			{
				int dh=pImage->Height();
				dt.y=(ddh-dh+1)>>1;
				dt.x=(im-SLIDER_WIDTH+1)*dw+dt.y+12;
				irect.left=TICON_ZOOMIN*dh;
				irect.right=irect.left+dh;
				irect.top=0;
				irect.bottom=dh;
				XRect rtt(dt.x,dt.y,dt.x+irect.Width(),dt.y+irect.Height());
				dt.x+=(m_nItemWidth-ddh)>>1;
				rtt&=m_clip;
				if(!rtt.IsRectEmpty())
					g.DrawImage(*pImageBut,dt,irect);
			}
			pImageBut=pImage;
			if(m_items[i].iPos>=100&&!m_gray.IsEmpty())
				pImageBut=&m_gray;
			else if(m_items[i].iStatus==XTS_ACTIVE2&&!m_active.IsEmpty())
				pImageBut=&m_active;
			if(pImageBut&&!pImageBut->IsEmpty())
			{
				int dh=pImage->Height();
				dt.x=(im)*dw+dt.y;
				irect.left=TICON_ZOOMOUT*dh;
				irect.right=irect.left+dh;
				irect.top=0;
				irect.bottom=dh;
				XRect rtt(dt.x,dt.y,dt.x+irect.Width(),dt.y+irect.Height());
				dt.x+=(m_nItemWidth-ddh)>>1;
				rtt&=m_clip;
				if(!rtt.IsRectEmpty())
					g.DrawImage(*pImageBut,dt,irect);
			}
			int bx=(im-SLIDER_WIDTH+2)*dw+16;
			int ex=(im)*dw-12;
			int mh=ddh/2-1;
			g.SetColor(0xff3B65B5);
			g.DrawLine(bx,mh,ex,mh);mh++;
			g.SetColor(0xff000000);
			g.DrawLine(bx,mh,ex,mh);mh++;
			g.SetColor(0xff86ABE5);
			g.DrawLine(bx,mh,ex,mh);mh++;
			g.SetColor(0xff5A89D8);
			g.DrawLine(bx,mh,ex,mh);
			if(!m_bar.IsEmpty())
			{
				int x=(ex-bx)*m_items[i].iPos/100+bx;
				dt.x=x-m_bar.Width()/2;
				dt.y=mh-m_bar.Height()/2-1;
				g.DrawImage(m_bar,dt);
			}
			continue;
		}
		//int mx=0;
		
		if(m_items[i].bChecked)
			pImage=&m_check;
		if(m_items[i].bChecked&&!m_check.IsEmpty())
			pImage=&m_check;
		if(pImage&&!pImage->IsEmpty())
		{
			int dh=pImage->Height();
			dt.y=(ddh-dh+1)>>1;
			irect.left=m_items[i].iImage*dh;
			irect.right=irect.left+dh;
			irect.top=0;
			irect.bottom=dh;
		}
		switch(m_items[i].iPos)
		{
		case XTP_LEFT:
			 dt.x=dw*il+dt.y;
			 il++;
			 break;
		case XTP_RIGHT:
			 dt.x=m_rect.Width()-dw*ir+dt.y;
			 ir++;
			 break;
		default:
			 if(im>=m_nShowBegin+m_nPageCount)
			 {
				 pImage=XNULL;
				 break;
			 }
			 dt.x=llx+im*dw-m_nOffset+dt.y;
			 im++;
			 if(dt.x+irect.Width()>rrx)
			 {
				 irect.right=rrx-dt.x+irect.left;
			 }
			 if(dt.x<llx)
			 {
				 irect.left+=llx-dt.x;
				 dt.x=llx;
			 }
			 if(irect.left>=irect.right)
				 pImage=XNULL;
			 break;
		}
		if(pImage&&!m_items[i].bHide)
		{

			XRect rtt(dt.x,dt.y,dt.x+irect.Width(),dt.y+irect.Height());
			dt.x+=(m_nItemWidth-ddh)>>1;
			rtt&=m_clip;
			if(!rtt.IsRectEmpty())
				g.DrawImage(*pImage,dt,irect);
			/*if(m_items[i].bChecked)
			{
				g.SetColor(0xff1f0000);
				int dw=irect.Width();
				//int ddw=dw/2;
				g.DrawLine(dt.x+dw-4,dt.y+4,dt.x+4,dt.y+dw-4);
			}*/
		}
	
   }
   return XTRUE;
}

XBOOL XToolBar::Create(XWindow*pParent,int y,int w,XBOOL bHide,XU32 nBackID)
{
	m_pBack=XWindow::GetIcon(nBackID);
	if(m_pBack==XNULL) return XFALSE;
	m_nShowBegin=0;
	//m_pWnd=pParent;
	if(bHide)
		AddItem(XCMD_HIDE_TOOLBAR,1,XTS_NORMAL,XTP_LEFT);
	XRect rect(0,y-m_pBack->Height(),w,y);
	m_nItemWidth=m_pBack->Height();
//#ifdef __TOUCHPAD__
//	m_nItemWidth+=m_nItemWidth>>1;
//#endif
	//m_nWidth=w;
	//m_nHeight=m_pBack->Height();
	//m_nOffsetY=y-m_nHeight;
	//m_nButtonSize=bs;
	//m_pBack=XWindow::GetIcon(XICON_GREENBLOCK);
	//m_pBack=
	return XWindow::Create(rect,XES_VISIABLE,pParent); 
		//XTRUE;
}

void XToolBar::SetImage(XPCTSTR iconNormal,XPCTSTR iconGray,XPCTSTR iconActive,XPCTSTR iconCheck)
{
	if(!iconNormal) return;
	//XResource::LoadImage(&m_normal,iconNormal,XFALSE);
	m_normal.CreateFromResource(iconNormal,XFALSE);
	if(iconGray)
		m_gray.CreateFromResource(iconGray,XFALSE);
	if(iconActive)
		m_active.CreateFromResource(iconActive,XFALSE);
	if(iconCheck)
		m_check.CreateFromResource(iconCheck,XFALSE);
		////XResource::LoadImage(&m_check,iconCheck,XFALSE);
}

XBOOL XToolBar::AddItem(XU32 iCommand, int iImage, int iStatus,int iPos,XBOOL bCheck)
{
	if(iImage==TICON_SLIDER)
	{
		if(m_bar.IsEmpty())
			m_bar.CreateFromResource("SLIDERBAR");
	}
	for(int i=0;i<m_nItemCount;i++)
	{
		if(m_items[i].iCommand==iCommand)
		{			
			m_items[i].iImage=iImage;
			m_items[i].iPos=iPos;
			m_items[i].iStatus=iStatus;
			m_items[i].iSlider=0;
			/*switch(iPos)
			{
				case XTP_LEFT:m_nItemLeft++;break;
				case XTP_RIGHT:m_nItemRight++;break;
			}*/
			//RemoveItem(XCMD_MOVE_TOOLBAR);
			return XTRUE;
		}
	}
	if(m_nItemCount>=MAX_ITEMS) return XFALSE;
	switch(iPos)
	{
		case XTP_LEFT:m_nItemLeft++;break;
		case XTP_RIGHT:m_nItemRight++;break;
	}
	m_items[m_nItemCount].iCommand=iCommand;
	m_items[m_nItemCount].iImage=iImage;
	m_items[m_nItemCount].iPos=iPos;
	m_items[m_nItemCount].iSlider=0;
	m_items[m_nItemCount].iStatus=iStatus;
	m_items[m_nItemCount].bChecked=bCheck;
	m_items[m_nItemCount].bHide=0;
	m_nItemCount++;
	return XTRUE;
}

void XToolBar::RemoveAllItems()
{
	m_nItemLeft=1;
	m_nItemRight=0;
	m_nItemCount=1;
}

XBOOL XToolBar::RemoveItem(XU32 iCommand)
{
	for(int i=0;i<m_nItemCount;i++)
	{
		if(m_items[i].iCommand==iCommand)
		{
			XGlobal::Memcpy(&m_items[i],&m_items[i+1],(m_nItemCount-i-1)*sizeof(XTOOLITEM));
			m_nItemCount--;
			if(m_items[i].iImage==TICON_SLIDER)
				m_nItemLeft--;
			else
			{
				switch(m_items[i].iPos)
				{
				case XTP_LEFT:m_nItemLeft--;break;
				case XTP_RIGHT:m_nItemRight--;break;
				}
			}
			return XTRUE;
		}
		//return XFALSE;
	}
	return XFALSE;
}


void XToolBar::OnMove(XRect &rect)
{
	m_nShowBegin=0;
	for(int i=0;i<m_nItemCount;i++)
		if(m_items[i].iStatus==XTS_ACTIVE)
			m_items[i].iStatus=XTS_NORMAL;
	UpdateToolBar();
	XWindow::OnMove(rect);
}

XBOOL XToolBar::UpdateToolBar()
{
	if(m_nItemCount<=3) 
	{
		RemoveItem(XCMD_MOVE_TOOLBAR);
		m_nPageCount=m_nItemCount;
		return XFALSE;
	}
	int w=m_rect.Width(),h=m_nItemWidth;
#ifdef _SYMBIAN
	if(!h) h = 10;
#endif
	int nCount=w/h;
	XBOOL bPage=FindItem(XCMD_MOVE_TOOLBAR);
	if(bPage) nCount++;
	if(nCount>=m_nItemCount)
	{
		m_nPageCount=nCount;
			//m_nItemCount;
		RemoveItem(XCMD_MOVE_TOOLBAR);
		return XFALSE;
	}
	else
	{
		m_nPageCount=nCount-m_nItemLeft-m_nItemRight-1;
		if(bPage) m_nPageCount--;
		AddItem(XCMD_MOVE_TOOLBAR,0,XTS_NORMAL,XTP_RIGHT);
		return XTRUE;
	}
}


XBOOL XToolBar::SetItemSlider(XU32 iCommand,int iSlider)
{
	if(iSlider>100)
		iSlider=100;
	int dw=m_nItemWidth;
	int il=0,ir=1,im=0,llx=m_nItemLeft*dw,rrx=m_rect.Width()-m_nItemRight*dw;
	for(int i=0;i<m_nItemCount;i++)
	{
		if(m_items[i].iCommand==iCommand)
		{
			if(m_items[i].iPos==iSlider) return XTRUE;
			m_items[i].iPos=iSlider;
			//int ix=CommandToPos(iCommand);
			int ix=0,ex=0;
			XRect rect(0,0,0,m_rect.Height());
			if(m_items[i].iImage==TICON_SLIDER)
			{
				ix=dw*im;
				rect.left=ix;
				rect.right=rect.left+dw*SLIDER_WIDTH;
				InvalidateRect(rect);
			}
			else
			{
				switch(m_items[i].iPos)
				{
				case XTP_LEFT:
					 ix=dw*il;
					 rect.left=ix;
					 rect.right=rect.left+dw;
					 InvalidateRect(rect);
					 break;
				case XTP_RIGHT:
					 ix=m_rect.Width()-dw*ir;
					 rect.left=ix;
					 rect.right=ix+dw;
					 InvalidateRect(rect);
					 break;
				default:
					 ix=llx+im*dw-m_nOffset;
					 ex=ix+dw;
					 if(ix<llx)
						 ix=llx;
					 if(ex>rrx)
						 ex=rrx;
					 if(ix<ex)
					 {
						 rect.left=ix;
						 rect.right=ex;
						 InvalidateRect(rect);
					 }
					 break;
				}
			}
			

			return XTRUE;
		}
		if(m_items[i].iImage==TICON_SLIDER)
		{
			im+=SLIDER_WIDTH;
		}
		else
		{
			switch(m_items[i].iPos)
			{
			case XTP_LEFT:
				 il++;break;
			case XTP_RIGHT:
				 ir++;break;
			default:
				 im++;
				 break;
			}
		}
	}
	return XFALSE;
}


XBOOL XToolBar::SetItemStatus(XU32 iCommand,int nStatus)
{
	int dw=m_nItemWidth;
	int il=0,ir=1,im=0,llx=m_nItemLeft*dw,rrx=m_rect.Width()-m_nItemRight*dw;
	for(int i=0;i<m_nItemCount;i++)
	{
		if(m_items[i].iCommand==iCommand)
		{
			if(m_items[i].iStatus==nStatus&&m_items[i].iImage!=TICON_SLIDER) return XTRUE;
			m_items[i].iStatus=nStatus;
			//int ix=CommandToPos(iCommand);
			int ix=0,ex=0;
			XRect rect(0,0,0,m_rect.Height());
			if(m_items[i].iImage==TICON_SLIDER)
			{
				ix=dw*im;
				rect.left=ix;
				rect.right=rect.left+dw*SLIDER_WIDTH+24;
				InvalidateRect(rect);
			}
			else
			{
				switch(m_items[i].iPos)
				{
				case XTP_LEFT:
					 ix=dw*il;
					 rect.left=ix;
					 rect.right=rect.left+dw;
					 InvalidateRect(rect);
					 break;
				case XTP_RIGHT:
					 ix=m_rect.Width()-dw*ir;
					 rect.left=ix;
					 rect.right=ix+dw;
					 InvalidateRect(rect);
					 break;
				default:
					 ix=llx+im*dw-m_nOffset;
					 ex=ix+dw;
					 if(ix<llx)
						 ix=llx;
					 if(ex>rrx)
						 ex=rrx;
					 if(ix<ex)
					 {
						 rect.left=ix;
						 rect.right=ex;
						 InvalidateRect(rect);
					 }
					 break;
				}
			}
			

			return XTRUE;
		}
		if(m_items[i].iImage==TICON_SLIDER)
		{
			im+=SLIDER_WIDTH;
		}
		else
		{
			switch(m_items[i].iPos)
			{
			case XTP_LEFT:
				 il++;break;
			case XTP_RIGHT:
				 ir++;break;
			default:
				 im++;
				 break;
			}
		}
	}
	return XFALSE;
}


void XToolBar::OnMouseMove(XPoint &pt,int iPos)
{
	if(iPos) return;
	if(pt.y<0||pt.y>m_rect.Height()) return;
	//if(m_bDown) return;
	int subID=0;
    int id=GetActiveID(pt.x,subID);
	m_nSubID=subID;
	if(m_items[id].iImage==TICON_SLIDER)
	{
		if(m_items[id].iStatus!=XTS_DISABLE)
		{
			if(m_items[m_nActiveID].iStatus==XTS_ACTIVE)
				SetItemStatus(m_items[m_nActiveID].iCommand,XTS_NORMAL);
			m_nActiveID=id;
			if(subID)
			{
				SetItemStatus(m_items[id].iCommand,XTS_NORMAL);
				if(subID==1)
				{
					if(m_items[id].iPos>0)			
						m_items[id].iStatus=XTS_ACTIVE1;
				}
				else if(subID==2)
				{
					if(m_items[id].iPos<100)
						m_items[id].iStatus=XTS_ACTIVE2;
				}
				else 
				{
					if(m_nMvStatus==XTOOLBAR_MV_DOWN)
					{
						m_items[id].iSlider=1;
						PostMsg(XWM_COMMAND,m_items[id].iCommand,XTOOLBAR_MV_DOWN,XMS_PARENT);
					}
					if(m_bIsDown&&m_items[id].iPos!=subID-100&&m_items[id].iSlider)
					{
						m_items[id].iPos=subID-100;
						SetItemStatus(m_items[id].iCommand,XTS_NORMAL);
						//if(m_bIsDown)						
						PostMsg(XWM_COMMAND,m_items[id].iCommand,m_items[id].iPos+100,XMS_PARENT);
					}
				}

			}
		}
	}
	if(m_nActiveID==id) return;
	
	if(m_nActiveID>=0)
	{
		if(m_items[m_nActiveID].iStatus==XTS_ACTIVE)
			SetItemStatus(m_items[m_nActiveID].iCommand,XTS_NORMAL);
		m_bDown=XFALSE;
	}
	
	if(m_items[id].bHide){m_nActiveID=-1; return;}
	
	//else
	{
		switch(m_items[id].iStatus)
		{
		case XTS_NORMAL:
		case XTS_ACTIVE:
			m_nActiveID=id;
			if(m_nActiveID>=0)
				SetItemStatus(m_items[m_nActiveID].iCommand,XTS_ACTIVE);
			break;
		}
	}
}

void XToolBar::OnMouseUp(XPoint &pt,int iPos)
{
	if(iPos) return;
	int nDownID=m_nDownID;
	m_nDownID=-1;
	m_bIsDown=XFALSE;
	
	if(!m_bDown||m_nActiveID<0) return;
	m_bDown=XFALSE;	
	SetItemStatus(m_items[m_nActiveID].iCommand,XTS_NORMAL);
	XU32 iCmd=m_items[m_nActiveID].iCommand;
	if(iCmd==XCMD_MOVE_TOOLBAR)
	{
		m_nShowBegin+=m_nPageCount;
		if(m_nShowBegin>=m_nItemCount-m_nItemLeft-m_nItemRight)
			m_nShowBegin=0;
		m_nOffsetEnd=m_nShowBegin*m_nItemWidth;
		m_nStep=10;
	}
	else
	{
		if(m_items[m_nActiveID].iImage==TICON_SLIDER)
		{
			PostMsg(XWM_COMMAND,iCmd,XTOOLBAR_MV_UP,XMS_PARENT);
			if(m_nActiveID!=nDownID) return;
			int iPos=m_items[m_nActiveID].iPos;
			if(m_nSubID==1)
			{
				iPos-=20;
				if(iPos<0) iPos=0;
			}
			else if(m_nSubID==2)
			{
				iPos+=20;
				if(iPos>100) iPos=100;
			}
			//else
			
			m_items[m_nActiveID].iSlider=0;
			SetItemStatus(m_items[m_nActiveID].iCommand,XTS_NORMAL);
			//if(m_items[m_nActiveID].iPos!=iPos)
			{
				m_items[m_nActiveID].iPos=iPos;
				
			}
		}
		PostMsg(XWM_COMMAND,iCmd,m_items[m_nActiveID].iPos+100,XMS_PARENT);
		
	}
	m_nActiveID=-1;
}

void XToolBar::OnMouseDown(XPoint&pt,int iPos)
{
	if(iPos) return;
	m_bDown=XFALSE;
	m_nActiveID=-1;
	m_nMvStatus=XTOOLBAR_MV_DOWN;
	OnMouseMove(pt,iPos);
	m_nDownID=m_nActiveID;
	m_nMvStatus=XTOOLBAR_MV_MOVE;
    m_bDown=XTRUE;
	m_bIsDown=XTRUE;
	/*if(m_nActiveID>=0&&m_items[i].iImage==TICON_SLIDER)
	{

	}*/
}


int XToolBar::GetActiveID(int x,int&nSubID)
{
   int dw=m_nItemWidth;
   int il=0,ir=1,im=0,llx=m_nItemLeft*dw,rrx=m_rect.Width()-m_nItemRight*dw;

   for(int i=0;i<m_nItemCount;i++)
	{
		int	  ix,ex;
		if(m_items[i].iImage==TICON_SLIDER)
		{
			ix=dw*im;
			ex=ix+dw*SLIDER_WIDTH+dw;
			im+=SLIDER_WIDTH;
			if(x>ix&&x<ex) 
			{
				nSubID=0;
				//if(x<ix+dw)
				//if(x>ix+12&&x<ix+dw+12)
				//	subID=1;
				int bx=(im-SLIDER_WIDTH+1)*dw+12;//+16;
				int ex=(im)*dw;
				if(!m_items[i].iSlider&&x>bx&&x<bx+dw)
					nSubID=1;
				else if(!m_items[i].iSlider&&x>ex&&x<ex+dw)
					nSubID=2;
				else
				{
					bx+=dw+4;
					ex-=4;
					if(x>=bx&&x<=ex)
					{
						int iPos=100*(x-bx)/(ex-bx);
						if(iPos<0) iPos=0;
						else if(iPos>100) iPos=100;
						nSubID=iPos+100;
					}
					//bx+=dw
				}
				//else if(x<ex-12&&x>ex-12)
				return i;
			}
			
		}
		else
		{
			switch(m_items[i].iPos)
			{
			case XTP_LEFT:
				 ix=dw*il;
				 ex=ix+dw;
				 if(x>ix&&x<ex) return i;
				 il++;
				 break;
			case XTP_RIGHT:
				 ix=m_rect.Width()-dw*ir;
				 ex=ix+dw;
				 if(x>ix&&x<ex) return i;
				 ir++;
				 break;
			default:
				 if(im>=m_nShowBegin+m_nPageCount)
			 		 break;
				 
				 ix=llx+im*dw-m_nOffset;
				 ex=ix+dw;
				 im++;
				 if(ex>rrx) ex=rrx;			 
				 if(ix<llx) ix=llx;
				 
				 if(ix<ex&&x>ix&&x<ex)
					 return i;
				 break;
			}	
		}
   }
   return -1;
}

void XToolBar::OnIdle()
{
    if(!m_nStep) return;
	int ddx=(m_nItemWidth*m_nStep)/10;
	if(ddx<4) ddx=4;
	m_nOffset+=ddx;
	
	int dmx=m_nOffset-m_nOffsetEnd;
	if(dmx<0) dmx=-dmx;
	if(dmx<=ddx)
	{
		m_nOffset=m_nOffsetEnd;
		m_nStep=0;
	}
	else
		if(m_nStep>1) m_nStep--;
	if(m_nOffset>=(m_nItemCount-m_nItemLeft-m_nItemRight)*m_nItemWidth)
		m_nOffset=-(m_rect.Width()-(m_nItemLeft+m_nItemRight)*m_nItemWidth);
	XRect r(m_nItemWidth*m_nItemLeft,0,
			m_rect.Width()-m_nItemWidth*m_nItemRight,m_rect.Height());
	InvalidateRect(r);
	
}





XBOOL XToolBar::CheckItem(XU32 nCommand, XBOOL bCheck)
{
	int dw=m_nItemWidth;
	int il=0,ir=1,im=0,llx=m_nItemLeft*dw,rrx=m_rect.Width()-m_nItemRight*dw;
	for(int i=0;i<m_nItemCount;i++)
	{
		if(m_items[i].iCommand==nCommand)
		{
			if(m_items[i].bChecked==bCheck) return XTRUE;
			m_items[i].bChecked=bCheck;
			//int ix=CommandToPos(iCommand);
			int ix=0,ex=0;
			XRect rect(0,0,0,m_rect.Height());
			if(m_items[i].iImage==TICON_SLIDER)
			{
				ix=dw*im;
				rect.left=ix;
				rect.right=rect.left+dw*SLIDER_WIDTH;
				InvalidateRect(rect);
			}
			else
			{
				switch(m_items[i].iPos)
				{
				case XTP_LEFT:
					 ix=dw*im;
					 rect.left=ix;
					 rect.right=rect.left+dw;
					 InvalidateRect(rect);
					 break;
				case XTP_RIGHT:
					 ix=m_rect.Width()-dw*ir;
					 rect.left=ix;
					 rect.right=ix+dw;
					 InvalidateRect(rect);
					 break;
				default:
					 ix=llx+im*dw-m_nOffset;
					 ex=ix+dw;
					 if(ix<llx)
						 ix=llx;
					 if(ex>rrx)
						 ex=rrx;
					 if(ix<ex)
					 {
						 rect.left=ix;
						 rect.right=ex;
						 InvalidateRect(rect);
					 }
					 break;
				}
				

				return XTRUE;
			}
		}
		if(m_items[i].iImage==TICON_SLIDER)
		{
			im+=SLIDER_WIDTH;
		}
		else
		{
			switch(m_items[i].iPos)
			{
			case XTP_LEFT:
				 il++;break;
			case XTP_RIGHT:
				 ir++;break;
			default:
				 im++;
				 break;
			}
		}
	}
	return XFALSE;
}

#endif
#endif

