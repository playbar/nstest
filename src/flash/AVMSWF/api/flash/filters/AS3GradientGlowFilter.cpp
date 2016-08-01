#include "StdAfxflash.h"
#include "ShellCore.h"
using namespace avmplus;
#include "AS3GradientGlowFilter.h"
#include "splayer.h"
namespace avmshell{
GradientGlowFilterClass::GradientGlowFilterClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())GradientGlowFilterObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* GradientGlowFilterClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) GradientGlowFilterObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
GradientGlowFilterObject::GradientGlowFilterObject(VTable *vtable, ScriptObject* proto, int capacity): BitmapFilterObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...

GradientGlowFilterObject* GradientGlowFilterClass::CreateFilter(void*data)
{
	GradientGlowFilterObject*pObj=(GradientGlowFilterObject*)createInstance(ivtable(),prototype);
	return pObj;
}

_XFilter* GradientGlowFilterObject::CreateFilter()
{
	XSWFPlayer*player=((ShellCore*)core())->GetPlayer();
	_XFilter*pFilter=player->CreateFilter();
	pFilter->m_nFilterID=XFilterGradientGlow;
	pFilter->pGradientGlow.angle=m_slots_GradientGlowFilterObject.angle;
	pFilter->pGradientGlow.blurX=m_slots_GradientGlowFilterObject.blurX;
	pFilter->pGradientGlow.blurY=m_slots_GradientGlowFilterObject.blurY;
	pFilter->pGradientGlow.distance=m_slots_GradientGlowFilterObject.distance;
	Atom*pAtom;
	int l=0;
	if(m_slots_GradientGlowFilterObject.colors)
	{
		pAtom=m_slots_GradientGlowFilterObject.colors->GetAtoms();
		l = m_slots_GradientGlowFilterObject.colors->getLength();
		U32 color_temp = pAtom[l-1]>>3;
		pFilter->pGradientGlow.color[0]=((color_temp&0xFF0000)>>16)/255.0;
		pFilter->pGradientGlow.color[1]=((color_temp&0x00FF00)>>8)/255.0;
		pFilter->pGradientGlow.color[2]=(color_temp&0x0000FF)/255.0;
	}
	if(m_slots_GradientGlowFilterObject.alphas)
	{
		pAtom=m_slots_GradientGlowFilterObject.alphas->GetAtoms();
		pFilter->pGradientGlow.color[3]=((pAtom[l-1]>>3));
	}
	pFilter->pGradientGlow.konckout=m_slots_GradientGlowFilterObject.knockout;
	return pFilter;
}

}