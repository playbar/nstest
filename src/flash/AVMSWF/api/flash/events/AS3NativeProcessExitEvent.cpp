#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3NativeProcessExitEvent.h"
namespace avmshell{
NativeProcessExitEventClass::NativeProcessExitEventClass(VTable* cvtable):ClassClosure(cvtable)//EventClass(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* NativeProcessExitEventClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) NativeProcessExitEventObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
NativeProcessExitEventObject::NativeProcessExitEventObject(VTable *vtable, ScriptObject* proto, int capacity): EventObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
double NativeProcessExitEventObject::AS3_exitCode_get()
{
	return 0;
}
void NativeProcessExitEventObject::AS3_exitCode_set(double pExitCode)
{

}

void NativeProcessExitEventObject::AS3_constructor(Stringp pType, bool bubbles, bool cancelable, double exitCode)
{

}
EventObject* NativeProcessExitEventObject::AS3_clone()
{
	return NULL;
}
Stringp NativeProcessExitEventObject::AS3_toString()
{
	return NULL;
}

}