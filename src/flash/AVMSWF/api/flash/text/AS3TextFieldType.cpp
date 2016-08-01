#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3TextFieldType.h"
namespace avmshell{
TextFieldTypeClass::TextFieldTypeClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())TextFieldTypeObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* TextFieldTypeClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) TextFieldTypeObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
TextFieldTypeObject::TextFieldTypeObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}