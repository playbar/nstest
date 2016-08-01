#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3StorageVolumeChangeEvent.h"
namespace avmshell{
StorageVolumeChangeEventClass::StorageVolumeChangeEventClass(VTable* cvtable):ClassClosure(cvtable)//EventClass(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* StorageVolumeChangeEventClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) StorageVolumeChangeEventObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
StorageVolumeChangeEventObject::StorageVolumeChangeEventObject(VTable *vtable, ScriptObject* proto, int capacity): EventObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
FileObject* StorageVolumeChangeEventObject::AS3_rootDirectory_get()
{
	return NULL;
}
StorageVolumeObject* StorageVolumeChangeEventObject::AS3_storageVolume_get()
{
	return NULL;
}

void StorageVolumeChangeEventObject::AS3_constructor(Stringp pType, bool bubbles, bool cancelable, FileObject *pPath, StorageVolumeObject *pVolume)
{

}
EventObject* StorageVolumeChangeEventObject::AS3_clone()
{
	return NULL;
}
Stringp StorageVolumeChangeEventObject::AS3_toString()
{
	return NULL;
}

}