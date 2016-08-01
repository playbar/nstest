#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3PrintJobOrientation.h"
namespace avmshell{
PrintJobOrientationClass::PrintJobOrientationClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())PrintJobOrientationObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* PrintJobOrientationClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) PrintJobOrientationObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
PrintJobOrientationObject::PrintJobOrientationObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}