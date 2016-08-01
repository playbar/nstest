#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3TextRenderer.h"
namespace avmshell{
TextRendererClass::TextRendererClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())TextRendererObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* TextRendererClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) TextRendererObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
TextRendererObject::TextRendererObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
Stringp TextRendererClass::AS3_displayMode_get()
{
	//Add your act code here...
	return NULL;
}

void TextRendererClass::AS3_displayMode_set(Stringp)
{
	//Add your act code here...	
}

int TextRendererClass::AS3_maxLevel_get()
{
	//Add your act code here...
	return 0;//Modify this please!
}

void TextRendererClass::AS3_maxLevel_set(int)
{
	//Add your act code here...
	
}

void TextRendererClass::AS3_setAdvancedAntiAliasingTable(Stringp,Stringp,Stringp,ArrayObject*)
{
	//Add your act code here...

}

}