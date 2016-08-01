#include "StdAfxflash.h"
#include "sobject.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3StaticText.h"
namespace avmshell{
StaticTextClass::StaticTextClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())StaticTextObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* StaticTextClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) StaticTextObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
StaticTextObject::StaticTextObject(VTable *vtable, ScriptObject* proto, int capacity): DisplayObjectObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...

Stringp StaticTextObject::AS3_text_get()
{
	//Add your act code here...
	//return kAvmThunkUndefined;//Modify this please!
	if(m_pObject)
	{
		int l=0;
		U16*pData=m_pObject->AS3GetStaticText(l);
		if(pData)
		{
			Stringp str=core()->newStringUTF16(pData,l);
			delete pData;
			return str;
		}
	}
	return NULL;//core()->kEmptyString;
}

}