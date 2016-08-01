#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3GridFitType.h"
namespace avmshell{
GridFitTypeClass::GridFitTypeClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())GridFitTypeObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* GridFitTypeClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) GridFitTypeObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
GridFitTypeObject::GridFitTypeObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}