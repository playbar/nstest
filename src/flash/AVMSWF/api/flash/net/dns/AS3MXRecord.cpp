#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3MXRecord.h"
namespace avmshell{
MXRecordClass::MXRecordClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* MXRecordClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) MXRecordObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
MXRecordObject::MXRecordObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
AvmBox MXRecordObject::AS3_exchange_get(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox MXRecordObject::AS3_exchange_set(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox MXRecordObject::AS3_preference_get(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox MXRecordObject::AS3_preference_set(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

//AS3 contructor function..
AvmBox MXRecordObject::AS3_constructor(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

}