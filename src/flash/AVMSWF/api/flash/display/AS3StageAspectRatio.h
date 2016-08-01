#ifndef _AS3StageAspectRatio_
#define _AS3StageAspectRatio_
namespace avmplus{namespace NativeID{
class StageAspectRatioClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

//LANDSCAPE : String = "landscape"
//PORTRAIT : String = "portrait"
//ANY:String = "any"

	Stringp ANY;
	Stringp LANDSCAPE;
	Stringp PORTRAIT;

private:};
class StageAspectRatioObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class StageAspectRatioClass : public ClassClosure
	{
	public:
		StageAspectRatioClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::StageAspectRatioClassSlots m_slots_StageAspectRatioClass;
};
class StageAspectRatioObject : public ScriptObject
{
	public:
		StageAspectRatioObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::StageAspectRatioObjectSlots m_slots_StageAspectRatioObject;
};}
#endif