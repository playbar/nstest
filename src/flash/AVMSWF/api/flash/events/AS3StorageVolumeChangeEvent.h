#ifndef _AS3StorageVolumeChangeEvent_
#define _AS3StorageVolumeChangeEvent_

#include "AS3Event.h"
#include "AS3File.h"
#include "AS3StorageVolume.h"

namespace avmplus{namespace NativeID{
class StorageVolumeChangeEventClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

	//STORAGE_VOLUME_MOUNT : String = "storageVolumeMount"
	//STORAGE_VOLUME_UNMOUNT : String = "storageVolumeUnmount"

	Stringp STORAGE_VOLUME_MOUNT;
	Stringp STORAGE_VOLUME_UNMOUNT;

private:};
class StorageVolumeChangeEventObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class StorageVolumeChangeEventClass : public ClassClosure//EventClass
	{
	public:
		StorageVolumeChangeEventClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::StorageVolumeChangeEventClassSlots m_slots_StorageVolumeChangeEventClass;
};
class StorageVolumeChangeEventObject : public EventObject
{
	public:
		StorageVolumeChangeEventObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
		FileObject* AS3_rootDirectory_get();
		StorageVolumeObject* AS3_storageVolume_get();

		void AS3_constructor(Stringp pType, bool bubbles, bool cancelable, FileObject *pPath, StorageVolumeObject *pVolume);
		EventObject* AS3_clone();
		Stringp AS3_toString();
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::StorageVolumeChangeEventObjectSlots m_slots_StorageVolumeChangeEventObject;
};}
#endif