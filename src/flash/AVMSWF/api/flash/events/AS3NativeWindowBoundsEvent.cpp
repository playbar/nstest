#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3NativeWindowBoundsEvent.h"
namespace avmshell{
NativeWindowBoundsEventClass::NativeWindowBoundsEventClass(VTable* cvtable):ClassClosure(cvtable)//EventClass(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* NativeWindowBoundsEventClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) NativeWindowBoundsEventObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
NativeWindowBoundsEventObject::NativeWindowBoundsEventObject(VTable *vtable, ScriptObject* proto, int capacity): EventObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
RectangleObject* NativeWindowBoundsEventObject::AS3_afterBounds_get()
{
	return NULL;
}
RectangleObject* NativeWindowBoundsEventObject::AS3_beforeBounds_get()
{
	return NULL;
}

void NativeWindowBoundsEventObject::AS3_constructor(Stringp pType, bool bubbles, bool cancelable, 
													RectangleObject *pBeforeBounds, RectangleObject *pAfterBounds)
{

}
EventObject* NativeWindowBoundsEventObject::AS3_clone()
{
	return NULL;
}
Stringp NativeWindowBoundsEventObject::AS3_toString()
{
	return NULL;
}

}