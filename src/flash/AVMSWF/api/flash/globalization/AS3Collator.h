#ifndef _AS3Collator_
#define _AS3Collator_
namespace avmplus{namespace NativeID{
class CollatorClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class CollatorObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class CollatorClass : public ClassClosure
	{
	public:
		CollatorClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);

		ObjectVectorObject* AS3_getAvailableLocaleIDNames();
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::CollatorClassSlots m_slots_CollatorClass;
};
class CollatorObject : public ScriptObject
{
	public:
		CollatorObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		Stringp AS3_actualLocaleIDName_get();
		bool AS3_ignoreCase_get();
		void AS3_ignoreCase_set(bool ignoreCase);
		bool AS3_ignoreCharacterWidth_get();
		void AS3_ignoreCharacterWidth_set(bool ignoreCharacterWidth);
		bool AS3_ignoreDiacritics_get();
		void AS3_ignoreDiacritics_set(bool ignoreDiacritics);
		bool AS3_ignoreKanaType_get();
		void AS3_ignoreKanaType_set(bool ignoreKanaType);
		bool AS3_ignoreSymbols_get();
		void AS3_ignoreSymbols_set(bool ignoreSymbols);
		Stringp AS3_lastOperationStatus_get();
		bool AS3_numericComparison_get();
		void AS3_numericComparison_set(bool numericComparison);
		Stringp AS3_requestedLocaleIDName_get();
		void AS3_constructor(Stringp requestedLocaleIDName, Stringp initialMode);
		int AS3_compare(Stringp string1, Stringp string2);
		bool AS3_equals(Stringp string1, Stringp string2);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::CollatorObjectSlots m_slots_CollatorObject;
};}
#endif