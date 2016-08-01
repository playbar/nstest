#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3SQLCollationType.h"
namespace avmshell{
SQLCollationTypeClass::SQLCollationTypeClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* SQLCollationTypeClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) SQLCollationTypeObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
SQLCollationTypeObject::SQLCollationTypeObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}