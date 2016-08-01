#include "StdAfxflash.h"
#include "ShellCore.h"
using namespace avmplus;
#include "AS3BevelFilter.h"
#include "splayer.h"
namespace avmshell{
BevelFilterClass::BevelFilterClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())BevelFilterObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* BevelFilterClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) BevelFilterObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
BevelFilterObject::BevelFilterObject(VTable *vtable, ScriptObject* proto, int capacity): BitmapFilterObject(vtable, proto, 0)
{
	//Add your construct code here...
}

BevelFilterObject* BevelFilterClass::CreateFilter(void*data)
{
	BevelFilterObject*pObj=(BevelFilterObject*)createInstance(ivtable(),prototype);
	return pObj;
}

_XFilter* BevelFilterObject::CreateFilter()
{
	XSWFPlayer*player=((ShellCore*)core())->GetPlayer();
	_XFilter*pFilter=player->CreateFilter();
	pFilter->m_nFilterID=XFilterBevel;
	//pFilter->nColor=m_slots_BevelFilterObject.shadowColor;
//#if (__CORE_VERSION__>=0x02076000)
	/*if(m_slots_BevelFilterObject.shadowAlpha)
	{
		int alpha=m_slots_BevelFilterObject.shadowAlpha*0xff;
		if(alpha>255) alpha=255;
		pFilter->nColor|=(alpha<<24);
	}*/
	//pFilter->nData=(m_slots_BevelFilterObject.blurX+m_slots_BevelFilterObject.blurY)/2;
	pFilter->filter_konckout=0;
	pFilter->pBevel.angle=m_slots_BevelFilterObject.angle;
	pFilter->pBevel.blurX=m_slots_BevelFilterObject.blurX;
	pFilter->pBevel.blurY=m_slots_BevelFilterObject.blurY;
	pFilter->pBevel.distance=m_slots_BevelFilterObject.distance;
	pFilter->pBevel.shadowColor[0]=((m_slots_BevelFilterObject.highlightColor&0xFF0000)>>16)/255.0;
	pFilter->pBevel.shadowColor[1]=((m_slots_BevelFilterObject.highlightColor&0x00FF00)>>8)/255.0;
	pFilter->pBevel.shadowColor[2]=(m_slots_BevelFilterObject.highlightColor&0x0000FF)/255.0;
	pFilter->pBevel.shadowColor[3]=(m_slots_BevelFilterObject.highlightAlpha);
	pFilter->pBevel.hiColor[0]=((m_slots_BevelFilterObject.shadowColor&0xFF0000)>>16)/255.0;
	pFilter->pBevel.hiColor[1]=((m_slots_BevelFilterObject.shadowColor&0x00FF00)>>8)/255.0;
	pFilter->pBevel.hiColor[2]=(m_slots_BevelFilterObject.shadowColor&0x0000FF)/255.0;
	pFilter->pBevel.hiColor[3]=(m_slots_BevelFilterObject.shadowAlpha);
	pFilter->pBevel.konckout=m_slots_BevelFilterObject.knockout;
//#endif
	return pFilter;
}

}
