#ifndef _AS3PixelSnapping_
#define _AS3PixelSnapping_
namespace avmplus{namespace NativeID{
class PixelSnappingClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!
	//Stringp ALWAYS = "always";
	//Stringp AUTO = "auto";
	//Stringp NEVER = "never"; 

	Stringp ALWAYS;
	Stringp AUTO;
	Stringp NEVER; 
private:};
class PixelSnappingObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!

	Stringp ALWAYS;
	Stringp AUTO;
	Stringp NEVER; 
private:};
}}
namespace avmshell{
	class PixelSnappingClass : public ClassClosure
	{
	public:
		PixelSnappingClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::PixelSnappingClassSlots m_slots_PixelSnappingClass;
};
class PixelSnappingObject : public ScriptObject
{
	public:
		PixelSnappingObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::PixelSnappingObjectSlots m_slots_PixelSnappingObject;
};}
#endif