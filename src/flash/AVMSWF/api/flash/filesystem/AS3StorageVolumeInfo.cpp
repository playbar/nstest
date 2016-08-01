#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3StorageVolumeInfo.h"
namespace avmshell{
StorageVolumeInfoClass::StorageVolumeInfoClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* StorageVolumeInfoClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) StorageVolumeInfoObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
StorageVolumeInfoObject::StorageVolumeInfoObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
AvmBox StorageVolumeInfoObject::AS3_isSupported_get(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox StorageVolumeInfoObject::AS3_storageVolumeInfo_get(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox StorageVolumeInfoObject::AS3_getStorageVolumes(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

}