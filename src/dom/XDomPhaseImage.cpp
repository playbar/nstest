// XDomPhaseImage.cpp: implementation of the XDomPhaseImage class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxggdom.h"
#include "XDomPhaseImage.h"
#include "XDomView.h"
#include "XDomCard.h"
#include "XDomImg.h"
#include "XDom.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XDomPhaseImage::XDomPhaseImage(XDom*pDom):XDomPhase(pDom)
{
 
}

XDomPhaseImage::~XDomPhaseImage()
{
   if(m_pDom->m_imgList.GetCount()>0)
   {
	   XIMAGEDATA*p=m_pDom->m_imgList[0];
	   p->Final(m_pDom);
	   //m_pDom->FinalImage(p);
	   /*if(p->image.Final())
	   {
		  p->nStatus=XL_OK;
	   }
	   else
		  p->nStatus=XL_FAILED;*/
	   //m_pDom->Layerout();
	   m_pDom->m_bLayerout=XTRUE;//SetLayerout();
   }
}

XBOOL XDomPhaseImage::PhaseData(XTCHAR *pData, int nSize,bool bFlash)
{
   if(m_pDom->m_imgList.GetCount()<=0)
   {
	   //XIMAGEDATA*p=new XIMAGEDATA;
	   //m_pDom->m_imgList.Add(p);
	   //p->nStatus=XL_LOADING;
	   //m_pDom->m_imgList.SetImage("",);
	   m_pDom->AddTag(new XDomNode(XDOM_WML));
	   //XVar var(XEAB::ALIGN,XEA::CENTER);
	   XDomCard*pc=new XDomCard(XDOM_CARD);
	   //pc->AddAttrib(var);
	   m_pDom->AddTag(pc);
	   //XDomP*pp=new XDomP(XDOM_P);
	   XDomImg* pImg=new XDomImg(XDOM_IMG);
	   XVar var1(XEAB::SRC,m_pDom->GetURL().m_strURL);
	   pImg->SetAttrib(var1,XTRUE);
	   //pImg->SetImage(p);
	   m_pDom->AddTag(pImg);
	   m_pDom->Parent(XDOM_IMG);
	   if(pImg->m_pData!=XNULL)
		   pImg->m_pData->nStatus=XL_LOADING;
	   //pp->AddAttrib(var);
	   //m_pDom->AddTag(pp);
	   
   }
   XIMAGEDATA*p=m_pDom->m_imgList[0];
   p->AppendData((XU8*)pData,nSize);
   //m_pDom->SetLayerout();
   return XTRUE;
}
