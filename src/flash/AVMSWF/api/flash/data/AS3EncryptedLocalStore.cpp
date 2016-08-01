#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3EncryptedLocalStore.h"
namespace avmshell{
EncryptedLocalStoreClass::EncryptedLocalStoreClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* EncryptedLocalStoreClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) EncryptedLocalStoreObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
EncryptedLocalStoreObject::EncryptedLocalStoreObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
AvmBox EncryptedLocalStoreObject::AS3_isSupported_get(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox EncryptedLocalStoreObject::AS3_getItem(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox EncryptedLocalStoreObject::AS3_removeItem(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox EncryptedLocalStoreObject::AS3_reset(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox EncryptedLocalStoreObject::AS3_setItem(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

}