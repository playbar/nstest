#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3SQLTriggerSchema.h"
namespace avmshell{
SQLTriggerSchemaClass::SQLTriggerSchemaClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* SQLTriggerSchemaClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) SQLTriggerSchemaObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
SQLTriggerSchemaObject::SQLTriggerSchemaObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
AvmBox SQLTriggerSchemaObject::AS3_table_get(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

//AS3 contructor function..
AvmBox SQLTriggerSchemaObject::AS3_constructor(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

}