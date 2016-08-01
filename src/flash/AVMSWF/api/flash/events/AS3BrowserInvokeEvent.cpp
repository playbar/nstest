#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3BrowserInvokeEvent.h"
namespace avmshell{
BrowserInvokeEventClass::BrowserInvokeEventClass(VTable* cvtable):ClassClosure(cvtable)//EventClass(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* BrowserInvokeEventClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) BrowserInvokeEventObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
BrowserInvokeEventObject::BrowserInvokeEventObject(VTable *vtable, ScriptObject* proto, int capacity): EventObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
ArrayObject* BrowserInvokeEventObject::AS3_arguments_get()
{
	return NULL;
}
bool BrowserInvokeEventObject::AS3_isHTTPS_get()
{
	return false;
}
bool BrowserInvokeEventObject::AS3_isUserEvent_get()
{
	return false;
}
Stringp BrowserInvokeEventObject::AS3_sandboxType_get()
{
	return NULL;
}
Stringp BrowserInvokeEventObject::AS3_securityDomain_get()
{
	return NULL;
}

void BrowserInvokeEventObject::AS3_constructor(Stringp pType, bool bubbles, bool cancelable, 
											   ArrayObject *pArguments, Stringp pSandboxType, Stringp pSecurityDomain, 
											   bool isHTTPS, bool isUserEvent)
{

}
EventObject* BrowserInvokeEventObject::AS3_clone()
{
	return NULL;
}

}
