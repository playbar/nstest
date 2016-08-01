#include "StdAfxflash.h"
#include "ShellCore.h"
using namespace avmplus;
#include "AS3ColorMatrixFilter.h"
#include "splayer.h"
namespace avmshell{
ColorMatrixFilterClass::ColorMatrixFilterClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())ColorMatrixFilterObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* ColorMatrixFilterClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) ColorMatrixFilterObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
ColorMatrixFilterObject::ColorMatrixFilterObject(VTable *vtable, ScriptObject* proto, int capacity): BitmapFilterObject(vtable, proto, 0)
{
	
}
//////////////////////////////////////////////////////////
//Native Method start...
ColorMatrixFilterObject* ColorMatrixFilterClass::CreateFilter(void*data)
{
	ColorMatrixFilterObject*pObj=(ColorMatrixFilterObject*)createInstance(ivtable(),prototype);
	return pObj;
}

_XFilter* ColorMatrixFilterObject::CreateFilter()
{
	if(m_slots_ColorMatrixFilterObject.matrix==XNULL)
		return XNULL;
	XSWFPlayer*player=((ShellCore*)core())->GetPlayer();
	_XFilter*pFilter=player->CreateFilter();
	pFilter->m_nFilterID=XFilterColorMatrix;
	pFilter->filter_konckout=0;
	if(!m_slots_ColorMatrixFilterObject.matrix)
		return pFilter;
	Atom*pAtom;
	pAtom=m_slots_ColorMatrixFilterObject.matrix->GetAtoms();
	AvmCore* c=core();
	for(int i=0;i<20;i++)
	{
		if(i==4||i==9||i==14||i==19)
			pFilter->pColorMatrix.fData[i]=(c->number(pAtom[i])/256);//pAtom[i]>>3;
		else
			pFilter->pColorMatrix.fData[i]=(c->number(pAtom[i]));
	}
	return pFilter;
}

}