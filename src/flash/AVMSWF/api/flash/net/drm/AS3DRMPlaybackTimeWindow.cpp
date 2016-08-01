#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3DRMPlaybackTimeWindow.h"
namespace avmshell{
DRMPlaybackTimeWindowClass::DRMPlaybackTimeWindowClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* DRMPlaybackTimeWindowClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) DRMPlaybackTimeWindowObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
DRMPlaybackTimeWindowObject::DRMPlaybackTimeWindowObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
//AvmBox DRMPlaybackTimeWindowObject::AS3_endDate_get(AvmMethodEnv, uint32_t, AvmBox* argv)
//{
//	//Add your act code here...
//	return kAvmThunkUndefined;//Modify this please!
//}
//
//AvmBox DRMPlaybackTimeWindowObject::AS3_period_get(AvmMethodEnv, uint32_t, AvmBox* argv)
//{
//	//Add your act code here...
//	return kAvmThunkUndefined;//Modify this please!
//}
//
//AvmBox DRMPlaybackTimeWindowObject::AS3_startDate_get(AvmMethodEnv, uint32_t, AvmBox* argv)
//{
//	//Add your act code here...
//	return kAvmThunkUndefined;//Modify this please!
//}

}