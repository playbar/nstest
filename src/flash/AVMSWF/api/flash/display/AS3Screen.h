#ifndef _AS3Screen_
#define _AS3Screen_

#include "AS3EventDispatcher.h"

#include "AS3Rectangle.h"

namespace avmplus{namespace NativeID{
class ScreenClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class ScreenObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class ScreenObject;

	class ScreenClass : public ClassClosure
	{
	public:
		ScreenClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);

		ScreenObject* AS3_mainScreen_get();
		ArrayObject* AS3_screens_get();

	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ScreenClassSlots m_slots_ScreenClass;
};
class ScreenObject : public EventDispatcherObject
{
	public:
		ScreenObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
		RectangleObject* AS3_bounds_get();
		int AS3_colorDepth_get();
		RectangleObject* AS3_visibleBounds_get();

		ArrayObject* AS3_getScreensForRectangle(RectangleObject *pRect);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ScreenObjectSlots m_slots_ScreenObject;
};}
#endif