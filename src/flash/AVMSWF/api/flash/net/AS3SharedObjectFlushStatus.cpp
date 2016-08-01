#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3SharedObjectFlushStatus.h"
namespace avmshell{
SharedObjectFlushStatusClass::SharedObjectFlushStatusClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())SharedObjectFlushStatusObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* SharedObjectFlushStatusClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) SharedObjectFlushStatusObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
SharedObjectFlushStatusObject* SharedObjectFlushStatusClass::CreateSharedObjectFlushStatus()
{
	SharedObjectFlushStatusObject *pObj = (SharedObjectFlushStatusObject *)createInstance(ivtable(), prototype);

	return pObj;
}

SharedObjectFlushStatusObject::SharedObjectFlushStatusObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}