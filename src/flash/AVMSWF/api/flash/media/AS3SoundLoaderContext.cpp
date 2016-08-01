#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3SoundLoaderContext.h"
namespace avmshell{
SoundLoaderContextClass::SoundLoaderContextClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())SoundLoaderContextObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* SoundLoaderContextClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) SoundLoaderContextObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
SoundLoaderContextObject::SoundLoaderContextObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
/*AvmBox SoundLoaderContextObject::AS3_bufferTime_get(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox SoundLoaderContextObject::AS3_bufferTime_set(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox SoundLoaderContextObject::AS3_checkPolicyFile_get(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox SoundLoaderContextObject::AS3_checkPolicyFile_set(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

//AS3 contructor function..
AvmBox SoundLoaderContextObject::AS3_constructor(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}*/

}