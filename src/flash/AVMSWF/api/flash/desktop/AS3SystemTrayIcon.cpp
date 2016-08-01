#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3SystemTrayIcon.h"
namespace avmshell{
SystemTrayIconClass::SystemTrayIconClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* SystemTrayIconClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) SystemTrayIconObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
SystemTrayIconObject::SystemTrayIconObject(VTable *vtable, ScriptObject* proto, int capacity): InteractiveIconObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
AvmBox SystemTrayIconObject::AS3_bitmaps_get(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox SystemTrayIconObject::AS3_bitmaps_set(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox SystemTrayIconObject::AS3_height_get(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox SystemTrayIconObject::AS3_menu_get(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox SystemTrayIconObject::AS3_menu_set(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox SystemTrayIconObject::AS3_tooltip_get(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox SystemTrayIconObject::AS3_tooltip_set(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox SystemTrayIconObject::AS3_width_get(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

}