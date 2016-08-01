#ifndef _AS3StorageVolume_
#define _AS3StorageVolume_
namespace avmplus{namespace NativeID{
class StorageVolumeClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class StorageVolumeObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class StorageVolumeClass : public ClassClosure
	{
	public:
		StorageVolumeClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::StorageVolumeClassSlots m_slots_StorageVolumeClass;
};
class StorageVolumeObject : public ScriptObject
{
	public:
		StorageVolumeObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
		AvmBox AS3_drive_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_fileSystemType_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_isRemovable_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_isWritable_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_name_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_rootDirectory_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_constructor(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::StorageVolumeObjectSlots m_slots_StorageVolumeObject;
};}
#endif