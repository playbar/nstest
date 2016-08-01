#ifndef _AS3HTMLSWFCapability_
#define _AS3HTMLSWFCapability_
namespace avmplus{namespace NativeID{
class HTMLSWFCapabilityClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

//ERROR_INSTALLED_PLAYER_NOT_FOUND : int = 3221
//ERROR_INSTALLED_PLAYER_TOO_OLD : int = 3222
//STATUS_OK : int = 0

	int ERROR_INSTALLED_PLAYER_NOT_FOUND;
	int ERROR_INSTALLED_PLAYER_TOO_OLD;
	int STATUS_OK;

private:};
class HTMLSWFCapabilityObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class HTMLSWFCapabilityClass : public ClassClosure
	{
	public:
		HTMLSWFCapabilityClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::HTMLSWFCapabilityClassSlots m_slots_HTMLSWFCapabilityClass;
};
class HTMLSWFCapabilityObject : public ScriptObject
{
	public:
		HTMLSWFCapabilityObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::HTMLSWFCapabilityObjectSlots m_slots_HTMLSWFCapabilityObject;
};}
#endif