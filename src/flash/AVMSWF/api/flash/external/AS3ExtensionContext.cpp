#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3ExtensionContext.h"
namespace avmshell{
ExtensionContextClass::ExtensionContextClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* ExtensionContextClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) ExtensionContextObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
ExtensionContextObject::ExtensionContextObject(VTable *vtable, ScriptObject* proto, int capacity): EventDispatcherObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
Atom ExtensionContextObject::AS3_actionScriptData_get()
{
	return kAvmThunkUndefined;
}
void ExtensionContextObject::AS3_actionScriptData_set(Atom pActionScriptData)
{

}

ExtensionContextObject* ExtensionContextClass::AS3_createExtensionContext(Stringp pExtensionID, Stringp pContextType)
{
	return NULL;
}
FileObject* ExtensionContextClass::AS3_getExtensionDirectory(Stringp pExtensionID)
{
	return NULL;
}
Atom ExtensionContextObject::AS3_call(Stringp pFunctionName, ArrayObject *pArgs)
{
	return kAvmThunkUndefined;
}
void ExtensionContextObject::AS3_dispose()
{

}

}