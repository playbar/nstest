#ifndef _AS3AccessibilityProperties_
#define _AS3AccessibilityProperties_
namespace avmplus{namespace NativeID{
class AccessibilityPropertiesClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class AccessibilityPropertiesObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class AccessibilityPropertiesClass : public ClassClosure
	{
	public:
		AccessibilityPropertiesClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::AccessibilityPropertiesClassSlots m_slots_AccessibilityPropertiesClass;
};
class AccessibilityPropertiesObject : public ScriptObject
{
	public:
		AccessibilityPropertiesObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		DRCWB(Stringp)	m_strDescription;
		bool			m_bForceSimple;
		DRCWB(Stringp)	m_strName;
		bool			m_bNoAutoLabeling;
		DRCWB(Stringp)	m_strShortcut;
		bool			m_bSilent;
		Stringp AS3_description_get();
		void AS3_description_set(Stringp);
		bool AS3_forceSimple_get();
		void AS3_forceSimple_set(bool);
		Stringp AS3_name_get();
		void AS3_name_set(Stringp);
		bool AS3_noAutoLabeling_get();
		void AS3_noAutoLabeling_set(bool);
		Stringp AS3_shortcut_get();
		void AS3_shortcut_set(Stringp);
		bool AS3_silent_get();
		void AS3_silent_set(bool);
		void AS3_constructor();
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::AccessibilityPropertiesObjectSlots m_slots_AccessibilityPropertiesObject;
};}
#endif