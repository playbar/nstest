#ifndef _AS3GradientType_
#define _AS3GradientType_
namespace avmplus{namespace NativeID{
class GradientTypeClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!
	//Stringp LINEAR = "linear"; 
	//Stringp RADIAL = "radial";

	Stringp LINEAR; 
	Stringp RADIAL;
private:};
class GradientTypeObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class GradientTypeClass : public ClassClosure
	{
	public:
		GradientTypeClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::GradientTypeClassSlots m_slots_GradientTypeClass;
	public:
		inline Stringp getSlotLINEAR(){return m_slots_GradientTypeClass.LINEAR;}
		inline Stringp getSlotRADIAL(){return m_slots_GradientTypeClass.RADIAL;}
};
class GradientTypeObject : public ScriptObject
{
	public:
		GradientTypeObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::GradientTypeObjectSlots m_slots_GradientTypeObject;
};}
#endif