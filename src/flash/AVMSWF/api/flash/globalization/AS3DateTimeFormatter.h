#ifndef _AS3DateTimeFormatter_
#define _AS3DateTimeFormatter_
namespace avmplus{namespace NativeID{
class DateTimeFormatterClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class DateTimeFormatterObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class DateTimeFormatterClass : public ClassClosure
	{
	public:
		DateTimeFormatterClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);

		ObjectVectorObject* AS3_getAvailableLocaleIDNames();
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::DateTimeFormatterClassSlots m_slots_DateTimeFormatterClass;
};
class DateTimeFormatterObject : public ScriptObject
{
	public:
		DateTimeFormatterObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		Stringp AS3_actualLocaleIDName_get();
		Stringp AS3_lastOperationStatus_get();
		Stringp AS3_requestedLocaleIDName_get();

		void AS3_constructor(Stringp requestedLocaleIDName, Stringp dateStyle, Stringp timeStyle);
		Stringp AS3_format(DateObject *pDateTime);
		Stringp AS3_formatUTC(DateObject *pDateTime);
		Stringp AS3_getDateStyle();
		Stringp AS3_getDateTimePattern();
		int AS3_getFirstWeekday();
		ObjectVectorObject* AS3_getMonthNames(Stringp nameStyle, Stringp context);
		Stringp AS3_getTimeStyle();
		ObjectVectorObject* AS3_getWeekdayNames(Stringp nameStyle, Stringp context);
		void AS3_setDateTimePattern(Stringp pattern);
		void AS3_setDateTimeStyles(Stringp dateStyle, Stringp timeStyle);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::DateTimeFormatterObjectSlots m_slots_DateTimeFormatterObject;
};}
#endif