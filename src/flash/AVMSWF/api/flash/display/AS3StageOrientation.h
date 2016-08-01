#ifndef _AS3StageOrientation_
#define _AS3StageOrientation_
namespace avmplus{namespace NativeID{
class StageOrientationClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

//DEFAULT : Stringp = "default"
//ROTATED_LEFT : Stringp = "rotatedLeft"
//ROTATED_RIGHT : Stringp = "rotatedRight"
//UNKNOWN : Stringp = "unknown"
//UPSIDE_DOWN : Stringp = "upsideDown"

	Stringp DEFAULT;
	Stringp ROTATED_LEFT;
	Stringp ROTATED_RIGHT;
	Stringp UNKNOWN;
	Stringp UPSIDE_DOWN;

private:};
class StageOrientationObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class StageOrientationClass : public ClassClosure
	{
	public:
		StageOrientationClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::StageOrientationClassSlots m_slots_StageOrientationClass;
};
class StageOrientationObject : public ScriptObject
{
	public:
		StageOrientationObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::StageOrientationObjectSlots m_slots_StageOrientationObject;
};}
#endif