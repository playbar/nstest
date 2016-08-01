#include "StdAfxflash.h"
#include "ShellCore.h"
using namespace avmplus;
#include "AS3BlurFilter.h"
#include "splayer.h"

namespace avmshell{
BlurFilterClass::BlurFilterClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())BlurFilterObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* BlurFilterClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) BlurFilterObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
BlurFilterObject::BlurFilterObject(VTable *vtable, ScriptObject* proto, int capacity): BitmapFilterObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
BlurFilterObject* BlurFilterClass::CreateFilter(void*data)
{
	BlurFilterObject*pObj=(BlurFilterObject*)createInstance(ivtable(),prototype);
	return pObj;
}

_XFilter* BlurFilterObject::CreateFilter()
{
	XSWFPlayer*player=((ShellCore*)core())->GetPlayer();
	_XFilter*pFilter=player->CreateFilter();
	pFilter->m_nFilterID=XFilterBlur;
	pFilter->filter_konckout=0;
//#if (__CORE_VERSION__>=0x02076000)	
	//pFilter->nData=(m_slots_BlurFilterObject.blurX+m_slots_BlurFilterObject.blurY)/2;
	pFilter->pBlur.blurX=m_slots_BlurFilterObject.blurX;
	pFilter->pBlur.blurY=m_slots_BlurFilterObject.blurY;
//#endif
	return pFilter;
}

}