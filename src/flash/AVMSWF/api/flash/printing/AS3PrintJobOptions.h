#ifndef _AS3PrintJobOptions_
#define _AS3PrintJobOptions_
namespace avmplus{namespace NativeID{
class PrintJobOptionsClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class PrintJobOptionsObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class PrintJobOptionsClass : public ClassClosure
	{
	public:
		PrintJobOptionsClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::PrintJobOptionsClassSlots m_slots_PrintJobOptionsClass;
};
class PrintJobOptionsObject : public ScriptObject
{
	public:
		PrintJobOptionsObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		bool m_bAsBitmap;
		bool AS3_printAsBitmap_get(){return m_bAsBitmap;}
		void AS3_printAsBitmap_set(bool b){m_bAsBitmap=b;}
		void AS3_constructor(bool b){m_bAsBitmap=b;}

		//new adds
		double AS3_pixelsPerInch_get();
		void AS3_pixelsPerInch_set(double pixelsPerInch);
		Stringp AS3_printMethod_get();
		void AS3_printMethod_set(Stringp pPrintMethod);
		//new adds end

	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::PrintJobOptionsObjectSlots m_slots_PrintJobOptionsObject;
};}
#endif