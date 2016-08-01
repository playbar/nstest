#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3InteractiveIcon.h"
namespace avmshell{
InteractiveIconClass::InteractiveIconClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* InteractiveIconClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) InteractiveIconObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
InteractiveIconObject::InteractiveIconObject(VTable *vtable, ScriptObject* proto, int capacity): IconObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
AvmBox InteractiveIconObject::AS3_bitmaps_get(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox InteractiveIconObject::AS3_bitmaps_set(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox InteractiveIconObject::AS3_height_get(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox InteractiveIconObject::AS3_width_get(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

}