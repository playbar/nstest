#ifndef _AS3TextRotation_
#define _AS3TextRotation_
namespace avmplus{namespace NativeID{
class TextRotationClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

//AUTO : String = "auto"
//ROTATE_0 : String = "rotate0"
//ROTATE_180 : String = "rotate180"
//ROTATE_270 : String = "rotate270"
//ROTATE_90 : String = "rotate90"

	Stringp AUTO;
	Stringp ROTATE_0;
	Stringp ROTATE_180;
	Stringp ROTATE_270;
	Stringp ROTATE_90;

private:};
class TextRotationObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class TextRotationClass : public ClassClosure
	{
	public:
		TextRotationClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
		Stringp getROTATE_0(){return m_slots_TextRotationClass.ROTATE_0;}
		Stringp getROTATE_180(){return m_slots_TextRotationClass.ROTATE_180;}
		Stringp getROTATE_270(){return m_slots_TextRotationClass.ROTATE_270;}
		Stringp getROTATE_90(){return m_slots_TextRotationClass.ROTATE_90;}
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::TextRotationClassSlots m_slots_TextRotationClass;
};
class TextRotationObject : public ScriptObject
{
	public:
		TextRotationObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::TextRotationObjectSlots m_slots_TextRotationObject;
};}
#endif