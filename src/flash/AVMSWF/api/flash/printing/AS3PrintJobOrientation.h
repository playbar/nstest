#ifndef _AS3PrintJobOrientation_
#define _AS3PrintJobOrientation_
namespace avmplus{namespace NativeID{
class PrintJobOrientationClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!
	//Stringp LANDSCAPE = "landscape";
	//Stringp PORTRAIT = "portrait";

	Stringp LANDSCAPE;
	Stringp PORTRAIT;
private:};
class PrintJobOrientationObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!	
private:};
}}
namespace avmshell{
	class PrintJobOrientationClass : public ClassClosure
	{
	public:
		PrintJobOrientationClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	public:
		inline Stringp getSlotLANDSCAPE(){return m_slots_PrintJobOrientationClass.LANDSCAPE;}
		inline Stringp getSlotPORTRAIT(){return m_slots_PrintJobOrientationClass.PORTRAIT;}
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::PrintJobOrientationClassSlots m_slots_PrintJobOrientationClass;
};
class PrintJobOrientationObject : public ScriptObject
{
	public:
		PrintJobOrientationObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::PrintJobOrientationObjectSlots m_slots_PrintJobOrientationObject;
};}
#endif