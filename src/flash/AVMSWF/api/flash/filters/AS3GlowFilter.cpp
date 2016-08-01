#include "StdAfxflash.h"
#include "ShellCore.h"
using namespace avmplus;
#include "AS3GlowFilter.h"
#include "splayer.h"
namespace avmshell{
GlowFilterClass::GlowFilterClass(VTable* cvtable):ClassClosure(cvtable)
{
	
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())GlowFilterObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* GlowFilterClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) GlowFilterObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
GlowFilterObject::GlowFilterObject(VTable *vtable, ScriptObject* proto, int capacity): BitmapFilterObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
GlowFilterObject* GlowFilterClass::CreateFilter(void*data)
{
	GlowFilterObject*pObj=(GlowFilterObject*)createInstance(ivtable(),prototype);
	return pObj;
}


_XFilter* GlowFilterObject::CreateFilter()
{
	XSWFPlayer*player=((ShellCore*)core())->GetPlayer();
	_XFilter*pFilter=player->CreateFilter();
	pFilter->m_nFilterID=XFilterGlow;
	pFilter->nColor=m_slots_GlowFilterObject.color;
#ifndef _ARGB_PIXEL_
	SWAPPIXELRB((pFilter->nColor));
	pFilter->filter_konckout=0;
#endif
//#if (__CORE_VERSION__>=0x02076000)
	//pFilter->nData=(m_slots_GlowFilterObject.blurX+m_slots_GlowFilterObject.blurY)/2;
	//if(m_slots_GlowFilterObject.alpha>0)
	//{
	//	XU8*pData=(XU8*)&pFilter->nColor;
	//	int nDiv=m_slots_GlowFilterObject.alpha*16;
	//		//(pFilter->nData+4);
	//	if(nDiv>16) nDiv=16;
	//	pData[0]=pData[0]*nDiv/16;
	//	pData[1]=pData[1]*nDiv/16;
	//	pData[2]=pData[2]*nDiv/16;
	//	
	//	//XU8 tmp=pData[0];
	//	//pData[0]=pData[2];
	//	//pData[2]=tmp;
	//	//pFilter->nColor=0;
	//	int alpha=(m_slots_GlowFilterObject.alpha*255);
	//	if(alpha>255) alpha=255;
	//	pFilter->nColor|=alpha<<24;
	//}
	pFilter->pGlow.blurX=m_slots_GlowFilterObject.blurX;
	pFilter->pGlow.blurY=m_slots_GlowFilterObject.blurY;
	pFilter->pGlow.color[0]=((m_slots_GlowFilterObject.color&0xFF0000)>>16)/255.0;
	pFilter->pGlow.color[1]=((m_slots_GlowFilterObject.color&0x00FF00)>>8)/255.0;
	pFilter->pGlow.color[2]=(m_slots_GlowFilterObject.color&0x0000FF)/255.0;
	pFilter->pGlow.color[3]=(m_slots_GlowFilterObject.alpha);
	pFilter->pGlow.strength=m_slots_GlowFilterObject.strength;
	pFilter->pGlow.konckout=m_slots_GlowFilterObject.knockout;
	
//#endif
	return pFilter;
}

}