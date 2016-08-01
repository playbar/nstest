#include "StdAfxflash.h"
#include "ShellCore.h"
using namespace avmplus;
#include "AS3GradientBevelFilter.h"
#include "splayer.h"
namespace avmshell{
GradientBevelFilterClass::GradientBevelFilterClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())GradientBevelFilterObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* GradientBevelFilterClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) GradientBevelFilterObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
GradientBevelFilterObject::GradientBevelFilterObject(VTable *vtable, ScriptObject* proto, int capacity): BitmapFilterObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
GradientBevelFilterObject* GradientBevelFilterClass::CreateFilter(void*data)
{
	GradientBevelFilterObject*pObj=(GradientBevelFilterObject*)createInstance(ivtable(),prototype);
	return pObj;
}

_XFilter* GradientBevelFilterObject::CreateFilter()
{
	XSWFPlayer*player=((ShellCore*)core())->GetPlayer();
	_XFilter*pFilter=player->CreateFilter();
	pFilter->m_nFilterID=XFilterGradientBevel;
	pFilter->filter_konckout=0;
	pFilter->pGradientBevel.angle=m_slots_GradientBevelFilterObject.angle;
	pFilter->pGradientBevel.blurX=m_slots_GradientBevelFilterObject.blurX;
	pFilter->pGradientBevel.blurY=m_slots_GradientBevelFilterObject.blurY;
	pFilter->pGradientBevel.distance=m_slots_GradientBevelFilterObject.distance;
	Atom*pAtom;
	int l=0;
	pAtom=m_slots_GradientBevelFilterObject.colors->GetAtoms();
	l = m_slots_GradientBevelFilterObject.colors->getLength();
	U32 color_temp = pAtom[0]>>3;
	pFilter->pGradientBevel.shadowColor[0]=((color_temp&0xFF0000)>>16)/255.0;
	pFilter->pGradientBevel.shadowColor[1]=((color_temp&0x00FF00)>>8)/255.0;
	pFilter->pGradientBevel.shadowColor[2]=(color_temp&0x0000FF)/255.0;
	color_temp = pAtom[l-1]>>3;
	pFilter->pGradientBevel.hiColor[0]=((color_temp&0xFF0000)>>16)/255.0;
	pFilter->pGradientBevel.hiColor[1]=((color_temp&0x00FF00)>>8)/255.0;
	pFilter->pGradientBevel.hiColor[2]=(color_temp&0x0000FF)/255.0;
	pAtom=m_slots_GradientBevelFilterObject.alphas->GetAtoms();
	pFilter->pGradientBevel.shadowColor[3]=((pAtom[0]>>3));
	pFilter->pGradientBevel.hiColor[3]=((pAtom[l-1]>>3));
	pFilter->pGradientBevel.konckout=m_slots_GradientBevelFilterObject.knockout;
	return pFilter;
}

}