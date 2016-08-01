#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3NativeProcessStartupInfo.h"
namespace avmshell{
NativeProcessStartupInfoClass::NativeProcessStartupInfoClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* NativeProcessStartupInfoClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) NativeProcessStartupInfoObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
NativeProcessStartupInfoObject::NativeProcessStartupInfoObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
AvmBox NativeProcessStartupInfoObject::AS3_arguments_get(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox NativeProcessStartupInfoObject::AS3_arguments_set(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox NativeProcessStartupInfoObject::AS3_executable_get(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox NativeProcessStartupInfoObject::AS3_executable_set(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox NativeProcessStartupInfoObject::AS3_workingDirectory_get(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox NativeProcessStartupInfoObject::AS3_workingDirectory_set(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

//AS3 contructor function..
AvmBox NativeProcessStartupInfoObject::AS3_constructor(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

}