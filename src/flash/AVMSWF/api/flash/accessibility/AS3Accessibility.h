#ifndef _AS3Accessibility_
#define _AS3Accessibility_
namespace avmplus{namespace NativeID{
class AccessibilityClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class AccessibilityObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class DisplayObjectObject;
	class AccessibilityClass : public ClassClosure
	{
	public:
		AccessibilityClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
		bool AS3_active_get();
		void AS3_updateProperties();
		void AS3_sendEvent(DisplayObjectObject*,XU32,XU32,bool);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::AccessibilityClassSlots m_slots_AccessibilityClass;
};
class AccessibilityObject : public ScriptObject
{
	public:
		AccessibilityObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:		
		
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::AccessibilityObjectSlots m_slots_AccessibilityObject;
};}
#endif