#ifndef _AS3EncryptedLocalStore_
#define _AS3EncryptedLocalStore_
namespace avmplus{namespace NativeID{
class EncryptedLocalStoreClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class EncryptedLocalStoreObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class EncryptedLocalStoreClass : public ClassClosure
	{
	public:
		EncryptedLocalStoreClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::EncryptedLocalStoreClassSlots m_slots_EncryptedLocalStoreClass;
};
class EncryptedLocalStoreObject : public ScriptObject
{
	public:
		EncryptedLocalStoreObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
		AvmBox AS3_isSupported_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_getItem(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_removeItem(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_reset(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_setItem(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::EncryptedLocalStoreObjectSlots m_slots_EncryptedLocalStoreObject;
};}
#endif