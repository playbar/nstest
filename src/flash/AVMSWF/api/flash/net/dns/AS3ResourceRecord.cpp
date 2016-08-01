#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3ResourceRecord.h"
namespace avmshell{
ResourceRecordClass::ResourceRecordClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* ResourceRecordClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) ResourceRecordObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
ResourceRecordObject::ResourceRecordObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
AvmBox ResourceRecordObject::AS3_name_get(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox ResourceRecordObject::AS3_name_set(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox ResourceRecordObject::AS3_ttl_get(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox ResourceRecordObject::AS3_ttl_set(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

}