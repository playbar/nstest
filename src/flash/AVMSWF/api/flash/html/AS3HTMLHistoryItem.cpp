#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3HTMLHistoryItem.h"
namespace avmshell{
HTMLHistoryItemClass::HTMLHistoryItemClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* HTMLHistoryItemClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) HTMLHistoryItemObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
HTMLHistoryItemObject::HTMLHistoryItemObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
AvmBox HTMLHistoryItemObject::AS3_isPost_get(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox HTMLHistoryItemObject::AS3_originalUrl_get(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox HTMLHistoryItemObject::AS3_title_get(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox HTMLHistoryItemObject::AS3_url_get(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

}