#ifndef _AS3StorageVolumeInfo_
#define _AS3StorageVolumeInfo_
namespace avmplus{namespace NativeID{
class StorageVolumeInfoClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class StorageVolumeInfoObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class StorageVolumeInfoClass : public ClassClosure
	{
	public:
		StorageVolumeInfoClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::StorageVolumeInfoClassSlots m_slots_StorageVolumeInfoClass;
};
class StorageVolumeInfoObject : public ScriptObject
{
	public:
		StorageVolumeInfoObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
		AvmBox AS3_isSupported_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_storageVolumeInfo_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_getStorageVolumes(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::StorageVolumeInfoObjectSlots m_slots_StorageVolumeInfoObject;
};}
#endif