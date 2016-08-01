#ifndef _AS3GridFitType_
#define _AS3GridFitType_
namespace avmplus{namespace NativeID{
class GridFitTypeClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!
	//Stringp NONE = "none"; 
	//Stringp PIXEL = "pixel"; 
	//Stringp SUBPIXEL = "subpixel"; 

	Stringp NONE; 
	Stringp PIXEL; 
	Stringp SUBPIXEL; 
private:};
class GridFitTypeObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!

	Stringp NONE; 
	Stringp PIXEL; 
	Stringp SUBPIXEL; 
private:};
}}
namespace avmshell{
	class GridFitTypeClass : public ClassClosure
	{
	public:
		GridFitTypeClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::GridFitTypeClassSlots m_slots_GridFitTypeClass;
	public:
		inline Stringp getSlotNONE(){return m_slots_GridFitTypeClass.NONE;} 
		inline Stringp getSlotPIXEL(){return m_slots_GridFitTypeClass.PIXEL;} 
		inline Stringp getSlotSUBPIXEL(){return m_slots_GridFitTypeClass.SUBPIXEL;}
};
class GridFitTypeObject : public ScriptObject
{
	public:
		GridFitTypeObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		//void* pData;//Set your data!!
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::GridFitTypeObjectSlots m_slots_GridFitTypeObject;
};}
#endif