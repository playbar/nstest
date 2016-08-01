#ifndef _AS3MouseCursorData_
#define _AS3MouseCursorData_

#include "AS3Point.h"

namespace avmplus{namespace NativeID{
class MouseCursorDataClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class MouseCursorDataObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class MouseCursorDataClass : public ClassClosure
	{
	public:
		MouseCursorDataClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::MouseCursorDataClassSlots m_slots_MouseCursorDataClass;
};
class MouseCursorDataObject : public ScriptObject
{
	public:
		MouseCursorDataObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		ObjectVectorObject* AS3_data_get();
		void AS3_data_set(ObjectVectorObject *pData);
		double AS3_frameRate_get();
		void AS3_frameRate_set(double frameRate);
		PointObject* AS3_hotSpot_get();
		void AS3_hotSpot_set(PointObject *pHotSpot);

//		void AS3_constructor();
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::MouseCursorDataObjectSlots m_slots_MouseCursorDataObject;
};}
#endif