#ifndef _AS3InterpolationMethod_
#define _AS3InterpolationMethod_
namespace avmplus{namespace NativeID{
class InterpolationMethodClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!
	//Stringp LINEAR_RGB = "linearRGB";
	//Stringp RGB = "rgb";

	Stringp LINEAR_RGB;
	Stringp RGB;
private:};
class InterpolationMethodObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!

	Stringp LINEAR_RGB;
	Stringp RGB;
private:};
}}
namespace avmshell{
	class InterpolationMethodClass : public ClassClosure
	{
	public:
		InterpolationMethodClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::InterpolationMethodClassSlots m_slots_InterpolationMethodClass;
};
class InterpolationMethodObject : public ScriptObject
{
	public:
		InterpolationMethodObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::InterpolationMethodObjectSlots m_slots_InterpolationMethodObject;
};}
#endif