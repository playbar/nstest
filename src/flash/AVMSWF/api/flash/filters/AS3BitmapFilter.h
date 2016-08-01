#ifndef _AS3BitmapFilter_
#define _AS3BitmapFilter_

#include "XFilter.h"

namespace avmplus{namespace NativeID{
class BitmapFilterClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class BitmapFilterObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class BitmapFilterObject;
	class BitmapFilterClass : public ClassClosure
	{
	public:
		BitmapFilterClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
		BitmapFilterObject* CreateFilter(void*data);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::BitmapFilterClassSlots m_slots_BitmapFilterClass;
};
class BitmapFilterObject : public ScriptObject
{
	public:
		BitmapFilterObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		virtual int GetFilterType(){return 0;}
		virtual _XFilter* CreateFilter(){return NULL;}
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::BitmapFilterObjectSlots m_slots_BitmapFilterObject;
};}
#endif