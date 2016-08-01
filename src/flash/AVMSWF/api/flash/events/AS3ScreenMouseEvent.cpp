#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3ScreenMouseEvent.h"
namespace avmshell{
ScreenMouseEventClass::ScreenMouseEventClass(VTable* cvtable):ClassClosure(cvtable)//MouseEventClass(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* ScreenMouseEventClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) ScreenMouseEventObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
ScreenMouseEventObject::ScreenMouseEventObject(VTable *vtable, ScriptObject* proto, int capacity): MouseEventObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
double ScreenMouseEventObject::AS3_screenX_get()
{
	return 0;
}
double ScreenMouseEventObject::AS3_screenY_get()
{
	return 0;
}

void ScreenMouseEventObject::AS3_constructor(Stringp pType, bool bubbles, bool cancelable, double screenX, double screenY, 
											 bool ctrlKey, bool altKey, bool shiftKey, bool buttonDown, 
											 bool commandKey, bool controlKey)
{

}
EventObject* ScreenMouseEventObject::AS3_clone()
{
	return NULL;
}
Stringp ScreenMouseEventObject::AS3_toString()
{
	return NULL;
}

}