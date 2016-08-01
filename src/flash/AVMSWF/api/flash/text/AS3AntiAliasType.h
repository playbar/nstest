#ifndef _AS3AntiAliasType_
#define _AS3AntiAliasType_
namespace avmplus{namespace NativeID{
class AntiAliasTypeClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!
//	Stringp ADVANCED = "advanced";
//	Stringp NORMAL = "normal";

	Stringp ADVANCED;
	Stringp NORMAL;
private:};
class AntiAliasTypeObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!

private:};
}}
namespace avmshell{
	class AntiAliasTypeClass : public ClassClosure
	{
	public:
		AntiAliasTypeClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::AntiAliasTypeClassSlots m_slots_AntiAliasTypeClass;
	public:		
		inline Stringp getSlotADVANCED(){return m_slots_AntiAliasTypeClass.ADVANCED;}
		inline Stringp getSlotNORMAL(){return m_slots_AntiAliasTypeClass.NORMAL;}
};
class AntiAliasTypeObject : public ScriptObject
{
	public:
		AntiAliasTypeObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::AntiAliasTypeObjectSlots m_slots_AntiAliasTypeObject;
};}
#endif