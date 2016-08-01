#ifndef _AS3PerspectiveProjection_
#define _AS3PerspectiveProjection_

#include "AS3Point.h"
#include "AS3Matrix3D.h"

namespace avmplus{namespace NativeID{
class PerspectiveProjectionClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class PerspectiveProjectionObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class PerspectiveProjectionObject;
	class PerspectiveProjectionClass : public ClassClosure
	{
	public:
		PerspectiveProjectionClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
		PerspectiveProjectionObject* CreateObject();
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::PerspectiveProjectionClassSlots m_slots_PerspectiveProjectionClass;
};
class PerspectiveProjectionObject : public ScriptObject
{
	public:
		PerspectiveProjectionObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		double AS3_fieldOfView_get();
		void AS3_fieldOfView_set(double fieldOfView);
		double AS3_focalLength_get();
		void AS3_focalLength_set(double focalLength);
		PointObject* AS3_projectionCenter_get();
		void AS3_projectionCenter_set(PointObject *pProjectionCenter);
		void AS3_constructor();
		Matrix3DObject* AS3_toMatrix3D();
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::PerspectiveProjectionObjectSlots m_slots_PerspectiveProjectionObject;
};}
#endif