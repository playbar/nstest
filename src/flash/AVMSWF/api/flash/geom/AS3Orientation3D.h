#ifndef _AS3Orientation3D_
#define _AS3Orientation3D_
namespace avmplus{namespace NativeID{
class Orientation3DClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

//AXIS_ANGLE : String = "axisAngle"
//EULER_ANGLES : String = "eulerAngles"
//QUATERNION : String = "quaternion"

	Stringp AXIS_ANGLE;
	Stringp EULER_ANGLES;
	Stringp QUATERNION;

private:};
class Orientation3DObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class Orientation3DClass : public ClassClosure
	{
	public:
		Orientation3DClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::Orientation3DClassSlots m_slots_Orientation3DClass;
};
class Orientation3DObject : public ScriptObject
{
	public:
		Orientation3DObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::Orientation3DObjectSlots m_slots_Orientation3DObject;
};}
#endif