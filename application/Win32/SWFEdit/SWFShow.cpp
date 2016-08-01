// SWFShow.cpp : implementation file
//

#include "stdafx.h"
#include "SWFEdit.h"
#include "SWFShow.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSWFShow

BEGIN_MESSAGE_MAP(XSWFShow, CButton)
	//{{AFX_MSG_MAP(CSWFShow)
	// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

XSWFShow::XSWFShow()
{
	m_pPlayer=NULL;
	miType= -1;
	mbFullScreen = false;
}

XSWFShow::~XSWFShow()
{
}

/////////////////////////////////////////////////////////////////////////////
// CSWFShow message handlers

void XSWFShow::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your code to draw the specified item
	CRect rect(lpDrawItemStruct->rcItem);
	CDC*pDC=CDC::FromHandle(lpDrawItemStruct->hDC);
	if(!m_dib.IsEmpty())
	{
		XRect rect(0,0,m_dib.Width(),m_dib.Height());
		if( mbFullScreen )
		{
			CRect r;
			GetClientRect(r);
			rect.right=r.Width();
			rect.bottom=r.Height();
		}
		m_dib.Paint(pDC->m_hDC,0,0,rect);
	}
	else if(m_pPlayer==NULL)
	{
		CBrush brush((COLORREF)0);
		pDC->FillRect(&rect,&brush);
	}
	else
	{
		XRect rt(rect);
		m_pPlayer->bits.bitmap->Paint(pDC->m_hDC,0,0,rt);//->raster;
	}
}

XBOOL XSWFShow::LoadDib(XDrawDib&dib,CString&strFile)
{
	CFile file;
	if(!file.Open(strFile,CFile::modeRead|CFile::typeBinary)) return XFALSE;
	XImage image;
	int ilen=file.GetLength();
	if(ilen<=0)
	{
		file.Close();
		return XFALSE;
	}
	BYTE*pData=new BYTE[ilen];
	file.Read(pData,ilen);
	file.Close();
	image.Append(pData,ilen);
	delete pData;
	if(!image.Final(1)) 
		return XFALSE;
	int w=image.Width();
	int h=image.Height();
	dib.Create(w,h,32);
	image.GetBitmapData(dib.GetBuffer(),w,h,32,w*h*4);
	return XTRUE;
}

void XSWFShow::ShowObject(CString&strFile)
{
	LoadDib(m_dib,strFile);
	Invalidate();
}

void XSWFShow::ShowObject(SCharacter*ch)
{
	m_showObj=ch;
	m_dib.Release();
	if(m_pPlayer)
	{
		
		DisplayList*disp=&m_pPlayer->display;
		disp->root.Modify();
		disp->root.FreeChildren();
		if(ch!=NULL)
		{
			//disp->root.FreeChildren();
			//m_pPlayer->player->DrawFrame(0,false,false);//(0,false,0);
			//m_pPlayer->CheckUpdate();
			PlaceInfo info;
			info.flags = splaceCharacter|splaceMatrix;
			XU16 tag = ch->tag;
			info.character = ch;

			info.depth = 1;
			
			CRect rect;
			GetClientRect(rect);
			CSize sz=rect.Size();
			::MatrixIdentity(&info.mat);
			
			if( !mbFullScreen)
			{
				XMatrix mat=disp->camera.mat;
				::MatrixInvert(&mat,&info.mat);
				::MatrixIdentity(&mat);
				mat.a/=20;//>>=disp->antialias_b;
				mat.d/=20;//>>=disp->antialias_b;
				if(disp->antialias_b)
				{
					mat.a<<=disp->antialias_b;
					mat.d<<=disp->antialias_b;
				}
				::MatrixConcat(&info.mat,&mat,&info.mat);
				/*			if(  ch->type != bitsChar )
				{
				info.mat.tx = sz.cx * 10;
				info.mat.ty = sz.cy * 10;
				}*/
			}
			else if(!RectIsEmpty(&ch->rbounds))
			{
				SRECT rt=ch->rbounds;
				XMatrix mat;
				::MatrixIdentity(&mat);
				::MatrixTransformRect(&disp->camera.mat,&rt,&rt);
				int fx=sz.cx*100/(rt.xmax-rt.xmin);
				int fy=sz.cy*100/(rt.ymax-rt.ymin);
				int f=XMIN(fx,fy);
				if(disp->antialias_b)
				{
					f<<=disp->antialias_b;
				}
				mat.a=mat.a*f/100;
				mat.d=mat.d*f/100;
				info.mat=mat;
			}
				
		
			if(  miType == buttonChar )
			{
				info.mat.tx = sz.cx * 10;
				info.mat.ty = sz.cy * 10;
			}
			info.cxform.Clear();
			info.ratio = 0;
			info.name = _strEmpty;
			info.bAdd=XTRUE;//this->m_bInDisplay;
			disp->PlaceObject2(&disp->root, &info, XNULL,false);//!seeking);
		}
		disp->Update();
	}
	Invalidate();
}
