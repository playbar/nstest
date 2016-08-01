#include "StdAfxflash.h"
#include "ShellCore.h"
using namespace avmplus;
#include "AS3DropShadowFilter.h"
#include "splayer.h"
namespace avmshell{
DropShadowFilterClass::DropShadowFilterClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())DropShadowFilterObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* DropShadowFilterClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) DropShadowFilterObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
DropShadowFilterObject::DropShadowFilterObject(VTable *vtable, ScriptObject* proto, int capacity): BitmapFilterObject(vtable, proto, 0)
{
	//Add your construct code here...
}

DropShadowFilterObject* DropShadowFilterClass::CreateFilter(void*data)
{
	DropShadowFilterObject*pObj=(DropShadowFilterObject*)createInstance(ivtable(),prototype);
	return pObj;
}


_XFilter* DropShadowFilterObject::CreateFilter()
{
	XSWFPlayer*player=((ShellCore*)core())->GetPlayer();
	_XFilter*pFilter=player->CreateFilter();
	pFilter->m_nFilterID=XFilterDropShadow;
	pFilter->filter_konckout=0;
	//pFilter->nColor=m_slots_DropShadowFilterObject.color;
	pFilter->pDropShadow.angle=m_slots_DropShadowFilterObject.angle;
	pFilter->pDropShadow.blurX=m_slots_DropShadowFilterObject.blurX;
	pFilter->pDropShadow.blurY=m_slots_DropShadowFilterObject.blurY;
	pFilter->pDropShadow.distance=m_slots_DropShadowFilterObject.distance;
	pFilter->pDropShadow.color[0]=((m_slots_DropShadowFilterObject.color&0xFF0000)>>16)/255.0;
	pFilter->pDropShadow.color[1]=((m_slots_DropShadowFilterObject.color&0x00FF00)>>8)/255.0;
	pFilter->pDropShadow.color[2]=(m_slots_DropShadowFilterObject.color&0x0000FF)/255.0;
	pFilter->pDropShadow.color[3]=(m_slots_DropShadowFilterObject.alpha);
	pFilter->pDropShadow.strength=m_slots_DropShadowFilterObject.strength;
	pFilter->pDropShadow.konckout=m_slots_DropShadowFilterObject.knockout;
	return pFilter;
}
//////////////////////////////////////////////////////////
//Native Method start...

}