#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3NativeDragOptions.h"
namespace avmshell{
NativeDragOptionsClass::NativeDragOptionsClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* NativeDragOptionsClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) NativeDragOptionsObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
NativeDragOptionsObject::NativeDragOptionsObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
AvmBox NativeDragOptionsObject::AS3_allowCopy_get(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox NativeDragOptionsObject::AS3_allowCopy_set(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox NativeDragOptionsObject::AS3_allowLink_get(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox NativeDragOptionsObject::AS3_allowLink_set(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox NativeDragOptionsObject::AS3_allowMove_get(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox NativeDragOptionsObject::AS3_allowMove_set(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox NativeDragOptionsObject::AS3_toString(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

}