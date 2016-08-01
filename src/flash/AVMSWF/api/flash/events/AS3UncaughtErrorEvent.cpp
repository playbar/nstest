#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3UncaughtErrorEvent.h"
namespace avmshell{
UncaughtErrorEventClass::UncaughtErrorEventClass(VTable* cvtable):ClassClosure(cvtable)//ErrorEventClass(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* UncaughtErrorEventClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) UncaughtErrorEventObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
UncaughtErrorEventObject::UncaughtErrorEventObject(VTable *vtable, ScriptObject* proto, int capacity): ErrorEventObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
//Atom UncaughtErrorEventObject::AS3_error_get()
//{
//	//Add your act code here...
//	return kAvmThunkUndefined;//Modify this please!
//}
//
////AS3 contructor function..
//void UncaughtErrorEventObject::AS3_constructor(Stringp pType, bool bubbles, bool cancelable, Atom error_in)
//{
//	//Add your act code here...
//}
//
//EventObject* UncaughtErrorEventObject::AS3_clone()
//{
//	//Add your act code here...
//	return NULL;//Modify this please!
//}
//
//Stringp UncaughtErrorEventObject::AS3_toString()
//{
//	//Add your act code here...
//	return NULL;//Modify this please!
//}

}