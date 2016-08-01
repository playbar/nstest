#include "StdAfxflash.h"
#include "ShellCore.h"
using namespace avmplus;
#include "AS3ConvolutionFilter.h"
#include "splayer.h"

namespace avmshell{
ConvolutionFilterClass::ConvolutionFilterClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())ConvolutionFilterObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* ConvolutionFilterClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) ConvolutionFilterObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
ConvolutionFilterObject::ConvolutionFilterObject(VTable *vtable, ScriptObject* proto, int capacity): BitmapFilterObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...

ConvolutionFilterObject* ConvolutionFilterClass::CreateFilter(void*data)
{
	ConvolutionFilterObject*pObj=(ConvolutionFilterObject*)createInstance(ivtable(),prototype);
	return pObj;
}

_XFilter* ConvolutionFilterObject::CreateFilter()
{
	XSWFPlayer*player=((ShellCore*)core())->GetPlayer();
	_XFilter*pFilter=player->CreateFilter();
	pFilter->m_nFilterID=XFilterConvolution;
	pFilter->filter_konckout=0;
	//int l = m_slots_ConvolutionFilterObject.matrix->getDenseLength();
	if(m_slots_ConvolutionFilterObject.matrixX==3&&m_slots_ConvolutionFilterObject.matrixY==3)
	{
		Atom*pAtom;
		pAtom=m_slots_ConvolutionFilterObject.matrix->GetAtoms();
		AvmCore* c=core();
		for(int i=0;i<9;i++)
		{
			c->number(pAtom[i]);
			pFilter->pConvolution.fData[i]=(pAtom[i]>>3)/m_slots_ConvolutionFilterObject.divisor;
		}
	}
	return pFilter;
}

}