#ifndef _AS3AccessibilityImplementation_
#define _AS3AccessibilityImplementation_
namespace avmplus{namespace NativeID{
class AccessibilityImplementationClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class AccessibilityImplementationObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
	XU32	errno;
	XBOOL	stub;
private:};
}}
namespace avmshell{
	class AccessibilityImplementationClass : public ClassClosure
	{
	public:
		AccessibilityImplementationClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
		bool AS3_active_get();
		void AS3_updateProperties();
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::AccessibilityImplementationClassSlots m_slots_AccessibilityImplementationClass;
};
class AccessibilityImplementationObject : public ScriptObject
{
	public:
		AccessibilityImplementationObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:		
		
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::AccessibilityImplementationObjectSlots m_slots_AccessibilityImplementationObject;
};}
#endif