#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3InvokeEventReason.h"
namespace avmshell{
InvokeEventReasonClass::InvokeEventReasonClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* InvokeEventReasonClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) InvokeEventReasonObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
InvokeEventReasonObject::InvokeEventReasonObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}